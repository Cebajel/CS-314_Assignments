#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void get_rows_cols(ifstream &input_image, string &buffer, int &rows, int &cols);
int ***generate_image(int rows, int cols);
int ***read_image(int rows, int cols, ifstream &input_image, string &buffer);
void grayscale_transformation(int ***original_image, int pipefd, int rows, int cols);
void flipping_transformation(int ***original_image, int pipefd, int rows, int cols);
int read_shared_pipe(int pipefd);
void write_shared_pipe(int pipefd, int temp);
void write_image(int ***image, ofstream &output_image, int rows, int cols);
void delete_image(int ***image, int rows, int cols);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        // Correct number of arguments are not passed
        cout << "Syntax : ./a.out <path-to-original-image> <path-to-transformed-image>\n";
        exit(EXIT_FAILURE);
    }

    // Defining file descriptiors and buffers for reading files along with variables for rows and columns.
    ifstream input_image;
    ofstream output_image;
    string buffer;
    int rows = 0, cols = 0;
    int pipefd[2];
    pipe(pipefd);

    try
    {
        // Opening given files
        input_image.open(argv[1]);
        output_image.open(argv[2]);
    }
    catch (exception e1)
    {
        // Catching exceptions
        cout << e1.what() << endl;
        exit(EXIT_FAILURE);
    }

    // Fetching number of rows and columns
    get_rows_cols(input_image, buffer, rows, cols);
    // Converting ppm image to an 3D array
    int ***image = read_image(rows, cols, input_image, buffer);
    // Creating two 3D array representing two independent images
    int ***modified_image = generate_image(rows, cols);

    int pid = fork();

    if (pid == 0)
    {
        // Child Process
        close(pipefd[1]); //close the write of child
        flipping_transformation(modified_image, pipefd[0], rows, cols);
        close(pipefd[0]);
    }
    else
    {
        // Parent Process
        close(pipefd[0]); // close the read of parent
        grayscale_transformation(image, pipefd[1], rows, cols);
        close(pipefd[1]);
        delete_image(image, rows, cols);
        delete_image(modified_image, rows, cols);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);

    // Converting the final image to ppm format
    write_image(modified_image, output_image, rows, cols);

    // Deallocating memory assigned to all the image matrices
    delete_image(image, rows, cols);
    delete_image(modified_image, rows, cols);

    // Closing file decriptors
    input_image.close();
    output_image.close();

    return EXIT_SUCCESS;
}

void get_rows_cols(ifstream &input_image, string &buffer, int &rows, int &cols)
{
    if (!input_image)
    {
        cout << "File Not Found\n";
        exit(EXIT_FAILURE);
    }
    input_image >> buffer;
    input_image >> buffer;
    while (buffer == "#")
    {
        getline(input_image, buffer);
        input_image >> buffer;
    }
    cols = stoi(buffer);
    input_image >> buffer;
    rows = stoi(buffer);
    cout << "Rows: " << rows << " | "
         << " Columns: " << cols << endl;
    input_image >> buffer;
    return;
}

int ***generate_image(int rows, int cols)
{
    int ***image = new int **[rows];
    for (int i = 0; i < rows; i++)
    {
        image[i] = new int *[cols];
        for (int j = 0; j < cols; j++)
        {
            image[i][j] = new int[3];
            for (int k = 0; k < 3; k++)
            {
                image[i][j][k] = 0;
            }
        }
    }
    return image;
}

int ***read_image(int rows, int cols, ifstream &input_image, string &buffer)
{
    int ***image = new int **[rows];
    for (int i = 0; i < rows; i++)
    {
        image[i] = new int *[cols];
        for (int j = 0; j < cols; j++)
        {
            image[i][j] = new int[3];
            for (int k = 0; k < 3; k++)
            {
                input_image >> buffer;
                image[i][j][k] = stoi(buffer);
            }
        }
    }
    return image;
}

void grayscale_transformation(int ***original_image, int pipefd, int rows, int cols)
{
    int temp = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // NTSC (National Television Standards Committee) Formula
            temp = (int)(0.299 * original_image[i][j][0] + 0.587 * original_image[i][j][1] + 0.114 * original_image[i][j][2]);
            write_shared_pipe(pipefd, temp);
        }
    }
    return;
}

void flipping_transformation(int ***modified_image, int pipefd, int rows, int cols)
{
    int temp = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            temp = read_shared_pipe(pipefd);
            for (int k = 0; k < 3; k++)
            {
                modified_image[i][cols - j - 1][k] = temp;
            }
        }
    }
    return;
};

void write_image(int ***image, ofstream &output_image, int rows, int cols)
{
    output_image << "P3\n";
    output_image << cols << " " << rows << "\n";
    output_image << "255\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols - 1; j++)
        {
            output_image << image[i][j][0] << " " << image[i][j][1] << " " << image[i][j][2] << " ";
        }
        output_image << image[i][cols - 1][0] << " " << image[i][cols - 1][1] << " " << image[i][cols - 1][2] << endl;
    }

    return;
}

void delete_image(int ***image, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            delete[] image[i][j];
        }
        delete[] image[i];
    }
    delete[] image;
    return;
}

void write_shared_pipe(int pipefd, int temp)
{
    unsigned char buffer = temp;
    write(pipefd, &buffer, sizeof(char));
    return;
}

int read_shared_pipe(int pipefd)
{
    unsigned char buffer;
    while (read(pipefd, &buffer, sizeof(buffer)) <= 0)
        ;
    return (int)buffer;
}
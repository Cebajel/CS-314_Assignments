#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>

using namespace std;

void get_rows_cols(ifstream &input_image, string &buffer, int &rows, int &cols);
int ***generate_image(int rows, int cols);
int ***read_image(int rows, int cols, ifstream &input_image, string &buffer);
void grayscale_transformation(int ***original_image, int ***modified_image, int rows, int cols);
void flipping_transformation(int ***original_image, int ***modified_image, int rows, int cols);
void read_shared_memory(char *shm, int ***image, int rows, int cols);
void write_shared_memory(char *shm, int ***image, int rows, int cols);
void write_image(int ***image, ofstream &output_image, int rows, int cols);
void delete_image(int ***image, int rows, int cols);

sem_t *s = sem_open("/semaphore", O_CREAT, 0666, 0);

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
    int ***modified_image_1 = generate_image(rows, cols);
    int ***modified_image_2 = generate_image(rows, cols);

    key_t key = 1234;
    // shmget returns an identifier in shmid
    int shmid = shmget(key, rows * cols * 3 * 4, 0666 | IPC_CREAT);
    if (shmid < 0)
    {
        cout << "Shared Memory ERROR!\n";
        exit(EXIT_FAILURE);
    }

    char *shm = (char *)shmat(shmid, NULL, 0);

    int pid = fork();

    if (pid == 0)
    {
        // Parent Process
        sem_wait(s);
        read_shared_memory(shm, modified_image_1, rows, cols);
        flipping_transformation(modified_image_1, modified_image_2, rows, cols);
    }
    else
    {
        // Child Process
        grayscale_transformation(image, modified_image_1, rows, cols);
        write_shared_memory(shm, modified_image_1, rows, cols);
        sem_post(s);
        sem_close(s);
        delete_image(image, rows, cols);
        delete_image(modified_image_1, rows, cols);
        delete_image(modified_image_2, rows, cols);
        sem_unlink("/semaphore");
        // detach from shared memory
        shmdt(shm);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);

    // Converting the final image to ppm format
    write_image(modified_image_2, output_image, rows, cols);

    // Deallocating memory assigned to all the image matrices
    delete_image(image, rows, cols);
    delete_image(modified_image_1, rows, cols);
    delete_image(modified_image_2, rows, cols);
    sem_close(s);
    sem_unlink("/semaphore");

    // detach from shared memory
    shmdt(shm);
    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);

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

void grayscale_transformation(int ***original_image, int ***modified_image, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // NTSC (National Television Standards Committee) Formula
            modified_image[i][j][0] = (int)(0.299 * original_image[i][j][0] + 0.587 * original_image[i][j][1] + 0.114 * original_image[i][j][2]);
            modified_image[i][j][1] = modified_image[i][j][0];
            modified_image[i][j][2] = modified_image[i][j][0];
        }
    }
    return;
}

void flipping_transformation(int ***original_image, int ***modified_image, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                modified_image[i][j][k] = original_image[i][cols - j - 1][k];
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

void write_shared_memory(char *shm, int ***image, int rows, int cols)
{
    char *str = shm + 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < 3; k++, str++)
            {
                *str = image[i][j][k];
            }
        }
    }
    *str = 0;
    return;
}

void read_shared_memory(char *shm, int ***image, int rows, int cols)
{
    char *str = shm + 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < 3; k++, str++)
            {
                image[i][j][k] = *str;
            }
        }
    }
    return;
}
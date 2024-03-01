#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <pthread.h>
#include <atomic>

using namespace std;

typedef struct arg_struct
{
    int ***original_image;
    volatile int ***modified_image_1;
    volatile int ***modified_image_2;
    int rows;
    int cols;
} ARGS;

void get_rows_cols(ifstream &input_image, string &buffer, int &rows, int &cols);
volatile int ***generate_image(int rows, int cols);
int ***read_image(int rows, int cols, ifstream &input_image, string &buffer);
void *grayscale_transformation(void *arguments);
void *flipping_transformation(void *arguments);
void write_image(volatile int ***image, ofstream &output_image, int rows, int cols);
void delete_image(volatile int ***image, int rows, int cols);
void delete_image_non_volatile(int ***image, int rows, int cols);
void initialize_lock(int rows, int cols);
void clear_lock(int rows, int cols);

//  = ATOMIC_FLAG_INIT
atomic_flag **my_lock;

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
    pthread_t t1, t2;
    ARGS args;

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
    initialize_lock(rows, cols);
    // Converting ppm image to an 3D array
    int ***image = read_image(rows, cols, input_image, buffer);
    // Creating two 2D array representing two independent graypoint images
    volatile int ***modified_image_1 = generate_image(rows, cols);
    volatile int ***modified_image_2 = generate_image(rows, cols);

    args.original_image = image;
    args.modified_image_1 = modified_image_1;
    args.modified_image_2 = modified_image_2;
    args.rows = rows;
    args.cols = cols;

    pthread_create(&t1, NULL, &grayscale_transformation, (void *)&args);
    pthread_create(&t2, NULL, &flipping_transformation, (void *)&args);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Converting the final image to ppm format
    write_image(modified_image_2, output_image, rows, cols);

    // Deallocating memory assigned to all the image matrices
    delete_image_non_volatile(image, rows, cols);
    delete_image(modified_image_1, rows, cols);
    delete_image(modified_image_2, rows, cols);

    // Closing file decriptors
    input_image.close();
    output_image.close();
    clear_lock(rows, cols);

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

volatile int ***generate_image(int rows, int cols)
{
    volatile int ***image = new volatile int **[rows];
    for (int i = 0; i < rows; i++)
    {
        image[i] = new volatile int *[cols];
        for (int j = 0; j < cols; j++)
        {
            image[i][j] = new volatile int[3];
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

void *grayscale_transformation(void *arguments)
{
    ARGS *args = (ARGS *)arguments;
    int rows = args->rows;
    int cols = args->cols;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // NTSC (National Television Standards Committee) Formula
            args->modified_image_1[i][j][0] = (int)(0.299 * args->original_image[i][j][0] + 0.587 * args->original_image[i][j][1] + 0.114 * args->original_image[i][j][2]);
            args->modified_image_1[i][j][1] = args->modified_image_1[i][j][0];
            args->modified_image_1[i][j][2] = args->modified_image_1[i][j][0];
            my_lock[i][j].clear();
        }
    }
    return NULL;
}

void *flipping_transformation(void *arguments)
{
    ARGS *args = (ARGS *)arguments;
    int rows = args->rows;
    int cols = args->cols;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            while (my_lock[i][j].test_and_set());
            for (int k = 0; k < 3; k++)
            {
                args->modified_image_2[i][cols-j-1][k] = args->modified_image_1[i][j][k];
            }
            my_lock[i][j].clear();
        }
    }
    return NULL;
};

void write_image(volatile int ***image, ofstream &output_image, int rows, int cols)
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

void delete_image(volatile int ***image, int rows, int cols)
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

void initialize_lock(int rows, int cols)
{
    my_lock = new atomic_flag *[rows];
    for (int i = 0; i < rows; i++)
    {
        my_lock[i] = new atomic_flag[cols];
        for (int j = 0; j < cols; j++)
        {
            my_lock[i][j].clear();
            while(my_lock[i][j].test_and_set());
        }
    }
    return;
}

void clear_lock(int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            my_lock[i][j].clear();
        }
    }
    return;
}

void delete_image_non_volatile(int ***image, int rows, int cols)
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
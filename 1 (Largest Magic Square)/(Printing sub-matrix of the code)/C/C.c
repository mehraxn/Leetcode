#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to print a matrix
void print_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to generate a random matrix of given size (rows x cols)
int** generate_random_matrix(int rows, int cols) {
    int **matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 10;  // Random values between 0 and 9
        }
    }
    return matrix;
}

// Function to print all square sub-matrices with sizes 2x2 to 6x6
void print_all_square_matrices(int **matrix, int rows, int cols) {
    // Iterate over all possible square sub-matrices sizes from 2x2 to 6x6
    for (int size = 2; size <= 6; size++) {
        if (size > rows || size > cols) continue;  // Skip if size exceeds matrix dimensions
        for (int i = 0; i <= rows - size; i++) {
            for (int j = 0; j <= cols - size; j++) {
                // Extract and print the sub-matrix of the current size
                printf("Square Sub-Matrix of size %d starting at (%d, %d):\n", size, i, j);
                for (int x = 0; x < size; x++) {
                    for (int y = 0; y < size; y++) {
                        printf("%d ", matrix[i + x][j + y]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        }
    }
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Define the matrix size
    int rows = 6, cols = 8;

    // Generate a random matrix of size 6x8
    int **matrix = generate_random_matrix(rows, cols);

    // Print the generated matrix
    printf("Generated Random Matrix (6x8):\n");
    print_matrix(matrix, rows, cols);

    // Print all square sub-matrices with sizes 2x2 to 6x6
    print_all_square_matrices(matrix, rows, cols);

    // Free the allocated memory for the matrix
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}

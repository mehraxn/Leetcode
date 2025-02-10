#include <stdio.h>

#define MAX_SIZE 10  // Maximum matrix size (for simplicity)

void multiply_matrices(int m1[][MAX_SIZE], int m2[][MAX_SIZE], int m[][MAX_SIZE], int r1, int c1, int c2) {
    // Initialize result matrix to 0
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            m[i][j] = 0;  // Initialize result matrix with zeros
        }
    }

    // Perform matrix multiplication
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            for (int k = 0; k < c1; k++) {
                m[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

void print_matrix(int m[][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int r1, c1, r2, c2;

    // Input dimensions for matrix m1
    printf("Enter number of rows and columns for matrix 1: ");
    scanf("%d %d", &r1, &c1);

    // Input dimensions for matrix m2
    printf("Enter number of rows and columns for matrix 2: ");
    scanf("%d %d", &r2, &c2);

    // Check if multiplication is possible
    if (c1 != r2) {
        printf("Matrix multiplication is not possible. The number of columns of matrix 1 must be equal to the number of rows of matrix 2.\n");
        return 0;
    }

    // Declare the matrices
    int m1[MAX_SIZE][MAX_SIZE], m2[MAX_SIZE][MAX_SIZE], m[MAX_SIZE][MAX_SIZE];

    // Input elements for matrix m1
    printf("Enter elements of matrix 1:\n");
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c1; j++) {
            scanf("%d", &m1[i][j]);
        }
    }

    // Input elements for matrix m2
    printf("Enter elements of matrix 2:\n");
    for (int i = 0; i < r2; i++) {
        for (int j = 0; j < c2; j++) {
            scanf("%d", &m2[i][j]);
        }
    }

    // Call matrix multiplication function
    multiply_matrices(m1, m2, m, r1, c1, c2);

    // Print the result matrix
    printf("Resultant Matrix (m1 * m2):\n");
    print_matrix(m, r1, c2);

    return 0;
}

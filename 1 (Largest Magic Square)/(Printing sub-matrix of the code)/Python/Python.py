import random

# Function to print the matrix
def print_matrix(matrix):
    for row in matrix:
        print(' '.join(map(str, row)))

# Function to generate a random matrix with given dimensions
def generate_random_matrix(rows, cols):
    matrix = []
    for i in range(rows):
        row = [random.randint(0, 9) for _ in range(cols)]  # Random values between 0 and 9
        matrix.append(row)
    return matrix

# Function to print all square sub-matrices from size 2x2 to 6x6
def print_all_square_matrices(matrix):
    rows = len(matrix)
    cols = len(matrix[0])

    # Iterate over all square sizes (2x2, 3x3, ..., up to the smallest dimension of the matrix)
    for size in range(2, min(rows, cols) + 1):  # From 2 to the minimum of rows or cols
        for i in range(rows - size + 1):
            for j in range(cols - size + 1):
                print(f"Square Sub-Matrix of size {size} starting at ({i}, {j}):")
                # Print the sub-matrix of current size
                for x in range(size):
                    for y in range(size):
                        print(matrix[i + x][j + y], end=" ")
                    print()  # New line for each row of the sub-matrix
                print()  # Add a blank line between different sub-matrices

# Main function
def main():
    rows, cols = 6, 8  # Matrix size: 6x8

    # Generate a random 6x8 matrix
    matrix = generate_random_matrix(rows, cols)

    # Print the generated matrix
    print("Generated Random Matrix (6x8):")
    print_matrix(matrix)
    print()

    # Print all square sub-matrices from size 2x2 to 6x6
    print_all_square_matrices(matrix)

# Run the main function
if __name__ == "__main__":
    main()

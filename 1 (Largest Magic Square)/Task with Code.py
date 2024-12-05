from typing import List

"""
README: Finding the Largest Magic Square in a Grid

Task Description:
The goal is to find the largest magic square in a given m x n grid of integers. A magic square is a k x k sub-grid where:
1. The sum of all rows is equal.
2. The sum of all columns is equal.
3. The sum of the main diagonal (top-left to bottom-right) is equal to the row and column sums.
4. The sum of the secondary diagonal (top-right to bottom-left) is also equal to the row and column sums.

Every 1 x 1 grid is trivially a magic square.

Input:
- A 2D list `grid` of integers representing the m x n grid.

Output:
- An integer representing the size (side length k) of the largest magic square found.

Solution:
1. Checking Functionality:
   - The `checking` method verifies if a given k x k sub-matrix satisfies the conditions of a magic square.
   - It checks if:
     - Row sums are equal.
     - Column sums are equal.
     - Both diagonal sums are equal.
   - Returns the size of the square if valid, or 0 otherwise.

2. Iterative Search for Largest Magic Square:
   - The `largestMagicSquare` method starts by checking for the largest possible square (min(rows, cols)) and decreases the size.
   - For each size, it extracts all possible k x k sub-matrices and uses the `checking` method to validate them.
   - Returns the size of the first valid magic square found (the largest, due to iteration order).

Example:
Input:
grid = [
    [7, 1, 4, 5, 6],
    [2, 5, 1, 6, 4],
    [1, 5, 4, 3, 2],
    [1, 2, 7, 3, 4],
    [4, 6, 5, 7, 8]
]

Output:
5 (the largest magic square is the entire grid).

"""
from typing import List

class Solution:
    # This method checks if the given input matrix is a magic square.
    def checking(self, input_matrix: List[List[int]]) -> int:
        # Get the number of rows and columns in the input matrix.
        row = len(input_matrix)
        column = len(input_matrix[0])
        
        # A magic square must be a square matrix; return 0 if it's not square.
        if row != column:
            return 0

        # Calculate the sum of the first row to use as the target sum (scale).
        scale = sum(input_matrix[0])
        
        # Check if all rows have the same sum as the first row.
        for i in range(1, row):
            if sum(input_matrix[i]) != scale:
                return 0  # If any row sum differs, it's not a magic square.

        # Variable to track the sum of the main diagonal.
        temp_diag = 0

        # Iterate through the rows to calculate column sums and main diagonal sum.
        for i in range(row):
            temp = 0  # Temporary variable to hold the column sum for column i.
            for j in range(row):
                temp += input_matrix[j][i]  # Add the element in column i for the current row.
                if i == j:  # If the element is on the main diagonal (row index == column index).
                    temp_diag += input_matrix[i][j]
            if temp != scale:  # Check if the column sum matches the target sum.
                return 0

        # Check if the main diagonal sum matches the target sum.
        if temp_diag != scale:
            return 0

        # Variable to track the sum of the secondary diagonal.
        temp_second = 0

        # Iterate through the matrix to calculate the secondary diagonal sum.
        for i in range(row):
            for j in range(row):
                # Check if the element is on the secondary diagonal (i + j = row - 1).
                if i + j == row - 1:
                    temp_second += input_matrix[i][j]
        
        # Check if the secondary diagonal sum matches the target sum.
        if temp_second != scale:
            return 0

        # If all checks pass, return the size of the square matrix.
        return row

    # This method finds the largest magic square in the given grid.
    def largestMagicSquare(self, grid: List[List[int]]) -> int:
        # Get the number of rows and columns in the grid.
        rows = len(grid)
        cols = len(grid[0])

        # The maximum size of a possible square is the smaller of rows or columns.
        max_square_size = min(rows, cols)
        
        # Start checking for magic squares from the largest possible size and decrease.
        for size in range(max_square_size, 0, -1):
            # Iterate over all possible top-left corners of size x size sub-matrices.
            for i in range(rows - size + 1):
                for j in range(cols - size + 1):
                    # Initialize an empty sub-matrix.
                    sub_matrix = []
                    # Construct the sub-matrix row by row.
                    for k in range(i, i + size):
                        sub_matrix.append(grid[k][j:j + size])
                    
                    # Use the checking method to validate if this sub-matrix is a magic square.
                    number = self.checking(sub_matrix)
                    
                    # If a magic square is found, return its size.
                    if number > 0:
                        return size

        
        # If no magic square is found, return 1 (the smallest possible square size).
        return 1

#include <stdio.h>
#include <string.h>

// Declare the dfs function which will be used to mark connected land cells.
void dfs(char **grid, int gridRow, int gridCol, int row, int col);

// Function to count the number of islands in the grid.
int numIslands(char** grid, int gridSize, int* gridColSize) {
    int count = 0;                  // Initialize the island counter.
    int i, j;                       // Variables for iterating over the grid rows and columns.

    for (i = 0; i < gridSize; i++) {                // Loop over each row in the grid.
        for (j = 0; j < *gridColSize; j++) {        // Loop over each column in the current row.
            if (grid[i][j] == '0')                  // If the current cell is water ('0'), skip it.
                continue;                           // Continue to the next cell.
            else if (grid[i][j] == '1') {             // If the current cell is land ('1'):
                dfs(grid, gridSize, *gridColSize, i, j);  // Call dfs to mark the entire connected island.
                count++;                            // Increment the island counter.
            }
        }
    }
    return count;                                   // Return the total number of islands found.
}

/*
Function: numIslands
--------------------
This function iterates through every cell in the provided 2D binary grid.
If a cell contains land ('1'), it calls the dfs function to explore and mark all connected land cells.
Each exploration corresponds to one island, so the island counter is incremented.
Finally, the function returns the count of islands discovered in the grid.
*/

// Depth-first search function to mark all connected land cells as visited.
void dfs(char **grid, int gridRow, int gridCol, int row, int col) {
    if (row < 0 || row >= gridRow || col < 0 || col >= gridCol || grid[row][col] == '0')
        // If the current cell is out of bounds or is water ('0'), return immediately.
        return;

    grid[row][col] = '0';   // Mark the current land cell as visited by setting it to water ('0').

    dfs(grid, gridRow, gridCol, row - 1, col);  // Recursively visit the cell above.
    dfs(grid, gridRow, gridCol, row + 1, col);  // Recursively visit the cell below.
    dfs(grid, gridRow, gridCol, row, col - 1);  // Recursively visit the cell to the left.
    dfs(grid, gridRow, gridCol, row, col + 1);  // Recursively visit the cell to the right.
}

/*
Function: dfs
-------------
This function performs a depth-first search starting from the specified cell (row, col) in the grid.
It checks if the current cell is within bounds and is land ('1'). If not, the function returns immediately.
If the cell is valid land, it marks the cell as visited by changing its value to '0'.
Then, it recursively calls itself for the four adjacent cells (up, down, left, right),
ensuring that all connected land cells are visited and marked.
This process effectively "sinks" the island, preventing it from being counted multiple times.

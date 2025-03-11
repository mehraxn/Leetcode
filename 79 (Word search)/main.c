#include <stdbool.h>
#include <string.h>

// Helper DFS function
static bool dfs(char** board, int boardSize, int* boardColSize,
                char* word, int row, int col, int index)
{
    // If we've matched all characters, return true
    if (word[index] == '\0') {
        return true;
    }

    // Boundary checks and current-character match check
    if (row < 0 || row >= boardSize ||
        col < 0 || col >= boardColSize[row] ||
        board[row][col] != word[index])
    {
        return false;
    }

    // Temporarily mark the cell as visited
    char temp = board[row][col];
    board[row][col] = '#';

    // Explore 4 directions for the next character
    bool found = dfs(board, boardSize, boardColSize, word, row + 1, col,     index + 1) ||
                 dfs(board, boardSize, boardColSize, word, row - 1, col,     index + 1) ||
                 dfs(board, boardSize, boardColSize, word, row,     col + 1, index + 1) ||
                 dfs(board, boardSize, boardColSize, word, row,     col - 1, index + 1);

    // Restore the cell (backtrack)
    board[row][col] = temp;

    return found;
}

bool exist(char** board, int boardSize, int* boardColSize, char* word) {
    // Edge cases
    if (!board || boardSize == 0 || !word || word[0] == '\0') {
        return false;
    }

    // Try each cell as a starting point
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardColSize[i]; j++) {
            if (dfs(board, boardSize, boardColSize, word, i, j, 0)) {
                return true;
            }
        }
    }

    return false;
}

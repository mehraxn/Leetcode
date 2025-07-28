#include <stdlib.h>

int dr[4] = {1, -1, 0, 0};
int dc[4] = {0, 0, 1, -1};

int rows, cols;
int **visited;

int dfs(int** matrix, int i, int j) {
    if (visited[i][j] != 0)
        return visited[i][j];
    int best = 1;
    for (int k = 0; k < 4; ++k) {
        int ni = i + dr[k];
        int nj = j + dc[k];
        if (ni >= 0 && ni < rows &&
            nj >= 0 && nj < cols &&
            matrix[ni][nj] > matrix[i][j]) {
            int len = 1 + dfs(matrix, ni, nj);
            if (len > best) best = len;
            }
    }
    visited[i][j] = best;
    return best;
}

int longestIncreasingPath(int** matrix, int matrixSize, int* matrixColSize) {
    rows = matrixSize;
    if (rows == 0) return 0;
    cols = matrixColSize[0];
    visited = malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i)
        visited[i] = calloc(cols, sizeof(int));
    int answer = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int len = dfs(matrix, i, j);
            if (len > answer) answer = len;
        }
    }
    for (int i = 0; i < rows; ++i) free(visited[i]);
    free(visited);
    return answer;
}

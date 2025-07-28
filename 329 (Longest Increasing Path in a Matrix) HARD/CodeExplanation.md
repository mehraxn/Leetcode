# Longest Increasing Path in a Matrix

## Algorithm Overview

We solve the problem of finding the length of the longest strictly increasing path in an `m × n` integer matrix using **Depth‑First Search (DFS) with memoization**. Starting from each cell, we explore all four cardinal directions—up, down, left, right—to extend our path, but we cache intermediate results so that each cell is only processed once. This reduces the overall time complexity to **O(mn)**.

## Key Components Explained

### Direction Offsets (`dr` and `dc`)

```c
int dr[4] = { 1, -1,  0,  0 };
int dc[4] = { 0,  0,  1, -1 };
```

* `dr[k]` and `dc[k]` together represent the row and column change for the \_k\_th move:

  1. Down  `(+1,  0)`
  2. Up    `(-1,  0)`
  3. Right `( 0, +1)`
  4. Left  `( 0, -1)`
* Using two small arrays lets us loop over all four directions concisely.

### Global State (`rows`, `cols`, `visited`)

```c
int rows, cols;
int **visited;
```

* `rows` and `cols` store the matrix dimensions for quick boundary checks.
* `visited` is a 2D memo table: `visited[i][j]` holds the length of the longest increasing path *starting* at cell `(i,j)`. 0 means “not yet computed.”

### Depth‑First Search Function

```c
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
            if (len > best)
                best = len;
        }
    }

    visited[i][j] = best;
    return best;
}
```

* **Memo check**: Returns immediately if we've already computed `visited[i][j]`.
* **Base path length**: `best = 1` covers the case with no valid moves—at minimum the path includes the cell itself.
* **Recursion**: For each neighbor that is strictly larger, call `dfs` to get that neighbor’s longest path length, then add 1.
* **Memoization store**: Saves `best` so subsequent calls skip re‑exploration.

### Main Entry Point

```c
int longestIncreasingPath(int** matrix,
                         int matrixSize,
                         int* matrixColSize) {
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
            if (len > answer)
                answer = len;
        }
    }

    for (int i = 0; i < rows; ++i)
        free(visited[i]);
    free(visited);

    return answer;
}
```

1. **Initialization**: Set `rows`/`cols`, allocate and zero‑initialize `visited`.
2. **Computation**: Launch `dfs` from every cell, keeping track of the global maximum path length (`answer`).
3. **Cleanup**: Free all allocated memory to avoid leaks.

## Recursion & Memoization Insights

* **Recursion** naturally follows the fact that the problem of finding the longest path from `(i,j)` depends on the same problem from its neighbors `(ni,nj)` when `matrix[ni][nj] > matrix[i][j]`.
* **Memoization** ensures each cell’s DFS runs only once. Without it, overlapping subproblems would lead to exponential time.

## Why These Variable Choices?

* **`visited` vs. `dp`**: The name `visited` indicates both that we’ve “hit” the cell and recorded its result.
* **`best = 1` start**: Guarantees every cell contributes at least itself to any path.
* **Globals for dimensions**: Simplify boundary logic without passing `rows`/`cols` down every recursive call.

With these pieces working together—systematic exploration balanced by aggressive caching—the code computes the longest increasing path in linear time relative to the number of cells.

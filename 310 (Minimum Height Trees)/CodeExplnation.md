# Explanation of C Code for Leetcode 310: Minimum Height Trees

```c
int* findMinHeightTrees(int n, int** edges, int edgesSize, int* edgesColSize, int* returnSize) {
```

* **Function signature**: This function takes the number of nodes `n`, the edge list `edges`, the number of edges `edgesSize`, an array `edgesColSize` (not used in logic but needed for LeetCode signature), and a pointer to `returnSize`.
* **Returns**: A dynamically allocated array of node indices that are roots of Minimum Height Trees (MHTs).

```c
    if (n == 1) {
        int* r = malloc(sizeof(int));
        r[0] = 0;
        *returnSize = 1;
        return r;
    }
```

* **Edge case**: If the tree has only one node, it's trivially the MHT root.
* Allocates space for one integer, sets it to 0, and returns.

```c
    int* d = calloc(n, sizeof(int));
```

* `d` stores the **degree** (number of neighbors) of each node. Initialized to 0.

```c
    int* a = malloc(edgesSize * 2 * sizeof(int));
    int* h = malloc(n * sizeof(int));
    int* x = malloc(edgesSize * 2 * sizeof(int));
```

* `a` and `x` build the **adjacency list** using a custom format:

  * `a` stores destination node ids for edges.
  * `x` stores the previous index in the linked list of neighbors.
  * `h[i]` will store the **head** of the adjacency list for node `i`. Initialized to -1 next.

```c
    for (int i = 0; i < n; i++) h[i] = -1;
```

* Initializes all heads to -1, indicating empty lists.

```c
    int p = 0;
    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0], v = edges[i][1];
        a[p] = v; x[p] = h[u]; h[u] = p++;
        a[p] = u; x[p] = h[v]; h[v] = p++;
        d[u]++; d[v]++;
    }
```

* This loop builds the graph:

  * For each undirected edge `u-v`, adds both `v` to `u`'s list and `u` to `v`'s list.
  * `a[p] = v` assigns neighbor.
  * `x[p] = h[u]` links to the previous head.
  * `h[u] = p++` updates the head to new index.
  * Degree array `d` is incremented accordingly.

```c
    int* q = malloc(n * sizeof(int));
    int f = 0, b = 0;
```

* `q` is a queue for processing leaves in BFS style.
* `f` and `b` are front and back pointers.

```c
    for (int i = 0; i < n; i++) if (d[i] == 1) q[b++] = i;
```

* Adds all **leaves** (degree 1 nodes) to the queue.

```c
    int r = n;
    while (r > 2) {
        int s = b - f;
        r -= s;
```

* Performs iterative leaf trimming:

  * Each layer of leaves is removed.
  * `s` is the number of current leaves.
  * Subtracts `s` from total node count `r`.

```c
        while (s--) {
            int z = q[f++];
            for (int j = h[z]; j != -1; j = x[j]) {
                int y = a[j];
                if (--d[y] == 1) q[b++] = y;
            }
        }
    }
```

* For each leaf `z`:

  * Iterates over its neighbors `y` using adjacency list.
  * Decrements neighbor's degree.
  * If it becomes a leaf, add to queue.
* Repeats until <= 2 nodes left (these are MHT roots).

```c
    int t = b - f;
    int* o = malloc(t * sizeof(int));
    for (int i = 0; i < t; i++) o[i] = q[f + i];
    *returnSize = t;
```

* `t` is the number of nodes remaining (1 or 2).
* These are copied into result array `o`.
* `returnSize` is updated accordingly.

```c
    free(d); free(a); free(h); free(x); free(q);
    return o;
}
```

* Frees all dynamically allocated memory used for processing.
* Returns the result array `o`.

---

### Key Takeaways

* Uses **topological trimming** of leaves to find MHT roots efficiently.
* Space-optimized adjacency list.
* Time complexity is **O(n)**, ideal for large `n <= 2 * 10^4`.

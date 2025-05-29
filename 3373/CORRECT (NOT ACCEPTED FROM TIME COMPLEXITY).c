#include <stdlib.h>
#include <string.h>

typedef struct {
    int node;
    int parity;
} QueueNode;

void bfs_parity(int** edges, int edgesSize, int* edgesColSize, int n, int start, int* parities) {
    int* visited = (int*)malloc(n * sizeof(int));
    memset(visited, 0, n * sizeof(int));
    QueueNode* queue = (QueueNode*)malloc(n * sizeof(QueueNode));
    int front = 0, rear = 0;

    queue[rear++] = (QueueNode){start, 0};
    visited[start] = 1;

    while (front < rear) {
        QueueNode current = queue[front++];
        parities[current.node] = current.parity;

        for (int i = 0; i < edgesSize; i++) {
            int u = edges[i][0];
            int v = edges[i][1];
            if (u == current.node && !visited[v]) {
                visited[v] = 1;
                queue[rear++] = (QueueNode){v, current.parity ^ 1};
            } else if (v == current.node && !visited[u]) {
                visited[u] = 1;
                queue[rear++] = (QueueNode){u, current.parity ^ 1};
            }
        }
    }

    free(visited);
    free(queue);
}

int* maxTargetNodes(int** edges1, int edges1Size, int* edges1ColSize, int** edges2, int edges2Size, int* edges2ColSize, int* returnSize) {
    int n = edges1Size + 1;
    int m = edges2Size + 1;
    *returnSize = n;
    int* answer = (int*)malloc(n * sizeof(int));

    int* tree1_evens = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int* parities = (int*)malloc(n * sizeof(int));
        bfs_parity(edges1, edges1Size, edges1ColSize, n, i, parities);
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (parities[j] == 0) {
                count++;
            }
        }
        tree1_evens[i] = count;
        free(parities);
    }

    int* tree2_odds = (int*)malloc(m * sizeof(int));
    for (int j = 0; j < m; j++) {
        int* parities = (int*)malloc(m * sizeof(int));
        bfs_parity(edges2, edges2Size, edges2ColSize, m, j, parities);
        int count = 0;
        for (int k = 0; k < m; k++) {
            if (parities[k] == 1) {
                count++;
            }
        }
        tree2_odds[j] = count;
        free(parities);
    }

    int max_tree2_odd = 0;
    for (int j = 0; j < m; j++) {
        if (tree2_odds[j] > max_tree2_odd) {
            max_tree2_odd = tree2_odds[j];
        }
    }

    for (int i = 0; i < n; i++) {
        answer[i] = tree1_evens[i] + max_tree2_odd;
    }

    free(tree1_evens);
    free(tree2_odds);

    return answer;
}
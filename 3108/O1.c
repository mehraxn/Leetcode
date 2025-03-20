#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/*
   We'll store the graph as an adjacency list:
   adj[v] = list of (neighbor, weight).
*/
typedef struct {
    int to;
    int w;
} Edge;

typedef struct {
    Edge* data;
    int size;
    int capacity;
} Vec;

void vecInit(Vec* vec, int cap) {
    vec->data = (Edge*)malloc(cap * sizeof(Edge));
    vec->size = 0;
    vec->capacity = cap;
}
void vecPush(Vec* vec, int to, int w) {
    if (vec->size == vec->capacity) {
        vec->capacity *= 2;
        vec->data = (Edge*)realloc(vec->data, vec->capacity * sizeof(Edge));
    }
    vec->data[vec->size].to = to;
    vec->data[vec->size].w = w;
    vec->size++;
}

/* Min-heap for (cost, node). */
typedef struct {
    int cost;
    int node;
} HeapItem;

void swap(HeapItem* a, HeapItem* b) {
    HeapItem tmp = *a;
    *a = *b;
    *b = tmp;
}

typedef struct {
    HeapItem* arr;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int cap) {
    MinHeap* h = (MinHeap*)malloc(sizeof(MinHeap));
    h->arr = (HeapItem*)malloc(cap * sizeof(HeapItem));
    h->size = 0;
    h->capacity = cap;
    return h;
}

void heapPush(MinHeap* h, int cost, int node) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->arr = (HeapItem*)realloc(h->arr, h->capacity * sizeof(HeapItem));
    }
    int idx = h->size++;
    h->arr[idx].cost = cost;
    h->arr[idx].node = node;
    // bubble up
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (h->arr[parent].cost <= h->arr[idx].cost) break;
        swap(&h->arr[parent], &h->arr[idx]);
        idx = parent;
    }
}

HeapItem heapPop(MinHeap* h) {
    HeapItem ret = h->arr[0];
    h->size--;
    if (h->size > 0) {
        h->arr[0] = h->arr[h->size];
        // bubble down
        int idx = 0;
        while (true) {
            int left = 2*idx + 1, right = 2*idx + 2;
            int smallest = idx;
            if (left < h->size && h->arr[left].cost < h->arr[smallest].cost) {
                smallest = left;
            }
            if (right < h->size && h->arr[right].cost < h->arr[smallest].cost) {
                smallest = right;
            }
            if (smallest == idx) break;
            swap(&h->arr[idx], &h->arr[smallest]);
            idx = smallest;
        }
    }
    return ret;
}

bool heapEmpty(MinHeap* h) {
    return (h->size == 0);
}

/**
 * n: number of vertices
 * edges: array of [u, v, w]
 * edgesSize: number of edges
 * query: array of [s, t]
 * querySize: number of queries
 *
 * Return array must be malloced; caller will free.
 */
int* minimumCost(int n, int** edges, int edgesSize,
                 int* edgesColSize,
                 int** query, int querySize,
                 int* queryColSize,
                 int* returnSize)
{
    // Build adjacency list.
    // First allocate an array of Vec (one Vec per vertex).
    Vec* adj = (Vec*)malloc(n * sizeof(Vec));
    for(int i=0; i<n; i++){
        vecInit(&adj[i], 2);
    }
    for(int i=0; i<edgesSize; i++){
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];
        vecPush(&adj[u], v, w);
        vecPush(&adj[v], u, w);  // undirected
    }

    // We'll answer each query by running a D'Johnson-like
    // bitwise-AND "Dijkstra" from s to t.
    int* ans = (int*)malloc(querySize * sizeof(int));
    *returnSize = querySize;

    for(int q=0; q<querySize; q++){
        int s = query[q][0];
        int t = query[q][1];

        // dist[v] = smallest (integer) cost we can achieve to reach v
        // We'll store -1 if not visited yet; otherwise the actual cost
        int* dist = (int*)malloc(n*sizeof(int));
        for(int i=0; i<n; i++){
            dist[i] = -1;
        }

        // Use a min-heap keyed by the *numerical value* of the AND-cost
        MinHeap* heap = createMinHeap(n);

        // Start with cost = all bits set
        const int INF_COST = 0x7fffffff; // or 0xffffffff if you prefer
        const int START_COST = 0xffffffff;

        dist[s] = START_COST;
        heapPush(heap, dist[s], s);

        while(!heapEmpty(heap)) {
            HeapItem top = heapPop(heap);
            int curCost = top.cost;
            int node = top.node;

            // If this is stale (we have found a smaller cost for "node") skip
            if (dist[node] != curCost) continue;

            if (node == t) {
                // As soon as we pop t, we know curCost is the minimal AND for s->t
                break;
            }

            // Relax edges
            for(int i=0; i<adj[node].size; i++){
                int nxt = adj[node].data[i].to;
                int w   = adj[node].data[i].w;
                // newCost = current AND w
                int newCost = curCost & w;
                if (dist[nxt] == -1 || newCost < dist[nxt]) {
                    dist[nxt] = newCost;
                    heapPush(heap, newCost, nxt);
                }
            }
        }

        int answer = dist[t]; // might be -1 if never updated
        free(heap->arr);
        free(heap);

        ans[q] = answer;
        free(dist);
    }

    // Free adjacency list
    for(int i=0; i<n; i++){
        free(adj[i].data);
    }
    free(adj);

    return ans;
}

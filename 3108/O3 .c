#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// A structure to store adjacency info
typedef struct {
    int v;       // neighbor
    int weight;  // edge weight
} Edge;

// Min-heap (priority queue) node
typedef struct {
    int node;
    unsigned int cost; // we store bitwise-AND cost as an unsigned int
} PQItem;

// A simple min-heap comparison
int comparePQ(const void *a, const void *b) {
    PQItem *x = (PQItem*)a;
    PQItem *y = (PQItem*)b;
    if (x->cost < y->cost) return -1;
    if (x->cost > y->cost) return 1;
    return 0;
}

// A simple dynamic array-based min-heap for demonstration
// (In production code, you'd use a more efficient data structure.)
typedef struct {
    PQItem *data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createHeap(int capacity) {
    MinHeap* h = (MinHeap*)malloc(sizeof(MinHeap));
    h->data = (PQItem*)malloc(sizeof(PQItem)*capacity);
    h->size = 0;
    h->capacity = capacity;
    return h;
}
void pushHeap(MinHeap* h, PQItem val) {
    // Insert at end, then "bubble up" by re-sorting.
    if (h->size < h->capacity) {
        h->data[h->size++] = val;
        // For simplicity, just qsort after each push
        qsort(h->data, h->size, sizeof(PQItem), comparePQ);
    }
}
PQItem popHeap(MinHeap* h) {
    // Pop from front (smallest), then shift everything
    PQItem top = h->data[0];
    for(int i=1; i<h->size; i++){
        h->data[i-1] = h->data[i];
    }
    h->size--;
    return top;
}
bool emptyHeap(MinHeap* h) {
    return (h->size == 0);
}

// Solve function
int* minimumCost(int n, int** edges, int edgesSize, int* edgesColSize,
                 int** query, int querySize, int* queryColSize, int* returnSize)
{
    *returnSize = querySize;
    int* answer = (int*)malloc(sizeof(int)*querySize);

    // Build adjacency list
    // First, count how many edges per node
    int* deg = (int*)calloc(n, sizeof(int));
    for(int i=0; i<edgesSize; i++){
        int u = edges[i][0];
        int v = edges[i][1];
        deg[u]++;
        deg[v]++;
    }
    // Allocate adjacency
    Edge** graph = (Edge**)malloc(sizeof(Edge*)*n);
    for(int i=0; i<n; i++){
        graph[i] = (Edge*)malloc(sizeof(Edge)*deg[i]);
        deg[i] = 0; // reuse as index
    }
    // Fill adjacency
    for(int i=0; i<edgesSize; i++){
        int u = edges[i][0];
        int v = edges[i][1];
        int w = edges[i][2];
        graph[u][deg[u]].v = v;
        graph[u][deg[u]].weight = w;
        deg[u]++;
        graph[v][deg[v]].v = u;
        graph[v][deg[v]].weight = w;
        deg[v]++;
    }

    // For each query, run the "Dijkstra" that propagates bitwise-AND costs
    for(int i=0; i<querySize; i++){
        int s = query[i][0];
        int t = query[i][1];

        // If s == t, problem statement might define cost = 0 or something.
        // We'll assume cost=0 if no edges are used:
        if(s == t) {
            answer[i] = 0;
            continue;
        }

        // dist[u] will hold the minimum AND-cost to reach u
        unsigned int* dist = (unsigned int*)malloc(sizeof(unsigned int)*n);
        for(int j=0; j<n; j++){
            dist[j] = UINT_MAX; // 0xFFFFFFFF
        }
        dist[s] = 0xFFFFFFFF;

        // Min-heap
        MinHeap* pq = createHeap(n*2); // rough capacity

        // push start
        PQItem startItem = {s, dist[s]};
        pushHeap(pq, startItem);

        int ans = -1;

        while(!emptyHeap(pq)) {
            PQItem top = popHeap(pq);
            int u = top.node;
            unsigned int costSoFar = top.cost;

            // If we already found a better cost for u, skip
            if(costSoFar > dist[u]) continue;

            // If we reach t, we can stop
            if(u == t) {
                ans = (int)costSoFar;
                break;
            }

            // Relax edges
            for(int eidx=0; eidx<deg[u]; eidx++){
                int v = graph[u][eidx].v;
                unsigned int w = (unsigned int)graph[u][eidx].weight;
                unsigned int newCost = costSoFar & w;
                if(newCost < dist[v]) {
                    dist[v] = newCost;
                    PQItem newItem = {v, newCost};
                    pushHeap(pq, newItem);
                }
            }
        }

        free(pq->data);
        free(pq);
        free(dist);

        answer[i] = ans;
    }

    // Cleanup
    for(int i=0; i<n; i++){
        free(graph[i]);
    }
    free(graph);
    free(deg);

    return answer;
}

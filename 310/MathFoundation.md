# README: Explanation of Graph Theory in `findMinHeightTrees`

## Problem Statement

Given an undirected graph with `n` nodes labeled from `0` to `n-1`, and a list of edges, the goal is to find **all root labels** that result in a **Minimum Height Tree (MHT)**. This involves identifying the **centroid(s)** of the graph.

A **tree** is an undirected graph in which:

* Any two vertices are connected by exactly one path.
* There are no simple cycles.
* It is connected and acyclic.

For a tree with `n` nodes:

* It contains exactly `n - 1` edges.
* The height of a rooted tree is defined as the number of edges on the longest downward path from the root to a leaf.

---

## Mathematical and Graph Theory Background

### 1. **Fundamental Tree Properties**

* A tree is a **minimal connected graph**, meaning if you remove any edge, it becomes disconnected.
* Conversely, it is also a **maximal acyclic graph**, meaning if you add any edge, it creates a cycle.
* A tree has a unique path between any pair of nodes.

### 2. **Height of a Rooted Tree**

* For a chosen root node `r`, the height is defined by the maximum number of edges from `r` to any leaf node.
* Different root nodes can lead to different tree heights.

### 3. **Minimum Height Trees (MHTs)**

* The MHT is a rooted version of the tree such that the **height is minimized**.
* The optimal root(s) that minimize height correspond to the **centroid(s)** of the tree.

### 4. **Centroids of Trees**

* A **centroid** of a tree is a node which, when used as the root, minimizes the maximum depth.
* A tree has:

  * **One centroid** if the total number of nodes `n` is odd.
  * **Two centroids** if `n` is even.
* The centroid(s) lie in the "center" of the tree and balance the longest paths.

### 5. **Mathematical Intuition Behind the Algorithm**

* By removing all **leaf nodes** iteratively (nodes with degree 1), we move closer to the center of the tree.
* This process is akin to peeling the tree layer-by-layer.
* When `1` or `2` nodes remain, they are the centroid(s).
* This is equivalent to finding the **midpoint(s)** of the **longest path** in the tree.

### 6. **Why This Works**

* A tree can be thought of as having a central spine (the longest path, or diameter).
* MHTs occur at the **middle** of this spine.
* By trimming leaves from the outside, we eventually reach this middle.

---

## Algorithm Explanation

### Step 1: Handle Edge Case

* If `n == 1`, return the only node: `{0}`.

### Step 2: Build the Graph

* Use an **adjacency list** to store edges.
* `d[i]` keeps the **degree** (number of connections) of node `i`.

### Step 3: Leaf Pruning (Reverse BFS)

* **Leaves** are nodes with degree 1.
* Start from all leaves and **iteratively remove them** layer-by-layer.
* Each removal reduces the degree of connected nodes.
* If any neighbor becomes a leaf, enqueue it.

### Step 4: Stop When <= 2 Nodes Left

* This process continues until only 1 or 2 nodes remain.
* These remaining nodes are the **centroids**.
* These are the roots that yield MHTs.

---

## Graph Concepts Used

### a. **Adjacency List Construction**

* Efficient for sparse graphs.
* Each node stores linked indices to neighbors.

### b. **Degree Tracking**

* Degree indicates how many edges are connected to a node.
* Leaves have degree 1; pruning reduces degree of neighbors.

### c. **Topological-like Trimming**

* Similar to topological sort, but undirected.
* Works like peeling an onion from the outermost layer inward.

### d. **Breadth-First Search (BFS)**

* The pruning mimics BFS from leaf nodes.
* Ensures layer-by-layer traversal inward toward the centroids.

---

## Time and Space Complexity

* **Time Complexity:** O(n)

  * Each edge and node is visited once.
* **Space Complexity:** O(n)

  * For storing degrees, queue, and adjacency list.

---

## Summary

This solution uses core graph theory concepts:

* Tree structure
* Unique paths
* Centroid identification
* BFS-style leaf trimming

By repeatedly removing leaves and updating degrees, we narrow down to the centroids that define Minimum Height Trees. The algorithm is efficient and leverages the mathematical properties of trees and their structure to find the optimal root positions for minimizing height.

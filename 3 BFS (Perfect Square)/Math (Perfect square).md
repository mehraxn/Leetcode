# Minimum Perfect Squares Decomposition Problem

This document provides an explanation of the problem of finding the minimum number of perfect square numbers (e.g., 1², 2², 3², …) that sum up to a given integer \( n \). This problem is closely related to **Lagrange's Four Square Theorem**, which states that any positive integer can be represented as the sum of four or fewer perfect squares. The algorithm here determines the exact minimum number of perfect squares required using a Breadth-First Search (BFS) approach.

## Step-by-Step Explanation

### 1. Breadth-First Search (BFS) Framework
The algorithm employs BFS, treating \( n \) as the root of a tree. Each level of the tree represents subtracting a perfect square from the current number, and the goal is to find the smallest depth (or level) at which the remaining number becomes 0.

- **Initial state:** Start with \( n \) in the queue.
- **Next states:** Subtract all perfect squares \( i^2 \) where \( i^2 \leq \text{current number} \).
- **Stopping condition:** When the number becomes 0, the depth (number of levels traversed) represents the minimum number of perfect squares required.

### 2. Mathematical Breakdown
For a given number \( n \):

- **Perfect squares subtraction:** At each step, subtract the largest perfect square \( i^2 \) such that \( i^2 \leq n \). The result \( n - i^2 \) is added to the next queue for further exploration.
- **Key Insight:** BFS ensures that we explore the minimum number of steps (or levels) needed because we exhaust all possibilities at one level before moving to the next.

### 3. Key Observations from Mathematics

#### Lagrange's Four Square Theorem
Any integer can be expressed as the sum of four or fewer perfect squares:

- **Example 1:** \( n = 12 \) can be expressed as \( 4 + 4 + 4 = 12 \) (3 perfect squares).
- **Example 2:** \( n = 7 \) can be expressed as \( 4 + 1 + 1 + 1 = 7 \) (4 perfect squares).

#### Greedy Choice (Not Optimal)
Simply subtracting the largest perfect square at each step might not lead to the minimum number of steps (e.g., \( n = 12 \) might tempt subtracting \( 9 \) first, which is suboptimal).

#### BFS Guarantees Optimality
BFS ensures that all possible ways of subtracting perfect squares are explored level by level. This way, the first time 0 is reached, it corresponds to the minimum number of perfect squares.

### 4. Steps in the Algorithm

#### For \( n = 12 \):

1. **Step 1:** Start with \( 12 \):
   - Subtract \( 1^2, 2^2, \) or \( 3^2 \): \( 12 - 1 = 11 \), \( 12 - 4 = 8 \), \( 12 - 9 = 3 \).
   - Queue for next level: \([11, 8, 3]\).

2. **Step 2:** Explore \([11, 8, 3]\):
   - Subtract all possible perfect squares from \( 11 \): \( 11 - 1^2 = 10 \), \( 11 - 2^2 = 7 \), etc.
   - Repeat for \( 8 \) and \( 3 \).
   - Continue adding results to the next queue.

3. **Final Step:** When \( 0 \) is reached, return the depth of the tree (number of steps). This is the minimum number of perfect squares.

### 5. Why BFS Works

BFS is suitable because it explores all possibilities in increasing order of steps. This ensures that the first time we encounter 0, we have used the minimum number of perfect squares.

## Conclusion
The BFS-based approach guarantees an optimal solution to the problem of finding the minimum number of perfect squares that sum to \( n \). By systematically exploring all possibilities level by level, the algorithm ensures accuracy and efficiency.


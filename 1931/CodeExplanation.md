# Grid Coloring Problem - Comprehensive Code Explanation

## Problem Statement

This problem asks us to solve a grid coloring challenge with specific constraints:

Given two integers `m` and `n`, we need to color an `m × n` grid where:
1. Each cell can be painted one of three colors: red, green, or blue
2. All cells MUST be painted
3. No two adjacent cells (sharing an edge) can have the same color

The task is to return the total number of ways to color the grid while satisfying these constraints. Since the answer can be very large, we need to return it modulo 10^9 + 7.

### Constraints:
- 1 ≤ m ≤ 5 (number of rows)
- 1 ≤ n ≤ 1000 (number of columns)

### Examples:

**Example 1:**
- Input: m = 1, n = 1
- Output: 3
- Explanation: For a 1×1 grid, we can use any of the three colors (red, green, or blue), giving us exactly 3 possibilities.

**Example 2:**
- Input: m = 1, n = 2
- Output: 6
- Explanation: For a 1×2 grid, we have 6 valid colorings:
  1. First cell red, second cell green
  2. First cell red, second cell blue
  3. First cell green, second cell red
  4. First cell green, second cell blue
  5. First cell blue, second cell red
  6. First cell blue, second cell green

**Example 3:**
- Input: m = 5, n = 5
- Output: 580986

## Problem Analysis

This is a classic constraint satisfaction problem that could grow exponentially with the grid size. The total number of possible colorings without constraints would be 3^(m×n), which is extremely large even for moderate grid sizes.

The key insight is that the constraints restrict which colorings are valid, and we can use dynamic programming to build up the solution.

### Why Dynamic Programming?

The problem has:
1. **Optimal substructure**: The solution to the whole grid can be built from solutions to smaller parts (columns)
2. **Overlapping subproblems**: The same column pattern combinations will be calculated multiple times

### Key Observations

1. The constraint m ≤ 5 is small, which allows us to enumerate all possible column patterns
2. Adjacent cells (both horizontally and vertically) cannot have the same color
3. We can solve the problem column by column, keeping track of compatible transitions

## Solution Approach

The solution uses dynamic programming with state representation based on column patterns:

1. Generate all valid column patterns (columns where no vertically adjacent cells have the same color)
2. For each valid pattern, determine which other patterns can be placed adjacent to it
3. Use DP to count valid colorings by building the grid column by column

## Detailed Code Explanation

Let's break down the code line by line:

### Header Inclusions and Definitions

```c
#include <stdio.h>
#include <string.h>
```
- `stdio.h`: Standard input/output library for functions like `printf`
- `string.h`: For memory functions like `memset` and `memcpy`

```c
#define MOD 1000000007
```
- Defines the modulo value (10^9 + 7) used to prevent integer overflow for large results

```c
#define MAX_PATTERNS 243
```
- The maximum number of possible column patterns
- This comes from 3^5 = 243, as each cell in a column can have 3 colors and m ≤ 5
- In practice, the actual number of valid patterns will be less due to the constraint

```c
#define MAX_M 5
```
- The maximum possible value for m (number of rows) as per the constraints

### Data Structure Definition

```c
typedef struct {
    int colors[MAX_M];
} ColumnPattern;
```
- Defines a struct to store a valid column pattern
- The `colors` array stores the color of each cell in the column (0, 1, or 2 representing the three colors)

### Function: Generate Valid Column Patterns

```c
int generate_valid_patterns(ColumnPattern patterns[], int m) {
```
- This function generates all valid column patterns where no two vertically adjacent cells have the same color
- Parameters:
  - `patterns[]`: Array to store valid patterns
  - `m`: Number of rows in the grid
- Returns: The number of valid patterns found

```c
    int count = 0;
```
- Initialize a counter for valid patterns

```c
    int max_code = 1;
    for (int i = 0; i < m; ++i) max_code *= 3;
```
- Calculate 3^m, which is the total number of possible color combinations for a column
- Each cell can have 3 colors, and there are m cells in a column

```c
    for (int code = 0; code < max_code; ++code) {
```
- Iterate through all possible color combinations (from 0 to 3^m - 1)
- Each number in this range can be represented in base-3, which corresponds to a coloring

```c
        int valid = 1;
        ColumnPattern pattern;
        int temp_code = code;
```
- Assume the pattern is valid until proven otherwise
- Create a temporary pattern object
- Make a copy of the current code to extract colors

```c
        for (int row = 0; row < m; ++row) {
            pattern.colors[row] = temp_code % 3;
            temp_code /= 3;
```
- Convert the number to a base-3 representation
- Each digit (0, 1, or 2) represents one of the three colors
- The modulo operation gives the remainder when divided by 3, which is the color for the current row
- Then divide by 3 to move to the next digit

```c
            if (row > 0 && pattern.colors[row] == pattern.colors[row - 1]) {
                valid = 0;
                break;
            }
```
- Check if the current cell has the same color as the one above it
- If so, mark the pattern as invalid and break the loop
- This enforces the constraint that no two vertically adjacent cells can have the same color

```c
        if (valid) {
            patterns[count] = pattern;
            ++count;
        }
```
- If the pattern is valid, add it to our collection and increment the counter

```c
    return count;
}
```
- Return the total number of valid patterns found

### Function: Check Pattern Compatibility

```c
int are_patterns_compatible(const ColumnPattern *a, const ColumnPattern *b, int m) {
```
- This function checks if two column patterns can be placed adjacent to each other
- Parameters:
  - `a`: The first column pattern
  - `b`: The second column pattern
  - `m`: Number of rows
- Returns: 1 if compatible, 0 otherwise

```c
    for (int row = 0; row < m; ++row) {
        if (a->colors[row] == b->colors[row]) {
            return 0;
        }
    }
    return 1;
}
```
- For each row, check if the corresponding cells in both columns have the same color
- If any pair has the same color, the columns are incompatible (return 0)
- If all corresponding cells have different colors, the columns are compatible (return 1)
- This enforces the constraint that no two horizontally adjacent cells can have the same color

### Main Function: Color The Grid

```c
int colorTheGrid(int m, int n) {
```
- The main function that calculates the total number of ways to color the grid
- Parameters:
  - `m`: Number of rows
  - `n`: Number of columns
- Returns: The number of valid colorings modulo 10^9 + 7

```c
    ColumnPattern patterns[MAX_PATTERNS];
    int num_patterns = generate_valid_patterns(patterns, m);
```
- Create an array to store all valid column patterns
- Call the function to generate these patterns and get the count

```c
    char compatible[MAX_PATTERNS][MAX_PATTERNS];
```
- Create a 2D array to store compatibility information between all pairs of patterns
- `compatible[i][j]` will be 1 if pattern i can be placed to the left of pattern j, 0 otherwise

```c
    for (int i = 0; i < num_patterns; ++i) {
        for (int j = 0; j < num_patterns; ++j) {
            compatible[i][j] = are_patterns_compatible(&patterns[i], &patterns[j], m);
        }
    }
```
- Precompute the compatibility between all pattern pairs
- This avoids redundant calculations during the dynamic programming step

```c
    long long dp[MAX_PATTERNS] = {0};
    long long next_dp[MAX_PATTERNS] = {0};
```
- Create two DP arrays:
  - `dp`: Stores the number of ways to color the grid up to the current column
  - `next_dp`: Temporary array for computing the next state

```c
    for (int i = 0; i < num_patterns; ++i) {
        dp[i] = 1;
    }
```
- Initialize the base case: For the first column, there is 1 way to use each valid pattern

```c
    for (int col = 1; col < n; ++col) {
```
- Iterate through the remaining columns (from the second column to the nth column)

```c
        memset(next_dp, 0, sizeof(next_dp));
```
- Reset the next_dp array for the current iteration

```c
        for (int prev = 0; prev < num_patterns; ++prev) {
            if (dp[prev] == 0) continue;
```
- For each valid pattern of the previous column
- Skip patterns that cannot be reached (dp[prev] = 0)

```c
            for (int curr = 0; curr < num_patterns; ++curr) {
                if (compatible[prev][curr]) {
                    next_dp[curr] = (next_dp[curr] + dp[prev]) % MOD;
                }
            }
        }
```
- For each valid pattern of the current column, check if it's compatible with the previous column
- If compatible, add the number of ways to reach the previous pattern to the current pattern's count
- Apply modulo to prevent overflow

```c
        memcpy(dp, next_dp, sizeof(next_dp));
```
- Update the dp array for the next iteration
- Now dp represents the number of ways to color the grid up to the current column

```c
    long long total = 0;
    for (int i = 0; i < num_patterns; ++i) {
        total = (total + dp[i]) % MOD;
    }
```
- Sum up all the ways to color the last column
- This gives the total number of ways to color the entire grid

```c
    return (int)total;
}
```
- Return the final result as an integer

## Time and Space Complexity Analysis

### Time Complexity:
- **Pattern Generation**: O(3^m × m) - We consider all 3^m possible patterns and validate each one in O(m) time
- **Compatibility Check**: O(num_patterns^2 × m) - We check all pairs of patterns for compatibility
- **Dynamic Programming**: O(n × num_patterns^2) - For each of the n columns, we consider transitions between all pattern pairs

The dominant term is O(n × num_patterns^2), where num_patterns is at most 3^m but in practice much less due to the constraint.

Since m ≤ 5, num_patterns is bounded by a constant, making the overall time complexity effectively O(n).

### Space Complexity:
- **Patterns Storage**: O(num_patterns × m)
- **Compatibility Matrix**: O(num_patterns^2)
- **DP Arrays**: O(num_patterns)

Again, since m ≤ 5, the space complexity is effectively O(1) with respect to the input parameters.

## Example Walkthrough

Let's trace through a simple example to illustrate how the algorithm works:

### Example: m = 1, n = 2 (a 1×2 grid)

1. **Generate Valid Patterns**:
   - For m = 1, there are 3 possible patterns: [0], [1], and [2] (representing red, green, and blue)
   - All are valid since there's only one cell per column

2. **Compute Compatibility**:
   - [0] is compatible with [1] and [2], but not with [0]
   - [1] is compatible with [0] and [2], but not with [1]
   - [2] is compatible with [0] and [1], but not with [2]

3. **Dynamic Programming**:
   - Initialize dp = [1, 1, 1] (one way to color the first column with each pattern)
   - For the second column:
     - From [0] we can go to [1] or [2]: +1 to next_dp[1], +1 to next_dp[2]
     - From [1] we can go to [0] or [2]: +1 to next_dp[0], +1 to next_dp[2]
     - From [2] we can go to [0] or [1]: +1 to next_dp[0], +1 to next_dp[1]
     - next_dp becomes [2, 2, 2]
   - Total = 2 + 2 + 2 = 6

This matches the expected output for Example 2.

## Summary

This solution efficiently solves the grid coloring problem by:

1. Exploiting the small constraint on m to enumerate all valid column patterns
2. Using a compatibility matrix to quickly determine which patterns can be adjacent
3. Applying dynamic programming to build the solution column by column

The approach reduces an exponentially large problem (3^(m×n) potential colorings) to a linear-time algorithm with respect to n. The modulo operation ensures that the result fits within the integer range, even for very large inputs.

The code is well-optimized for the given constraints and provides a practical solution to what would otherwise be an intractable combinatorial problem.
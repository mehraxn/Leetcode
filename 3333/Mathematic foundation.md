# README: Mathematical Explanation for "Find the Original Typed String II"

## Problem Overview

Given a string `word` that may contain repeated characters due to long keypresses, and an integer `k`, we want to find the number of different original strings that Alice might have intended to type. The original string must have a length of at least `k`. The result must be returned modulo 10^9 + 7.

## Mathematical Reformulation

We interpret `word` as a sequence of character runs, where each run represents a single character typed one or more times.

Let the final string `word` be broken down into runs:

```
word = a^C1 b^C2 c^C3 ... z^Cm
```

Each `Ci` denotes the number of times a character appears in a run. Alice could have intended to press that key anywhere from 1 to `Ci` times.

Let `t_i` be the number of times Alice intended to press the key for run `i`:

* `1 <= t_i <= C_i`
* `sum(t_i)` = original length of the string Alice meant to type

Our goal is to count how many valid combinations of `(t_1, t_2, ..., t_m)` satisfy:

```
sum(t_i) >= k
```

## Transformation

We define `u_i = t_i - 1`. Then:

* `0 <= u_i <= C_i - 1`
* `sum(u_i) >= k - m`

Let:

* `D_i = C_i - 1`
* `L = k - m`

Now the problem becomes:
Count the number of integer vectors `(u_1, ..., u_m)` such that:

* `0 <= u_i <= D_i`
* `sum(u_i) >= L`

## Total Possibilities

The total number of ways to pick `t_i` without any constraint is:

```
Product(C_i) over all i
```

We subtract the number of combinations where `sum(u_i) < L`.

## Efficient Counting with Dynamic Programming

Let `dp[s]` be the number of ways to reach sum `s` using the bounded values `u_i`.
We use a prefix sum trick to update `dp` efficiently:

* Initialize `dp[0] = 1`
* For each run `i`, update `dp` using its maximum value `D_i`
* We update only sums up to `L - 1` (bad cases)

After processing all runs:

```
valid = total - bad = Product(C_i) - sum(dp[0] + ... + dp[L-1])
```

Return this result modulo 10^9 + 7.

## Time Complexity

* Run-length encoding: O(N)
* DP on sum up to L: O(m \* L) where `m <= N` and `L <= 2000`
* Total: O(N + k^2)

## Edge Cases

* If `k > N`, return 0 (even typing one char per run won't reach `k`)
* If `m >= k`, return total possibilities without filtering (every valid combo has enough length)

## Final Notes

This solution leverages combinatorics and constrained integer partitioning, transformed into a bounded knapsack-style problem with dynamic programming.

# Longest Valid Subsequence

This README explains the C implementation of the function `maximumLength`, which finds the length of the longest valid subsequence in an integer array under a modulo constraint.

## Problem Statement

Given:

* An integer array `nums` of size `n`.
* A positive integer `k`.

Definition:
A subsequence `sub` of `nums` (not necessarily contiguous) of length `x` is **valid** if:

```
(sub[0] + sub[1]) % k == (sub[1] + sub[2]) % k == ... == (sub[x-2] + sub[x-1]) % k
```

In other words, every adjacent pair in the subsequence has the same sum modulo `k`.

**Goal:** Return the length of the longest valid subsequence.

## High-Level Approach

We use a **dynamic programming** strategy that tracks, for each possible target residue `r` (the common `(a+b)%k`), and each possible ending residue `c` (the value of the last element modulo `k`), the length of the longest valid subsequence found so far.

### DP State Definitions

* `dp[r][c]` : The maximum length of a valid subsequence with common adjacent-sum modulo = `r`, and whose last element has value congruent to `c` modulo `k`.
* `freq[c]` : The number of elements processed so far whose value mod `k` equals `c`. This helps us start new subsequences of length 2.

We flatten the 2D `dp` into a 1D array of size `k*k`, accessed as `dp[r * k + c]`.

### Initialization

* Allocate `dp` array of size `k*k` and initialize all entries to `0`.
* Allocate `freq` array of size `k` and initialize all entries to `0`.
* Maintain a variable `ans = 1`, since any single element is a valid subsequence of length 1.

### Iteration

For each element `nums[i]`:

1. Compute its residue `m = nums[i] % k`.
2. Prepare a temporary array `new_dp[r]` for each `r` in `[0, k-1]`, initially setting:

   ```c
   new_dp[r] = dp[r][m];  // Extend any subsequence already ending in residue m without increasing length
   ```
3. **Extend existing subsequences by 1:**
   For each `r`:

   * To extend a subsequence with common-sum-residue `r`, whose previous last residue was `c`, we need

     ```c
     (c + m) % k == r  =>  c == (r - m + k) % k.
     ```
   * If `dp[r][c] > 0`, we can append `nums[i]` to that subsequence:

     ```c
     new_dp[r] = max(new_dp[r], dp[r][c] + 1);
     ```
4. **Start new length-2 subsequences:**

   * Any previously seen element with residue `c` can pair with `m` to form a length-2 subsequence.
   * The common-sum-residue becomes `r2 = (c + m) % k`.
   * So if `freq[c] > 0`:

     ```c
     new_dp[r2] = max(new_dp[r2], 2);
     ```
5. **Write back** the updated values into `dp` for all `r`:

   ```c
   dp[r][m] = new_dp[r];
   ans = max(ans, dp[r][m]);
   ```
6. Increment `freq[m]`.

By the end, `ans` holds the length of the longest valid subsequence.

## Time and Space Complexity

* **Time Complexity:** O(n·k)

  * We process `n` elements. For each, we loop over `k` residues multiple times (constant factor).
* **Space Complexity:** O(k²) for the `dp` array, plus O(k) for `freq`.

## Code Walkthrough

```c
int maximumLength(int* nums, int numsSize, int k) {
    int *dp = calloc(k * k, sizeof(int));    // dp[r*k + c]
    int *freq = calloc(k, sizeof(int));      // frequency of residues
    int *new_dp = malloc(k * sizeof(int));   // temporary for updates
    int ans = 1;

    for (int i = 0; i < numsSize; i++) {
        int m = nums[i] % k;
        // Step 2: carry over existing dp values ending in m
        for (int r = 0; r < k; r++) {
            new_dp[r] = dp[r * k + m];
        }
        // Step 3: extend any valid subsequence by this element
        for (int r = 0; r < k; r++) {
            int c = (r - m + k) % k;
            int idx = r * k + c;
            if (dp[idx] > 0) {
                int val = dp[idx] + 1;
                if (val > new_dp[r]) new_dp[r] = val;
            }
        }
        // Step 4: start new subsequences of length 2
        for (int c = 0; c < k; c++) {
            if (freq[c] > 0) {
                int r2 = (c + m) % k;
                if (new_dp[r2] < 2) new_dp[r2] = 2;
            }
        }
        // Step 5: update dp and answer
        for (int r = 0; r < k; r++) {
            int idx2 = r * k + m;
            dp[idx2] = new_dp[r];
            if (dp[idx2] > ans) ans = dp[idx2];
        }
        // Step 6: mark this residue seen
        freq[m]++;
    }

    free(dp);
    free(freq);
    free(new_dp);
    return ans;
}
```

* We allocate and free dynamic arrays for DP and frequency bookkeeping.
* The core loop computes possible extensions and new subsequences efficiently in O(k) per element.

## Example

For `nums = [1,4,2,3,1,4]` and `k = 3`:

* Residues: \[1, 1, 2, 0, 1, 1]
* The algorithm finds the subsequence `[1,4,1,4]` (residues `[1,1,1,1]`) with common sum residue = 2, of length 4.

---

This completes the detailed explanation of the DP algorithm and its C implementation for finding the longest valid subsequence under the given modulo constraint. Feel free to refer back to this document for understanding the design choices and complexity analysis.

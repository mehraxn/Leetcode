# README: Deep Line-by-Line Code Explanation for `possibleStringCount`

## Objective

This C program solves the problem of counting all possible original strings that could result in a final string `word`, typed with potentially long keypresses, such that the original string has a length of at least `k`. The final answer is taken modulo 10^9 + 7, as the result can be very large.

---

## Section 1: Fast Modular Exponentiation Function

```c
long long modexp(long long a, long long e) {
    long long r = 1;
    a %= MOD;
    while (e) {
        if (e & 1) r = (r * a) % MOD;
        a = (a * a) % MOD;
        e >>= 1;
    }
    return r;
}
```

* **Purpose**: Calculates $a^e \mod \text{MOD}$ using binary exponentiation.
* **Why it's fast**: Instead of multiplying `a` by itself `e` times (which is O(e)), this method uses exponentiation by squaring to reduce the time complexity to O(log e).
* `r` holds the result, initialized to 1.
* Inside the loop:

  * If the current bit of exponent `e` is set (`e & 1`), multiply `r` by `a` and take modulo MOD.
  * Square `a` and reduce it modulo MOD.
  * Right shift `e` (divide by 2).
* **Note**: This function isn't used in the final implementation but is useful for modular inverses.

---

## Section 2: Core Function: `possibleStringCount`

### Declaration and Initialization

```c
int possibleStringCount(char* word, int k) {
    int n = strlen(word);
```

* Input: `word` is the final displayed string, and `k` is the required minimum original string length.
* `n` is the total number of characters in `word`.

```c
    int *C = malloc((n+1) * sizeof(int));
    int m = 0;
```

* `C` will store the length of each run of characters in `word` (run-length encoding).
* `m` counts how many character runs there are.

### Run-Length Encoding

```c
    for (int i = 0; i < n; ) {
        int j = i+1;
        while (j < n && word[j] == word[i]) j++;
        C[m++] = j - i;
        i = j;
    }
```

* This loop groups identical consecutive characters together:

  * Starts at index `i`, looks ahead with `j` while the characters are equal.
  * `j - i` gives the run length.
  * Stores the run length in `C[m]`, then increments `m`.
  * Continues until all characters are processed.

Example:

* For `word = "aabbccdd"`, run lengths will be `[2, 2, 2, 2]`, and `m = 4`.

### Early Exit Cases

```c
    if (k > n) {
        free(C);
        return 0;
    }
```

* If the required original length `k` exceeds the final word length `n`, it's impossible. Return 0.

### Calculate Total Possible Configurations

```c
    long long prodC = 1;
    for (int i = 0; i < m; i++) {
        prodC = (prodC * C[i]) % MOD;
    }
```

* Every run of length `C[i]` can correspond to any `t_i` from 1 to `C[i]`.
* The total number of unconstrained combinations is the product of `C[i]` over all runs.

```c
    if (m >= k) {
        free(C);
        return (int)prodC;
    }
```

* If the number of runs `m` is already >= `k`, even the minimal `t_i = 1` gives a valid length.
* So all combinations are valid. Return `prodC`.

---

## Section 3: Handling Case When m < k (Need to Filter Invalid Cases)

```c
    int L = k - m;
    long long *dp = calloc(L, sizeof(long long));
    dp[0] = 1;
```

* Let `L = k - m`. This is how much more length (in addition to 1 per run) we need.
* `u_i = t_i - 1` now ranges from 0 to `C[i] - 1`, and we require:

  * `sum(u_i) >= L`
* Initialize `dp[0] = 1` because there's one way to get sum 0 — choose all `u_i = 0`.

```c
    long long *pref = malloc(L * sizeof(long long));
```

* Prefix sum array used to optimize DP computation.

---

## Section 4: Dynamic Programming Loop

```c
    for (int i = 0; i < m; i++) {
        int D = C[i] - 1;
```

* For each run, `u_i` can take values from 0 to `D = C[i] - 1`.
* We update the DP array to account for all valid `u_i`.

```c
        pref[0] = dp[0];
        for (int s = 1; s < L; s++) {
            pref[s] = (pref[s-1] + dp[s]) % MOD;
        }
```

* Compute prefix sums for current DP to allow fast range-sum queries.
* `pref[s] = sum(dp[0] to dp[s])` modulo MOD.

```c
        for (int s = 0; s < L; s++) {
            if (s <= D) {
                dp[s] = pref[s];
            } else {
                dp[s] = (pref[s] - pref[s - D - 1] + MOD) % MOD;
            }
        }
    }
```

* This updates `dp[s]` using window sums:

  * If `s <= D`, we can use all previous entries up to `s`.
  * If `s > D`, we subtract the values outside the sliding window `[s-D, s]`.
  * The addition of MOD ensures no negative numbers after subtraction.

---

## Section 5: Compute and Return the Final Answer

```c
    long long bad = 0;
    for (int s = 0; s < L; s++) {
        bad = (bad + dp[s]) % MOD;
    }
```

* Sum all values of `dp[s]` where `s < L`, which correspond to invalid original strings of length `< k`.

```c
    long long ans = (prodC - bad + MOD) % MOD;
```

* Subtract invalid combinations from total. Again, add MOD to avoid negatives before applying modulo.

```c
    free(C);
    free(dp);
    free(pref);
    return (int)ans;
}
```

* Free all dynamically allocated memory and return the final answer as an integer.

---

## Section 6: Main Function for Testing

```c
int main() {
    char w1[] = "aabbccdd";
    printf("%d\n", possibleStringCount(w1, 7));
    printf("%d\n", possibleStringCount(w1, 8));

    char w2[] = "aaabbb";
    printf("%d\n", possibleStringCount(w2, 3));
    return 0;
}
```

* Test the function with sample inputs.
* The answers printed verify the logic:

  * For "aabbccdd" and `k=7`, result is 5.
  * For `k=8`, only full match is valid: result is 1.
  * For "aaabbb" and `k=3`, multiple combinations satisfy: result is 8.

---

## Summary

This implementation uses a combination of:

* Run-length encoding to simplify the character analysis.
* Combinatorial analysis using integer partitions.
* Bounded knapsack dynamic programming to subtract invalid sequences.

The result is a robust, efficient solution that runs in $O(n + k^2)$ time and uses $O(k)$ extra space. It handles edge cases and large inputs effectively using modulo arithmetic and optimized DP techniques.

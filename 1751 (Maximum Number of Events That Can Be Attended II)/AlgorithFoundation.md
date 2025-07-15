# Maximum Value of **k** Events – Algorithm Explanation

---

## Problem Recap

You are given **events**, where each event `events[i] = [start_i, end_i, value_i]` describes an interval that **starts at `start_i`, ends at `end_i` (inclusive),** and yields a reward of `value_i` if you attend it.
You may attend **at most `k` events** and **never overlap** two events (because you must attend the *entire* interval once you choose it).

> **Goal** – Find the **maximum total value** obtainable by selecting up to `k` non‑overlapping events.

---

## Core Idea

The task generalises classic *Weighted Interval Scheduling* to the case of **at most `k` picks** instead of exactly one.
We solve it with **dynamic programming + binary search**:

1. **Sort events by start day.**
2. For every event *i*, quickly find the first next event that starts **after** `end_i` using `bisect_right` → gives **index `next(i)`**.
3. Define

   $$
       \text{dp}[i][j] = \text{maximum value obtainable starting at index } i \text{ when } j \text{ picks remain}
   $$
4. Transition (bottom‑up):

   * **Skip** event *i*: `dp[i+1][j]`
   * **Take** event *i*: `value_i + dp[next(i)][j‑1]`
     Choose the larger.

Because we always jump to the earliest non‑overlapping candidate, the schedule built from these choices is valid and globally optimal.

---

## Step‑by‑Step Walk‑through

| Step                                                      | Action                                                                                                    | Purpose                                                         |
| --------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- |
| 1                                                         | **Sort** `events` ascending by `start`. Build an auxiliary array `starts` containing just the start days. | Enables binary search and ensures deterministic DP order.       |
| 2                                                         | **Initialise** a `(n + 1) × (k + 1)` table `dp` with zeros.                                               | `dp[n][*] = 0` is the base case (no events left).               |
| 3                                                         | **Iterate** `i` from `n – 1` down to `0` and `j` from `1` to `k`.                                         | Bottom‑up fills the table so dependencies are already computed. |
| 4                                                         | For each `(i, j)` compute `nextIdx = bisect_right(starts, end_i)` and set                                 |                                                                 |
| `dp[i][j] = max(dp[i+1][j], value_i + dp[nextIdx][j‑1])`. | Implements *skip vs take* choice while enforcing non‑overlap and pick limit.                              |                                                                 |
| 5                                                         | **Answer** is `dp[0][k]`.                                                                                 | We start considering the first event with `k` picks left.       |

### Why This Works

* The sub‑problem definition guarantees **optimal sub‑structure**: the best plan from event *i* splits into today’s choice and an optimal plan afterwards.
* Binary search yields the next compatible event in **O(log n)**, preserving correctness while avoiding a linear scan.
* Exhaustively evaluating *skip/take* over every state covers **all feasible schedules with ≤ `k` events**.

---

## Complexity Analysis

| Aspect                                                            | Cost                         |
| ----------------------------------------------------------------- | ---------------------------- |
| **Time**                                                          | Sorting: `O(n log n)`        |
| DP: `O(n × k × log n)` (each state performs one binary search).   |                              |
| The constraints guarantee `n × k ≤ 10^6`, keeping this tractable. |                              |
| **Space**                                                         | `O(n × k)` for the DP table. |
| (With rolling arrays it can be reduced to `O(n × 2)`.)            |                              |

---

## Code‑to‑Concept Mapping

```python
class Solution:
    def maxValue(self, events: List[List[int]], k: int) -> int:
        from bisect import bisect_right

        events.sort()                          # ➊ sort by start
        n = len(events)
        starts = [s for s, _, _ in events]     # helper for bisect

        dp = [[0]*(k+1) for _ in range(n+1)]   # ➋ DP table

        for i in range(n-1, -1, -1):           # ➌ iterate backwards
            for j in range(1, k+1):            #    up to k picks
                nxt = bisect_right(starts, events[i][1])  # ➍ next compatible
                dp[i][j] = max(
                    dp[i+1][j],                       # skip
                    events[i][2] + dp[nxt][j-1]        # take
                )
        return dp[0][k]                         # ➎ answer
```

* **Lines 4‑7** — *Input preparation* (sorting & start‑array for binary search).
* **Line 9** — DP table sized `(n+1) × (k+1)`; row `n` encodes the base case.
* **Lines 11‑17** — Nested loops traverse states bottom‑up; `bisect_right` finds `next(i)` in `O(log n)`.
* **Line 16** — Transition implements *skip* vs *take* logic.
* **Line 17** — Final result when no events processed yet and `k` picks allowed.

---

## Key Takeaways

* The problem is a **multi‑pick weighted interval scheduling** task.
* Combine **binary search** for the next non‑overlapping event with **dynamic programming** over *(index, remaining picks)*.
* Total work `O(n × k × log n)` is efficient for the given limits, and space can be optimised with rolling arrays.

Happy scheduling! 📅✨

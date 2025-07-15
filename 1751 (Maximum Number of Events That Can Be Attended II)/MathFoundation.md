# Reconstructing a k‑Event Schedule – Mathematical Foundations and Full Walk‑through

---

## Table of Contents

1. [Introduction](#introduction)
2. [Formal Problem Statement](#formalproblem)
3. [Interval‑Scheduling Modelling](#intervalmodel)
4. [Existence of an Optimal k‑Bounded Selection](#existence)
5. [Dynamic Programming Recurrence in Depth](#dp)
6. [Binary‑Search Acceleration](#binarysearch)
7. [Data‑Structure Design Choices](#datastructures)
8. [Step‑by‑Step Execution on Example 1](#example1)
9. [Proof of Correctness](#correctness)
10. [Complexity Analysis](#complexity)
11. [Edge Cases & Special Scenarios](#edgecases)
12. [Relation to Classic Results](#relations)
13. [Glossary of Terms](#glossary)
14. [References](#references)

## 1  Introduction

The **Maximum Value of Attending at Most k Events** problem (LeetCode #1751) asks us to choose up to **k** non‑overlapping events so that the sum of their values is maximised.
Each event is described by its starting day, ending day (inclusive), and an integer value.

The Python implementation supplied by the user employs a *bottom‑up* dynamic‑programming (DP) table enhanced with *binary search* to locate the next compatible event in `O(log n)` time.
This document establishes every mathematical foundation behind that algorithm: optimal‑substructure proofs, recurrence derivation, and complexity bounds.

## 2  Formal Problem Statement

Let

$$
    E = \{ (s_i,\, e_i,\, v_i) \mid 1 \le i \le n \} ,\qquad 1 \le s_i \le e_i \le 10^9,\ v_i>0
$$

be an indexed multiset of **events**, where an ordered triple denotes *start day* `s_i`, *end day* `e_i` and *value* `v_i`.
Fix an integer `k (1 \le k \le n)`.

> **Goal.** Choose a subset of event indices `S` with at most `k` elements such that the chosen events do not overlap in time (for any distinct indices `i` and `j` in `S`, either `e_i < s_j` or `e_j < s_i`). The objective is to maximise the total value `Σ_{i∈S} v_i`. Return that maximum value.

## 3  Interval‑Scheduling Modelling

### 3.1  Partial Order of Compatibility

Define a strict order
$(s_i,e_i) \prec (s_j,e_j) \iff e_i < s_j.$
Pairs related by `\prec` can be attended sequentially.  Sorting all events by **start day** gives us a *linear extension* of this partial order that aids DP indexing.

### 3.2  Weighted Interval Scheduling with Cardinality k

If `k=∞` the task reduces to classical *Weighted Interval Scheduling* (WIS).  Here we impose a *hard cap* `k`, giving rise to **k‑bounded WIS**.  The cap necessitates an extra dimension in DP to encode the count of events already picked.

## 4  Existence of an Optimal k‑Bounded Selection

The feasible set of solutions is finite (*$2^n$* subsets), so at least one optimum exists.  Crucially, **optimal‑substructure** holds:

> *Lemma 4.1.* Let
> $\text{OPT}(i,\,j)$ denote the maximum value attainable \*\*from index \*\****i*** onward using **≤ j** more events.
> For any optimal solution on prefix `[i, n)` either
>
> 1. event *i* is **skipped**, inheriting the optimum of `i+1`; **or**
> 2. event *i* is **chosen**, forcing us to start next at the first index `p(i)` with `s_{p(i)} > e_i` and reducing the remaining quota to `j‑1`.

The lemma follows from the greedy partition of solution space and shows that storing only the next compatible index is sufficient for optimality.

## 5  Dynamic Programming Recurrence in Depth

### 5.1  Pre‑processing

Sort `E` by ascending `s_i` (ties arbitrary).  Store the starts in an array
$\small p = [s_1, s_2, \dots, s_n].$

For each `i` compute

$$
    nxt(i) = \min\{ \ell \mid \ell>i \land s_\ell > e_i \}\cup\{n+1\},
$$

which is the **earliest future index** that can follow event `i`.  Binary search (`bisect_right`) over `p` delivers `nxt(i)` in `O(\log n)`.

### 5.2  Recurrence

Define a table

$$
    DP[i][j] = \text{OPT}(i,\,j),\qquad 0\le i\le n,\ 0\le j\le k.
$$

Base cases: `DP[n][j]=0` for all `j`, because no further events exist.

For `i` from `n−1` down to `0` and `j` from `1` to `k`:

$$
    DP[i][j] = \max\Bigl(\,DP[i+1][j],\; v_i + DP[nxt(i)][j-1]\Bigr). \tag{5.1}
$$

The first term *skips* event `i`; the second term *takes* it.

### 5.3  Iterative Filling Order

Reverse iteration (high → low indexes) ensures that sub‑entries `DP[i+1][·]` and `DP[nxt(i)][·]` are already computed when needed, enabling an **in‑place** table without extra buffering.

## 6  Binary‑Search Acceleration

The naïve evaluation of `nxt(i)` by linear scan yields `O(n^2 k)` time.  By storing the **sorted start‑times** array `p` we can instead invoke

```python
from bisect import bisect_right
nxt_i = bisect_right(p, e_i)
```

which runs in `O(log n)` and never revisits earlier indices.  Overall time thus drops to `O(n k log n)`.

## 7  Data‑Structure Design Choices

| Symbol | Meaning                        | Python Representation      | Complexity (per access)  |
| ------ | ------------------------------ | -------------------------- | ------------------------ |
| `E`    | list of events sorted by `s_i` | `list[tuple[int,int,int]]` | —                        |
| `p`    | array of all start days        | `list[int]`                | binary search `O(log n)` |
| `DP`   | value table `(n+1) × (k+1)`    | `list[list[int]]`          | read/write `O(1)`        |

Memory consumption is `O(nk)` which respects the constraint `k⋅n ≤ 10⁶`.

## 8  Step‑by‑Step Execution on Example 1

> **Input**  `events = [[1,2,4],[3,4,3],[2,3,1]],
> k = 2`

**Sorted by start**

| Index | `(s_i,e_i,v_i)` | `nxt(i)` | comment                    |
| ----- | --------------- | -------- | -------------------------- |
| 0     | (1,2,4)         | 2        | first index with `s > 2`   |
| 1     | (2,3,1)         | 3        | first index with `s > 3`   |
| 2     | (3,4,3)         | 3        | no later event ⇒ `n+1` (3) |
| 3     | — sentinel      | —        | beyond last event          |

We fill `DP` bottom‑up (rows `i`, columns `j`). Only salient entries shown:

| `i` \ `j` | 0 | 1 | 2     |
| --------- | - | - | ----- |
| 3         | 0 | 0 | 0     |
| 2         | 0 | 3 | 3     |
| 1         | 0 | 3 | 4     |
| 0         | 0 | 4 | **7** |

Therefore `DP[0][2]=7`, matching the sample explanation `(4 + 3)`.

## 9  Proof of Correctness

We prove by induction on the pair `(i,j)` that `DP[i][j] = OPT(i,j)`.

*Base.* `i=n`: no events remain, optimum value is 0; table initialises to 0.

*Inductive Step.* Assume rows `(i+1)` and `nxt(i)` are correct.
Equation (5.1) examines the only two mutually exclusive possibilities regarding event `i`.  The branch achieving the greater value equals the true optimum of sub‑instance `(i,j)` by *Lemma 4.1*.  Hence `DP[i][j]` stores `OPT(i,j)`.

Because the algorithm visits states in descending `i`, the hypothesis holds for all `i`.  Finally, `DP[0][k]` equals the maximum achievable value for the full instance. ∎

## 10  Complexity Analysis

* **Sorting:** `O(n log n)`.
* **DP table:** `n ⋅ k` states, each processed in `O(log n)` due to `bisect_right` ⇒ `O(n k log n)`.
* **Space:** `O(nk)` integers.

Given `k⋅n ≤ 10⁶`, both time and memory are acceptable in practice.

## 11  Edge Cases & Special Scenarios

* **`k=1`\*\*\*\*:** Reduces to picking the single highest‑valued event; algorithm handles this with `j=1` column.
* **All events mutally overlap:** Only one event can be chosen; algorithm naturally returns the maximum value among them.
* **Disjoint events count ≤ k:** Recurrence continues to pick every event, achieving the sum of all values.
* **Large day coordinates (****`10^9`****):** Only comparisons are performed; no array indexed by day is ever created.

## 12  Relation to Classic Results

* **Weighted Interval Scheduling (WIS):** Presents the same DP idea but without the *k‑cap*, hence one‑dimensional `DP`.
* **Knapsack with Conflicts:** Our problem may be viewed as 0/1‑knapsack where weight is implicit in overlap constraints.
* **Segment Scheduling Theorems:** Interval graphs have chromatic number equal to clique number, underpinning the non‑overlap feasibility test.

## 13  Glossary of Terms

| Term                         | Definition                                                       |
| ---------------------------- | ---------------------------------------------------------------- |
| **Event**                    | Closed interval `[s_i,e_i]` with associated value `v_i`.         |
| **Compatible (Non‑overlap)** | Two events with `e_i < s_j` or `e_j < s_i`.                      |
| **k‑bounded WIS**            | Weighted interval scheduling with a maximum of `k` choices.      |
| **Binary search successor**  | Earliest event whose start exceeds a given end.                  |
| **Dynamic programming**      | Solving a complex problem by combining solutions of subproblems. |

## 14  References

1. Cormen, T., Leiserson, C., Rivest, R., Stein, C. *Introduction to Algorithms*, 3rd ed., MIT Press, 2009, §15.
2. Kleinberg, J., Tardos, É. *Algorithm Design*, Pearson, 2006, §11 (Interval Scheduling).
3. LeetCode Problem #1751: *Maximum Number of Events That Can Be Attended II*.
4. D’Andrea, G. “A decomposition approach to weighted interval scheduling.” *Discrete Appl. Math.* 287 (2020): 71‑84.

---

**End of Document**

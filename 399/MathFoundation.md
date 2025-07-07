# Evaluate Division – Mathematical & Algorithmic Foundations README

## Index

1. [Introduction](#1-introduction)
2. [Problem Statement](#2-problem-statement)
3. [Mathematical Modeling](#3-mathematical-modeling)
      3.1. [Division Equations as Ratios](#31-division-equations-as-ratios)
      3.2. [Directed Weighted Graph Representation](#32-directed-weighted-graph-representation)
      3.3. [Equivalence Relations and Connected Components](#33-equivalence-relations-and-connected-components)
4. [Data Structures](#4-data-structures)
      4.1. [Disjoint Set Union (Union–Find)](#41-disjoint-set-union-union–find)
      4.2. [Weighted Union–Find for Ratio Maintenance](#42-weighted-union–find-for-ratio-maintenance)
5. [Algorithm Explanation](#5-algorithm-explanation)
      5.1. [Building the Structure – ](#51-building-the-structure--union)[`union`](#51-building-the-structure--union)
      5.2. [Path Compression and Weight Adjustment](#52-path-compression-and-weight-adjustment)
      5.3. [Query Evaluation – ](#53-query-evaluation--divide)[`divide`](#53-query-evaluation--divide)
6. [Complexity Analysis](#6-complexity-analysis)
      6.1. [Time Complexity](#61-time-complexity)
      6.2. [Space Complexity](#62-space-complexity)
7. [Proof of Correctness](#7-proof-of-correctness)
8. [Edge Cases & Undefined Variables](#8-edge-cases--undefined-variables)
9. [Numerical Stability Considerations](#9-numerical-stability-considerations)
10. [Step‑by‑Step Walk‑through with Example](#10-step‑by‑step-walk‑through-with-example)
11. [Implementation Details in Python](#11-implementation-details-in-python)
       11.1. [Class Structure](#111-class-structure)
       11.2. [Methods Explained Line‑by‑Line](#112-methods-explained-line‑by‑line)
12. [Testing Strategy](#12-testing-strategy)
13. [Extensions & Variations](#13-extensions--variations)
14. [References](#14-references)

---

## 1. Introduction

The *Evaluate Division* problem transforms a collection of ratio equations between symbolic variables into a system that can answer arbitrary division queries. Mathematically, each equation of the form $A_i / B_i = v_i$ defines a **multiplicative constraint** linking the positive real‑valued unknowns $A_i$ and $B_i$.
To compute answers efficiently for up to twenty queries (per the constraints), we need a data structure that can **merge relation groups** and **retrieve ratio paths** on demand.

---

## 2. Problem Statement

Given

* an array `equations`, where each element is a pair of variables $[A_i, B_i]$,
* an array `values`, where `values[i] = v_i` encodes the scalar ratio $A_i / B_i = v_i$, and
* an array `queries`, where each query $[C_j, D_j]$ asks for $C_j / D_j$,
  return an array of **double‑precision results**. If the variables of a query are not connected through known equations, the result is $-1.0$.
  The challenge lies in supporting **insertion of constraints** and **real‑time evaluation** with minimal recomputation.

---

## 3. Mathematical Modeling

### 3.1 Division Equations as Ratios

Each equation $A/B = k$ expresses that the unknowns satisfy $A = k \cdot B$. This is equivalent to saying the *ratio* between their magnitudes is fixed. Ratios are **multiplicative**, so chaining two equations multiplies their constants: if $A/B = k_1$ and $B/C = k_2$ then $A/C = k_1 k_2$. The reciprocal relation also holds: $B/A = 1/k_1$.

### 3.2 Directed Weighted Graph Representation

We can interpret the system as a **directed, weighted graph** $G = (V, E)$:

* **Vertices** $V$ are unique symbolic variables.
* **Edges** $E$ connect variables with weights: an equation $A/B = k$ adds edge $A \to B$ of weight $k$ and edge $B \to A$ of weight $1/k$.
* The answer to a query $C/D$ equals the **product of edge weights** along any path from $C$ to $D$. If no path exists, the variables are disconnected.

Although a graph with DFS/BFS can answer queries, continually searching paths per query is $\mathcal O(|V| + |E|)$ per query, which is acceptable at this scale but sub‑optimal for many queries or larger input.

### 3.3 Equivalence Relations and Connected Components

Observe that "is connected by a ratio" forms an **equivalence relation**:

* *Reflexive*: $A/A = 1$.
* *Symmetric*: If $A/B = k$ then $B/A = 1/k$.
* *Transitive*: If $A/B = k_1$ and $B/C = k_2$ then $A/C = k_1 k_2$.
  Thus variables fall into **disjoint sets** (connected components). Within each set, every pair has a deterministic ratio; between sets, ratios are undefined. Managing such sets is exactly the purview of a *Disjoint Set Union* (DSU), also called *Union–Find*.

---

## 4. Data Structures

### 4.1 Disjoint Set Union (Union–Find)

A DSU maintains a collection of disjoint sets under two operations:

* **`find(x)`** returns a canonical representative (root) of the set containing $x$.
* **`union(x, y)`** merges the sets of $x$ and $y$.
  Using *path compression* and *union by rank/size*, both operations run in *inverse Ackermann* time $\alpha(n)$, effectively constant for practical input sizes.

### 4.2 Weighted Union–Find for Ratio Maintenance

Standard DSU only tracks connectivity. To answer ratio queries we augment DSU with a **weight map**:

* `weight[x]` stores $w_x$, the product of edge weights from $x$ up to its parent in the forest.
* Invariant: For any node $x$, $\frac{x}{parent[x]} = w_x$.
* The root of each tree represents its set and has `weight[root] = 1`.
  Maintaining this invariant lets us compute $C/D$ as `weight[C] / weight[D]` after both nodes are compressed to the same root.

---

## 5. Algorithm Explanation

### 5.1 Building the Structure – `union`

For each equation $(A,B), k$:

1. Ensure both variables exist in the DSU (`_add`).
2. Find their roots $r_A, r_B$.
3. If they are already in the same set, nothing changes. Otherwise, merge by making root $r_A$ point to root $r_B$.
4. Assign `weight[r_A]` so that the invariant holds:

   $$
       \frac{A}{r_A} \cdot \frac{r_A}{r_B} \cdot \frac{r_B}{B} = k.\]
   Solving for \(\frac{r_A}{r_B}\) yields  
   \[
       weight[r_A] = k \cdot \frac{weight[B]}{weight[A]}.
   $$

This formula uses current weights (ratios to their roots) to set the new edge weight from $r_A$ to $r_B$.

### 5.2 Path Compression and Weight Adjustment

`find(x)` recursively climbs parents until it reaches a root. During the unwind it performs **path compression**: each visited node points directly to the root, multiplying its stored weight by its parent’s weight to preserve the invariant. This flattens the tree without breaking ratio correctness.

### 5.3 Query Evaluation – `divide`

To answer $C/D$:

1. Return $-1$ if either variable is absent.
2. Call `find(C)` and `find(D)`; if their roots differ, they lie in different components ⇒ answer is undefined.
3. Otherwise, by the invariant,

   $$
       \frac{C}{D} = \frac{C}{root} \bigg/ \frac{D}{root} = \frac{weight[C]}{weight[D]}.
   $$

Path compression during `find` guarantees these weights are direct root ratios.

---

## 6. Complexity Analysis

### 6.1 Time Complexity

Each `union` and `divide` involves at most two `find` operations. With path compression and (implicit) union‑by‑size, the amortized cost per `find` is $\mathcal O(\alpha(N))$, where $N$ is the number of unique variables ($\le 40$ in worst case because each equation adds two). For problem constraints this is effectively **constant time**.

* **Preprocessing** (`union` over all equations): $\mathcal O(M \alpha(N))$.
* **Queries**: $\mathcal O(Q \alpha(N))$.

### 6.2 Space Complexity

We store two dictionaries of size $|V|$: `parent` and `weight`. Hence $\mathcal O(|V|)$ additional memory.

---

## 7. Proof of Correctness

We prove by induction that the invariant holds after every `union` and `find`, and that `divide` returns the correct ratio.

*Base Case*: Before any operations, each new node added by `_add` is its own parent with `weight = 1`, so $\frac{x}{parent[x]} = 1$ holds trivially.

*Inductive Step – \*\*`union`*: Assume invariant holds for all nodes. We merge $r_A$ under $r_B$ with
$weight[r_A] = k \cdot \frac{weight[B]}{weight[A]}$.
For any variable $X$ in the subtree of $r_A$ its ratio to $r_B$ becomes
$\frac{X}{r_A} \times \frac{r_A}{r_B} = weight[X] \times weight[r_A]$.
By substitution this equals the product of edge weights along the unique path from $X$ to $r_B$, so the invariant survives. Other sets are unaffected.

*Inductive Step – \*\*`find`*: Path compression sets
`weight[x] *= weight[parent[x]]` *before* redirecting `parent[x] = root`. The new weight is precisely $\frac{x}{root}$, so invariant holds.

Finally, when `divide(C,D)` returns `weight[C]/weight[D]`, these weights encode $\frac{C}{R}$ and $\frac{D}{R}$ for their common root $R$, so quotient is exact. If roots differ, no path exists, hence $-1$ is correct.

---

## 8. Edge Cases & Undefined Variables

* **Unknown variable** – Absent from `parent` ⇒ return $-1$.
* **Same variable** – `C == D` ⇒ `find` returns same root and weights equal, yield $1.0$.
* **Already‑connected equation** – If `union` joins variables with same root, skip to avoid redundant work; invariant already covers ratio consistency.

---

## 9. Numerical Stability Considerations

Because input ratios are bounded ($0 < k \le 20$) and chain length $\le 20$, worst‑case product is $20^{20} \approx 1.05 \times 10^{26}$, within IEEE‑754 double range ($\approx 1.8 \times 10^{308}$). Precision loss is negligible for this range. However, beware of subtraction cancellation—none occurs here since we only multiply/divide positives.

---

## 10. Step‑by‑Step Walk‑through with Example

Consider Example 1:
Equations: $a/b = 2$, $b/c = 3$.

* Stage 1 – Initialization: add `a`, `b`, `c`; each weight $=1$.
* Stage 2 – `union(a,b,2)`

  * Roots: `a`→`a`, `b`→`b`.
  * Make `parent[a]=b`.
  * Compute `weight[a]=2 * weight[b] / weight[a] = 2 * 1 / 1 = 2`.
* Stage 3 – `union(b,c,3)`

  * Roots: `b`→`b`, `c`→`c`.
  * Make `parent[b]=c`.
  * `weight[b]=3 * weight[c] / weight[b] = 3 * 1 / 1 = 3`.
    After preprocessing:

```
parent: {a:b, b:c, c:c}
weight: {a:2, b:3, c:1}
```

* Query `a/c`

  * `find(a)` compresses:

    * `a`→`b`→`c`.
    * Update `weight[a] *= weight[b] = 2 * 3 = 6`; `parent[a]=c`.
  * `find(c)` returns `c`.
  * Answer = `weight[a]/weight[c] = 6/1 = 6`.
* Query `b/a` uses updated weights to return `1/2` ⇒ `0.5`.
* Other queries follow analogously.

---

## 11. Implementation Details in Python

### 11.1 Class Structure

```python
class Solution:
    class UF:
        ...
    def calcEquation(self, equations, values, queries):
        ...
```

`Solution.UF` encapsulates the weighted DSU logic; the outer `Solution` class satisfies LeetCode’s boilerplate.

### 11.2 Methods Explained Line‑by‑Line

* `__init__`: initializes empty `parent` and `weight` dicts.
* `_add(x)`: lazy insertion ensuring every variable has a default root and unit weight.
* `find(x)`:

  1. Adds `x` if missing.
  2. Recursively traces to root.
  3. During recursion unwind, multiplies `weight[x]` by `weight[parent[x]]` (the ratio from parent to root) before re‑pointing.
* `union(x,y,k)`: merges roots with formula described in §5.1.
* `divide(x,y)`: implements the query logic in §5.3.
* `calcEquation`: driver that processes equations then evaluates all queries with list comprehension.

---

## 12. Testing Strategy

* **Unit tests** for single, chained, and disconnected equations.
* **Randomized tests** generating acyclic ratio graphs and verifying against BFS‑computed ground truth.
* **Precision tests** checking reciprocal consistency: compute `v` then query inverse and ensure product ≈ 1 within tolerance.
* **Edge tests**: unknown variables, self‑division, duplicate equations.

---

## 13. Extensions & Variations

* **Dynamic queries**: allow insertion/removal of equations on the fly. The weighted DSU easily handles further unions; deletions require more sophisticated structures (e.g., *dynamic connectivity*).
* **Symbolic algebra**: ratios could be rational functions; weights become fraction objects instead of floats.
* **Scaling beyond 20 variables/queries**: the current asymptotics remain optimal even for millions of variables.

---

## 14. References

1. R. E. Tarjan, *Efficiency of a Good But Not Linear Set Union Algorithm*, Journal of the ACM, 1975.
2. T. H. Cormen et al., *Introduction to Algorithms*, 3rd ed., §21.
3. R. Sedgewick & K. Wayne, *Algorithms*, 4th ed., §1.5 (Weighted Quick‑Union).
4. Union‑Find topic on *CP‑Algorithms* (cp-algorithms.com).
5. IEEE 754‑2008: *Standard for Floating‑Point Arithmetic*.
6. LeetCode Problem 399 *Evaluate Division* discussion threads (accessed 2025‑07‑07).

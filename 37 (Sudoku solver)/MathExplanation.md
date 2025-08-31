# README — The Mathematics Behind the Sudoku Solver (Comprehensive)

## 1) Sudoku, precisely

**Board:** a 9×9 grid indexed by rows $r\in\{1,\dots,9\}$ and columns $c\in\{1,\dots,9\}$.
**Symbols:** digits $d\in\{1,\dots,9\}$.
**Boxes:** 3×3 subgrids indexed by $b\in\{1,\dots,9\}$; each box collects cells whose row and column share the same integer-division-by-3 block. Formally,
$b(r,c) \;=\; 3\,\Big\lfloor\frac{r-1}{3}\Big\rfloor + \Big\lfloor\frac{c-1}{3}\Big\rfloor + 1.$

**Rules:** Each of the digits 1–9 must occur exactly once in every row, column, and 3×3 box; every cell holds exactly one digit.

This is a **constraint satisfaction problem (CSP)**: assign a digit to each cell subject to the above constraints.

---

## 2) From CSP to Exact Cover

### 2.1 Exact Cover (definition)

Given a finite set (the **universe**) $U$ and a family of subsets $\{S_i\}$ with $S_i\subseteq U$, an **exact cover** is a subfamily whose union equals $U$ and in which every element of $U$ appears **exactly once**.

### 2.2 Sudoku as Exact Cover

We encode Sudoku so that **each legal digit placement** corresponds to a subset that “covers” exactly four constraints.

* **Universe of constraints** $U$ has size 324 and is the disjoint union of:

  1. **Cell constraints** $\mathcal C_{cell}$ (81 elements): each cell $(r,c)$ must be filled.
     Element: $C^{cell}_{r,c}$.
  2. **Row–digit constraints** $\mathcal C_{row}$ (81 elements): digit $d$ appears once in row $r$.
     Element: $C^{row}_{r,d}$.
  3. **Column–digit constraints** $\mathcal C_{col}$ (81 elements): digit $d$ appears once in column $c$.
     Element: $C^{col}_{c,d}$.
  4. **Box–digit constraints** $\mathcal C_{box}$ (81 elements): digit $d$ appears once in box $b$.
     Element: $C^{box}_{b,d}$.

* **Candidate placements** (rows of the exact-cover matrix): for every triple $(r,c,d)$ with $r,c,d\in\{1,\dots,9\}$, define a subset

  $$
    S_{r,c,d} \;=\; \{\, C^{cell}_{r,c},\; C^{row}_{r,d},\; C^{col}_{c,d},\; C^{box}_{b(r,c),d} \,\}.
  $$

  There are $9\cdot 9\cdot 9 = 729$ such subsets, and each covers **exactly four** constraints.

**Goal:** pick **81** of the $S_{r,c,d}$ so that every constraint in $U$ is covered once. That selection is precisely a completed Sudoku grid.

---

## 3) Incidence Matrix View

Let $M\in\{0,1\}^{729\times 324}$ be the binary matrix with

$$
  M[(r,c,d),\; C] \;=\; \begin{cases}
    1 & \text{if } C\in S_{r,c,d},\\
    0 & \text{otherwise.}
  \end{cases}
$$

Each row of $M$ has exactly four 1s (one for each constraint type). The total number of 1s is $729\times 4 = 2916$. In the *unconstrained* (blank) puzzle, each column initially has exactly 9 ones (because each constraint can be satisfied by 9 different digits/cells accordingly). Pre-filled digits (the **givens**) reduce the effective matrix by forcing certain rows to be chosen and eliminating conflicting rows.

An exact cover corresponds to selecting 81 rows of $M$ that sum to the all-ones vector in $\mathbb{Z}^{324}$.

---

## 4) 0–1 Integer Programming Formulation (Feasibility)

Introduce binary variables $x_{r,c,d}\in\{0,1\}$ meaning “place digit $d$ in cell $(r,c)$.” Then Sudoku is the feasibility system:

$$
\begin{aligned}
\text{for all } r,c: &\quad \sum_{d=1}^{9} x_{r,c,d} = 1 \quad &\text{(each cell gets exactly one digit)} \\
\text{for all } r,d: &\quad \sum_{c=1}^{9} x_{r,c,d} = 1 \quad &\text{(each row has digit d once)} \\
\text{for all } c,d: &\quad \sum_{r=1}^{9} x_{r,c,d} = 1 \quad &\text{(each column has digit d once)} \\
\text{for all } b,d: &\quad \sum_{(r,c)\in b} x_{r,c,d} = 1 \quad &\text{(each box has digit d once)}
\end{aligned}
$$

**Givens:** if cell $(r,c)$ is fixed to digit $d_0$, then set $x_{r,c,d_0}=1$ and $x_{r,c,d}=0$ for $d\neq d_0$. No objective is needed; we just need feasibility. Solving the 0–1 system is equivalent to finding an exact cover.

---

## 5) Correctness of the Exact-Cover Encoding

We show a bijection between Sudoku solutions and exact covers.

* (**Soundness**) Let $\mathcal S\subseteq \{S_{r,c,d}\}$ be an exact cover. Because every **cell constraint** $C^{cell}_{r,c}$ appears in exactly one chosen subset, exactly one digit is assigned to every cell. Because every **row-/column-/box-digit constraint** appears exactly once, each digit appears exactly once per row, column, and box. Hence the 81 chosen subsets define a valid Sudoku solution.

* (**Completeness**) Given a valid Sudoku solution $f(r,c)\in\{1,\dots,9\}$, select the 81 subsets $\{ S_{r,c,f(r,c)} \}$. They cover each constraint exactly once, so they form an exact cover.

Thus **Sudoku $\Leftrightarrow$ Exact Cover**.

---

## 6) Algorithm X (Conceptual Mathematics)

Algorithm X (Knuth) is a generic **backtracking scheme** to enumerate exact covers:

1. If all constraints are satisfied (no uncovered columns remain), we have a solution.
2. Choose an uncovered constraint (column) with **minimum remaining options** (the **MRV heuristic**).
3. For each row that contains a 1 in that column:

   * Add the row to the partial cover (commit to a digit placement).
   * **Cover** all columns where the row has 1s (enforcing exclusivity), which also removes all rows conflicting with those columns.
   * Recurse. If the recursion fails, **uncover** (undo) and try the next row.

Mathematically, “covering a column” is restricting to the submatrix that excludes that constraint and all rows that touch it. This is a canonical form of **constraint propagation**: selecting one fact removes all incompatible facts.

**Why MRV helps:** selecting the tightest constraint (fewest options) minimizes branching and forces early contradictions to appear near the top of the search tree, reducing the size of the explored subtree.

---

## 7) The Role of Givens (Pre-filled Digits)

A given digit at $(r,c)$ with value $d$ is modeled by **pre-selecting** the corresponding row $S_{r,c,d}$ before the search begins. Covering its four constraints immediately eliminates all inconsistent placements — this mirrors the human reasoning “since this cell is 5, no other 5 can be in its row/column/box and no other digit can be in this cell.” The resulting reduced matrix is typically much sparser and easier to solve.

---

## 8) Counting, Density, and Structure

* **Matrix size:** 729 rows × 324 columns.
* **Row density:** 4 ones per row.
* **Column density (blank grid):** 9 ones per column on average; initially, exactly 9 for each of the 324 constraints.
* **Total ones:** 2916.

These highly regular counts are a direct reflection of Sudoku’s symmetry: each constraint family is balanced, and each candidate placement contributes to one constraint of each family.

---

## 9) Polyhedral / Combinatorial View

The 0–1 IP above describes a **polytope** in $\mathbb{R}^{729}$ defined by the equality constraints and 0–1 bounds. Sudoku feasibility asks whether this polytope contains an **integral point** respecting all givens. Relaxing integrality to $[0,1]$ yields a linear program whose vertex structure relates to combinatorial objects (e.g., Latin squares). In practice, the integrality constraints are essential; the LP relaxation alone is too weak to determine a solution without additional cutting planes.

---

## 10) A Worked Miniature (4×4 Sudoku)

A 4×4 Sudoku with 2×2 boxes uses digits $\{1,2,3,4\}$. The exact-cover mapping is identical in pattern:

* **Constraints:** 16 cells + 16 row-digit + 16 column-digit + 16 box-digit = **64** columns.
* **Candidates:** 4×4×4 = **64** rows; each has 4 ones.
  Selecting 16 rows that cover all 64 columns exactly once reconstructs the 4×4 grid. This toy model is small enough to write the full incidence matrix by hand and confirms the equivalence concretely.

---

## 11) Complexity & Uniqueness

* **Computational hardness:** the *generalized* Sudoku problem (on $n^2\times n^2$ boards with $n\times n$ boxes) is NP-complete. For the fixed 9×9 case, instances are typically solved quickly due to strong structure and givens.
* **Uniqueness:** The exact-cover model does **not** enforce uniqueness; it only encodes feasibility. If a puzzle has multiple solutions, Algorithm X will find one (or more, if asked to continue). To certify uniqueness, run the search to completion and verify that exactly one exact cover exists.

---

## 12) Extending the Model

Many advanced human strategies (e.g., X-Wings, Swordfish, coloring) are forms of **inference** that prune candidates before search. In the exact-cover framework, they correspond to adding implied constraints or performing additional rounds of propagation. The core mathematics, however, already captures Sudoku precisely without such embellishments.

---

## 13) Takeaway

* Sudoku can be expressed **exactly** as choosing 81 mutually compatible facts among 729 possibilities so that 324 logical requirements are satisfied **once each**.
* This is the **exact cover** of a regular, sparse, 0–1 incidence matrix.
* **Algorithm X** is a principled way to search that space, and the MRV choice rule is a mathematically natural, entropy-reducing heuristic.

In short:

$$
\boxed{\text{Sudoku} \;\equiv\; \text{Exact Cover} \;\xrightarrow{\;\text{Algorithm X}\;}\; \text{Solution}}
$$

This equivalence is the mathematical backbone of the solver.

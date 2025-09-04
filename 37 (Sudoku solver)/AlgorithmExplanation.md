# README — Algorithmic Design of the Sudoku Solver 

## 0) Purpose

This document explains, end‑to‑end, the *algorithm* used by the solver: how the puzzle is modeled, which data structures power the search, how covering/uncovering works, why it’s correct, and how to extend or debug it.

---

## 1) High‑Level Overview

**Idea:** Encode Sudoku as an **exact‑cover** problem and solve it with **Algorithm X** using a compact, array‑based engine and a reversible operation stack (instead of Dancing Links pointers).

**Pipeline:**

1. Build an *implicit* 0–1 matrix for the exact‑cover formulation.
2. Initialize compact arrays that map **constraints ⇄ candidates**.
3. Apply the **givens** (pre‑filled digits) as forced choices.
4. Run **Algorithm X** with the **MRV** (minimum remaining values) column selection heuristic.
5. On success, reconstruct the 9×9 board from the chosen rows.

---

## 2) Exact‑Cover Modeling (recap)

* **Constraints (columns = 324):**

  * 81 cell constraints (every cell must be filled once)
  * 81 row‑digit constraints (digit d appears once in row r)
  * 81 column‑digit constraints (digit d appears once in column c)
  * 81 box‑digit constraints (digit d appears once in 3×3 box b)
* **Candidates (rows = 729):** each triple (r,c,d) covers *exactly four* constraints: the cell, row‑digit, column‑digit, and box‑digit.
* Solving Sudoku ≡ choosing **81** candidate rows that cover all **324** constraints exactly once.

We never materialize the full 729×324 matrix; we store only the incidences needed by the search.

---

## 3) Core Data Structures

All arrays live inside a single context object to avoid globals and `static` storage.

* `rowToCols[729][4]`: for each candidate (r,c,d), the four constraint indices it satisfies.
* `colStart[325]` and `colRows[2916]`: a **compressed column storage**; for each constraint column `C`, rows that have a 1 in column `C` are contiguous in `colRows[colStart[C] .. colStart[C+1])`.
* `colSize[324]`: number of currently active rows that can still satisfy each column (dynamic during search).
* `colActive[324]`: whether a constraint column is still uncovered.
* `rowActive[729]`: whether a candidate row is still available.
* `ops[]` (stack of small records): reversible log of changes; each entry is one of:

  * `ROW_ENABLE` (undo of disabling a row)
  * `COL_SIZE_INC` (undo of decremented column size)
  * `COL_UNCOVER` (undo of covering a column)
* `solRows[81]`: the chosen candidate rows that constitute the solution.
* `activeCols`: count of currently active (uncovered) columns.

### Memory Footprint

* `rowToCols`: 729×4 ints
* `colRows`: 2916 ints (total ones in the matrix)
* `colStart`: 325 ints; `colSize`: 324 ints
* Flags and stacks are small. This fits easily within typical OJ limits and requires no heap allocation.

---

## 4) Building the Incidence Layout

**Step A — Enumerate all candidates:** for every `(r,c,d)` compute the four columns:

* `cell = r*9 + c`
* `row  = 81 + r*9 + d`
* `col  = 162 + c*9 + d`
* `box  = 243 + boxIndex(r,c)*9 + d`
  Store them in `rowToCols[(r,c,d)]` and count how many times each column will appear (`counts[col]++`).

**Step B — Prefix sums:** compute `colStart` by prefix‑summing `counts`. This allocates a dedicated segment in `colRows` for each column.

**Step C — Fill column lists:** iterate all candidates again and append their row index into each of its four column segments using a moving tail pointer `next[col]` (initialized from `colStart`).

After this phase:

* Column `C`’s active rows live in `colRows[colStart[C] .. colStart[C+1])`.
* Initially `colSize[C] = colStart[C+1] - colStart[C]` (9 in a blank grid).
* `rowActive[] = 1`, `colActive[] = 1`.

---

## 5) Applying Givens (Pre‑filled Cells)

For each given digit at `(r,c) = d`:

1. Compute its candidate row index `rowIdx(r,c,d)`.
2. **Choose** that row (see §6), which covers the four corresponding columns and removes all conflicting rows.
3. Push the chosen row into `solRows`.

If a given contradicts previous givens, a column’s size will drop to zero; the solver then fails immediately (no solution). The leetcode variant guarantees solvability.

---

## 6) Cover / Uncover Mechanics

These are the heart of Algorithm X.

### 6.1 `coverColumn(C)`

* If `colActive[C]` is already 0, return (idempotent guard).
* Set `colActive[C]=0`, decrement `activeCols`, push `COL_UNCOVER(C)` to the op‑stack.
* For each row `R` in `colRows[colStart[C] .. colStart[C+1])`:

  * If `rowActive[R]` is 1:

    * Set `rowActive[R]=0`; push `ROW_ENABLE(R)`.
    * For each of the other three columns `C2` in `rowToCols[R]` (excluding `C`):

      * If `colActive[C2]` is 1: decrement `colSize[C2]`; push `COL_SIZE_INC(C2)`.

Intuition: covering a constraint removes that constraint and disables all currently compatible rows that touch it, while updating the sizes of the other constraints those rows would have satisfied.

### 6.2 `chooseRow(R)`

For the four columns `C ∈ rowToCols[R]`, call `coverColumn(C)` (in any order). This enforces that none of those columns can be satisfied by any other row, i.e., the choice becomes exclusive.

### 6.3 `uncoverTo(mark)`

Pop the op‑stack back to the saved `mark`, reversing each change:

* `COL_SIZE_INC(C)` → increment `colSize[C]`.
* `ROW_ENABLE(R)` → set `rowActive[R]=1`.
* `COL_UNCOVER(C)` → set `colActive[C]=1`, increment `activeCols`.

The stack ensures **LIFO** reversal. No auxiliary structure is needed.

---

## 7) Search (Algorithm X + MRV)

**Invariant:** at every point, the set of chosen rows (`solRows[0..k)`) are pairwise compatible, and each covered column is satisfied exactly once by those rows.

**Step:**

1. **Pick a column** with `colActive[C]=1` that has the smallest `colSize[C]` (MRV). If no column is active, all constraints are satisfied → **solution**.
2. **Iterate rows** `R` in that column’s segment. Skip if `rowActive[R]=0`.
3. Save `mark = opsTop` and `prevSol = solCount`. Call `chooseRow(R)`; append `R` to `solRows`.
4. Recurse. If recursion returns success, propagate success.
5. Otherwise `uncoverTo(mark)`, restore `solCount = prevSol`, and try the next `R`.
6. If no `R` works, return failure to caller.

Depth is at most 81 (number of cells). With good MRV and real puzzle givens, branching is typically very small.

---

## 8) Correctness Sketch

* **Safety:** `chooseRow(R)` covers each of its four columns; thereafter, no other row that touches those columns remains active. So chosen rows cannot conflict.
* **Progress:** each recursive step covers ≥1 column; recursion bottoms out when all columns are covered.
* **Backtrack soundness:** `uncoverTo(mark)` precisely reverses all effects since `mark`, restoring previous `rowActive`, `colActive`, and `colSize`. Hence the search explores a correct backtracking tree over partial exact covers.
* **Completeness:** if a solution exists, some path chooses exactly the 81 rows that comprise it. The MRV heuristic biases the path but does not remove any solutions.

---

## 9) Complexity & Performance

* **Worst case:** exponential in solution depth (NP‑hard in generalized form).
* **Per step cost:** covering a column touches its incident rows (≤9 in the initial grid) and updates up to 3 other columns per disabled row. In practice, the op‑stack and flat arrays keep constants low.
* **Heuristics:** MRV drastically prunes branching; applying givens early shrinks the matrix; deterministic row order yields reproducible runs.

Empirically, typical 9×9 Sudokus (even “hard”) solve in microseconds–milliseconds in optimized C.

---

## 10) Practical Implementation Notes

* **No `static` state:** everything is wrapped in a `SudokuCtx` passed by pointer.
* **Bounds:** `MAX_OPS` sized with headroom to hold all reversible changes made along any path; membership count is 2916, so 24k is safe.
* **IDs:** columns and rows fit in 16 bits; op entries store `(kind, id)` compactly.
* **Recursion depth:** ≤81; safe on conventional stacks. Tail recursion is not required.
* **Determinism:** choosing the first minimal column and iterating rows in stored order yields stable outcomes.

---

## 11) Reconstructing the Board

After success, `solRows[0..80]` contains the selected candidate row indices. Each index `R` encodes `(r,c,d)`. Write digit `d` into `board[r][c]` to obtain the final 9×9 solution.

---

## 12) Validation & Uniqueness

* **Validation:** verify each of the 324 constraints holds in the reconstructed grid (straightforward masks/sets check).
* **Uniqueness check:** after finding one solution, continue the search (do **not** return immediately) to see whether another disjoint selection exists. If none, the puzzle is unique.

---

## 13) Extensions

* **Dancing Links (DLX):** replace arrays + op‑stack with a 4‑way linked structure enabling O(1) cover/uncover by pointer splicing. Similar asymptotics; different constants and code complexity.
* **Generalized Sudoku (n²×n²):** the same schema with:

  * columns = 4·n⁴; rows = n⁶; each row has 4 ones; each column initially has n² ones.
* **Variant rules:** add extra constraint families (e.g., diagonals) by appending columns and adding the corresponding 1s to each candidate row that participates.
* **Hybrid pruning:** perform light logical eliminations (singles, hidden singles) before or between Algorithm X steps to trim search further.

---

## 14) Debugging & Instrumentation

* Add counters: recursive calls, max depth, ops pushed, columns covered.
* Invariants to assert:

  * `0 ≤ colSize[C] ≤ segment length` for all active columns
  * `rowActive[R]=0 ⇒ its four columns reflect decremented sizes` (since R was disabled)
  * `activeCols` equals the number of `colActive[C]=1`
* Log decisions at small depth to trace branching.

---

## 15) Pseudocode (Reference)

```
build_matrix(ctx)
apply_givens(ctx)
if search(ctx):
    reconstruct_board(ctx)

function search(ctx):
    C := argmin_{colActive[C]=1} colSize[C]
    if no such C: return true
    if colSize[C] == 0: return false
    for R in rows_of(C):
        if rowActive[R] == 0: continue
        mark := opsTop
        chooseRow(R)
        push R into solRows
        if search(ctx): return true
        pop R from solRows
        uncoverTo(mark)
    return false
```

---

## 16) Why This Algorithm

* It mirrors the *mathematical structure* of Sudoku exactly (no ad‑hoc checks).
* It is compact, cache‑friendly, and requires no dynamic allocation.
* The reversible op‑stack is simple yet powerful, offering DLX‑like speed without pointer gymnastics.

**Bottom line:** precise model → principled search → fast, robust solver.

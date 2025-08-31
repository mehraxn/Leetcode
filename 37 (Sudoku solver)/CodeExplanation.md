# README — Line‑by‑Line Walkthrough of the Provided Sudoku Solver

This document explains every part of the provided C code *line by line*, what each piece does, and which algorithmic ideas it implements. The solver models Sudoku as an **Exact‑Cover** problem and solves it with **Algorithm X** using an array‑based engine with a **reversible operation stack** and an **MRV** (minimum remaining values) choice rule.

---

## 1) Macros and Global Constants

```c
#define N 9
#define NROWS (N*N*N)
#define NCOLS (N*N*4)
#define POOL (NROWS*4)
#define MAX_OPS 24000
```

* `#define N 9` — size parameter for a standard Sudoku.
* `#define NROWS (N*N*N)` — number of **candidate rows** in the exact‑cover matrix: one for each `(r,c,d)` triple → `9*9*9 = 729`.
* `#define NCOLS (N*N*4)` — number of **constraints** (columns): `81` cells + `81` row‑digit + `81` column‑digit + `81` box‑digit → `324`.
* `#define POOL (NROWS*4)` — total number of ones in the matrix (each candidate touches 4 constraints) → `729*4 = 2916`.
* `#define MAX_OPS 24000` — capacity for the reversible operation log; safely exceeds the worst number of changes along any path.

**Algorithms referenced:** Exact‑Cover modeling (matrix dimensions), compressed storage sizing.

---

## 2) Tiny Operation Record

```c
typedef struct { unsigned char kind; unsigned short id; } Op;
```

* A compact log entry for **reversible changes** during cover/uncover (Algorithm X).
  `kind` encodes which reversal to perform; `id` identifies the column or row affected.

**Algorithms referenced:** Reversible backtracking (undo stack), core to Algorithm X when not using DLX pointer splicing.

---

## 3) Context Object (No statics, all state localized)

```c
typedef struct {
    int rowToCols[NROWS][4];
    int colStart[NCOLS + 1];
    int colRows[POOL];
    int colSize[NCOLS];
    unsigned char colActive[NCOLS];
    unsigned char rowActive[NROWS];
    Op ops[MAX_OPS];
    int opsTop;
    int solRows[81];
    int solCount;
    int activeCols;
} SudokuCtx;
```

* `rowToCols[729][4]` — for each candidate `(r,c,d)`, the four constraint column indices it covers.
  **Why:** lets us quickly find which columns to cover for a chosen row.
* `colStart[325]` — prefix offsets for each column’s row list in `colRows` (CSC‑style layout).
  **Why:** gives O(1) access to a column’s contiguous rows.
* `colRows[2916]` — the flattened list of candidate rows grouped by column (all matrix ones).
  **Why:** stores the incidence matrix sparsely and cache‑friendly.
* `colSize[324]` — dynamic count of *currently active* rows per column.
  **Why:** supports the **MRV heuristic** (choose smallest column).
* `colActive[324]` — whether a constraint (column) is still uncovered.
* `rowActive[729]` — whether a candidate row is still available.
* `ops[MAX_OPS]`, `opsTop` — reversible operation stack pointer.
* `solRows[81]`, `solCount` — the chosen candidate rows forming the solution.
* `activeCols` — number of columns still uncovered (diagnostic/optional).

**Algorithms referenced:** Compressed Sparse Column (CSC) layout for the incidence matrix; MRV; reversible logging.

---

## 4) Indexer Helpers (Mathematical to Linear Mapping)

```c
inline int col_cell(int r,int c){ return r*9 + c; }
inline int col_rowdig(int r,int d){ return 81 + r*9 + d; }
inline int col_coldig(int c,int d){ return 162 + c*9 + d; }
inline int box_of(int r,int c){ return (r/3)*3 + (c/3); }
inline int col_boxdig(int b,int d){ return 243 + b*9 + d; }
inline int row_idx(int r,int c,int d0){ return r*81 + c*9 + d0; }
```

Maps 2D/3D Sudoku concepts to **contiguous integer IDs**:

* `col_cell(r,c)` — 0..80: cell constraint for `(r,c)`.
* `col_rowdig(r,d)` — 81..161: row‑digit constraint.
* `col_coldig(c,d)` — 162..242: col‑digit constraint.
* `box_of(r,c)` — 0..8: which 3×3 box contains `(r,c)`.
* `col_boxdig(b,d)` — 243..323: box‑digit constraint.
* `row_idx(r,c,d)` — 0..728: candidate row index for placing digit `d+1` at `(r,c)`.

**Algorithms referenced:** Exact‑Cover matrix indexing (bijections between tuples and linear indices).

---

## 5) Covering a Constraint Column

```c
void coverColumn(SudokuCtx* ctx,int col){
    if (!ctx->colActive[col]) return;
    ctx->colActive[col] = 0; ctx->activeCols--;
    ctx->ops[ctx->opsTop++] = (Op){2,(unsigned short)col};
    int s = ctx->colStart[col], e = ctx->colStart[col+1];
    for (int i = s; i < e; ++i) {
        int r = ctx->colRows[i];
        if (ctx->rowActive[r]) {
            ctx->rowActive[r] = 0;
            ctx->ops[ctx->opsTop++] = (Op){0,(unsigned short)r};
            int *cs = ctx->rowToCols[r];
            for (int t = 0; t < 4; ++t) {
                int c2 = cs[t];
                if (c2 != col && ctx->colActive[c2]) {
                    ctx->colSize[c2]--;
                    ctx->ops[ctx->opsTop++] = (Op){1,(unsigned short)c2};
                }
            }
        }
    }
}
```

**Line‑by‑line purpose and algorithm:**

* Guard: `if (!colActive[col]) return;` — idempotence (avoid double cover).
* `colActive[col]=0`, `activeCols--` — mark column as **covered**.
* Push op `{2,col}` — record a `COL_UNCOVER` to reverse later.
* Get segment `[s,e)` — the rows that have a 1 in this column.
* Loop rows `r` in the column:

  * If row is active, **disable** it (`rowActive[r]=0`) and log `{0,r}` (`ROW_ENABLE` for undo).
  * For the other 3 columns `c2` that the row touches:

    * If still active, decrement `colSize[c2]` and log `{1,c2}` (`COL_SIZE_INC` for undo).

**Algorithms referenced:** Algorithm X “cover” operation; constraint propagation; reversible logging (undo stack).

---

## 6) Uncover Back to a Mark

```c
void uncoverTo(SudokuCtx* ctx,int mark){
    while (ctx->opsTop > mark) {
        Op op = ctx->ops[--ctx->opsTop];
        switch (op.kind) {
            case 1: ctx->colSize[op.id]++; break;
            case 0: ctx->rowActive[op.id] = 1; break;
            case 2: ctx->colActive[op.id] = 1; ctx->activeCols++; break;
        }
    }
}
```

* Pop operations in **LIFO** order until reaching `mark`.
* `kind==1` → undo a size decrement; `kind==0` → re‑enable a row; `kind==2` → re‑activate a column.

**Algorithms referenced:** Backtracking with precise state reversal (the Algorithm X “uncover” counterpart). Ensures the search tree is traversed without corruption.

---

## 7) Choose a Candidate Row (Commit a Placement)

```c
void chooseRow(SudokuCtx* ctx,int row){
    int *cs = ctx->rowToCols[row];
    for (int t = 0; t < 4; ++t) if (ctx->colActive[cs[t]]) coverColumn(ctx,cs[t]);
}
```

* Fetch the four columns this row satisfies.
* **Cover each column** (if still active). This enforces exclusivity: once we place `(r,c,d)`, the cell is filled and `d` cannot appear elsewhere in that row/column/box.

**Algorithms referenced:** Algorithm X row choice (cover all columns having 1s in the chosen row).

---

## 8) Build the Sparse Incidence Structure

```c
void build_matrix(SudokuCtx* ctx){
    int counts[NCOLS]; memset(counts, 0, sizeof(counts));
    for (int r = 0; r < 9; ++r) for (int c = 0; c < 9; ++c) for (int d0 = 0; d0 < 9; ++d0) {
        int row = row_idx(r,c,d0);
        int b = box_of(r,c);
        int c0 = col_cell(r,c);
        int c1 = col_rowdig(r,d0);
        int c2 = col_coldig(c,d0);
        int c3 = col_boxdig(b,d0);
        ctx->rowToCols[row][0] = c0;
        ctx->rowToCols[row][1] = c1;
        ctx->rowToCols[row][2] = c2;
        ctx->rowToCols[row][3] = c3;
        counts[c0]++; counts[c1]++; counts[c2]++; counts[c3]++;
    }
    int acc = 0;
    for (int c = 0; c < NCOLS; ++c) {
        ctx->colStart[c] = acc;
        acc += counts[c];
        ctx->colSize[c] = counts[c];
        ctx->colActive[c] = 1;
    }
    ctx->colStart[NCOLS] = acc;
    int next[NCOLS]; memcpy(next, ctx->colStart, sizeof(next));
    for (int r = 0; r < 9; ++r) for (int c = 0; c < 9; ++c) for (int d0 = 0; d0 < 9; ++d0) {
        int row = row_idx(r,c,d0);
        for (int t = 0; t < 4; ++t) {
            int col = ctx->rowToCols[row][t];
            ctx->colRows[next[col]++] = row;
        }
    }
    memset(ctx->rowActive, 1, sizeof(ctx->rowActive));
    ctx->opsTop = 0;
    ctx->solCount = 0;
    ctx->activeCols = NCOLS;
}
```

**Outer triple loop (enumeration):**

* Enumerate all `(r,c,d)` candidates; compute the four constraint columns.
* Store them in `rowToCols[row][0..3]`.
* Increment `counts[col]` for each column to know future segment sizes.

**Prefix build:**

* Accumulate `counts` into `colStart` via running total `acc`. This lays out **contiguous segments** in `colRows` (CSC format).
* Initialize `colSize[c]=counts[c]` and mark all columns active.
* `colStart[NCOLS] = acc` — sentinel end.

**Second triple loop (scatter fill):**

* `next[col]` starts at each column’s segment begin; for every candidate row, append its row index into each of its four columns: `colRows[next[col]++] = row`.

**Flags & counters:**

* `memset(rowActive,1,...)` — mark all rows active (byte value `1`).
* Reset stacks and counters.

**Algorithms referenced:** Sparse matrix construction (CSC), exact‑cover incidence generation.

---

## 9) Recursive Search (Algorithm X + MRV)

```c
bool search(SudokuCtx* ctx){
    int bestCol = -1, bestSz = 1<<30;
    for (int c = 0; c < NCOLS; ++c) if (ctx->colActive[c]) {
        int sz = ctx->colSize[c];
        if (sz < bestSz) { bestSz = sz; bestCol = c; if (sz <= 1) break; }
    }
    if (bestCol == -1) return true;
    if (bestSz == 0) return false;
    int s = ctx->colStart[bestCol], e = ctx->colStart[bestCol+1];
    for (int i = s; i < e; ++i) {
        int row = ctx->colRows[i];
        if (!ctx->rowActive[row]) continue;
        int mark = ctx->opsTop;
        int prevSol = ctx->solCount;
        chooseRow(ctx,row);
        ctx->solRows[ctx->solCount++] = row;
        if (search(ctx)) return true;
        ctx->solCount = prevSol;
        uncoverTo(ctx,mark);
    }
    return false;
}
```

**Column selection (MRV):**

* Scan active columns; pick the one with minimal `colSize`. Early‑exit when size ≤ 1.
  **MRV heuristic** reduces branching.

**Base cases:**

* `bestCol == -1` — no active columns remain ⇒ all constraints covered ⇒ **solution**.
* `bestSz == 0` — selected column has no available rows ⇒ **dead end**.

**Row iteration:**

* Retrieve the `[s,e)` segment of rows for `bestCol`.
* Skip inactive rows.
* Save `mark` (stack top) and `prevSol` for precise backtracking.
* `chooseRow` to cover its four columns, append to `solRows`, recurse.
* On failure: restore `solCount` and `uncoverTo(mark)` to revert all effects.
* If none succeed: return `false`.

**Algorithms referenced:** Algorithm X recursion; MRV; reversible backtracking.

---

## 10) Public Entry Point and Reconstruction

```c
void solveSudoku(char** board, int boardSize, int* boardColSize){
    SudokuCtx ctx;
    build_matrix(&ctx);
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            char ch = board[r][c];
            if (ch != '.') {
                int d0 = ch - '1';
                int row = row_idx(r,c,d0);
                chooseRow(&ctx,row);
                ctx->solRows[ctx->solCount++] = row;
            }
        }
    }
    search(&ctx);
    for (int i = 0; i < ctx.solCount; ++i) {
        int row = ctx.solRows[i];
        int rr = row / 81;
        int rem = row % 81;
        int cc = rem / 9;
        int d0 = rem % 9;
        board[rr][cc] = (char)('1' + d0);
    }
}
```

**Context allocation:** stack‑local `ctx` (no globals/statics).

**Matrix build:** construct the sparse incidence structure.

**Apply givens:**

* Iterate cells; for each pre‑filled `ch`, compute `d0 = ch - '1'` (digits 1..9 → 0..8), get `rowIdx`.
* `chooseRow` to enforce that given, and push it into `solRows` (it’s part of any valid solution).

**Search:** run `search(&ctx)`; the leetcode puzzle guarantees solvability (unique solution).

**Reconstruction:**

* Iterate `solRows[0..solCount)` and decode `(rr,cc,d0)` from the linear row index.
* Write the solved digit `('1'+d0)` back to `board[rr][cc]` in‑place.

**Algorithms referenced:** Initialization; applying forced choices; final decoding from exact‑cover rows to grid.

---

## 11) Algorithmic Summary per Function/Line Group

* **Indexers (`col_*`, `row_idx`)** — bijections turning tuples into offsets (array‑level view of the exact‑cover structure).
* **`build_matrix`** — constructs a **CSC** representation of the 0–1 incidence matrix; O(729) enumeration + O(2916) inserts.
* **`coverColumn` / `uncoverTo`** — Algorithm X’s cover/uncover with a compact **undo log** (operations: enable row, inc column size, uncover column).
* **`chooseRow`** — cover the four columns of a chosen candidate (propagates Sudoku constraints immediately).
* **`search`** — Algorithm X recursion + **MRV** heuristic on `colSize`.
* **`solveSudoku`** — apply givens, call search, and rebuild the solution grid.

---

## 12) Invariants and Correctness Hints

* Each active column `C` has `colRows[colStart[C]..colStart[C+1])` as its immutable roster; `colSize[C]` reflects the count of those whose rows are currently active.
* `rowActive[R]==0` implies all four columns of `R` had their sizes decremented exactly once since `R` was disabled.
* The op‑stack strictly reverses effects (LIFO). Pushed kinds map to exact reversals:

  * `2` → `COL_UNCOVER`: re‑enables a column and increments `activeCols`.
  * `0` → `ROW_ENABLE`: re‑enables a candidate row.
  * `1` → `COL_SIZE_INC`: restores a column’s size counter.
* Base cases in `search` ensure termination: either all constraints covered or a contradiction found.

---

## 13) Complexity Notes

* **Matrix build:** linear in number of candidates and ones (≈ few thousands ops).
* **Search:** exponential in worst case, but MRV + givens make typical instances fast.
* **Space:** dominated by arrays sized from fixed constants (fits comfortably in standard OJ limits).

---

## 14) What Algorithms Are Used Where

* **Exact‑Cover modeling** — everywhere the four constraint families and candidate mapping are used.
* **Algorithm X** — `search`, `coverColumn`, `uncoverTo`, `chooseRow`.
* **MRV heuristic** — the `bestCol` selection loop using `colSize`.
* **CSC sparse layout** — `colStart`, `colRows`, and related build in `build_matrix`.
* **Reversible backtracking (undo log)** — the `ops` stack and `uncoverTo`.

This concludes the line‑by‑line, algorithm‑annotated walkthrough of the provided code.

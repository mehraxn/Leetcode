#include <string.h>
#include <stdbool.h>

#define N 9
#define NROWS (N*N*N)
#define NCOLS (N*N*4)
#define POOL (NROWS*4)
#define MAX_OPS 24000

typedef struct { unsigned char kind; unsigned short id; } Op;

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

inline int col_cell(int r,int c){ return r*9 + c; }
inline int col_rowdig(int r,int d){ return 81 + r*9 + d; }
inline int col_coldig(int c,int d){ return 162 + c*9 + d; }
inline int box_of(int r,int c){ return (r/3)*3 + (c/3); }
inline int col_boxdig(int b,int d){ return 243 + b*9 + d; }
inline int row_idx(int r,int c,int d0){ return r*81 + c*9 + d0; }

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

void chooseRow(SudokuCtx* ctx,int row){
    int *cs = ctx->rowToCols[row];
    for (int t = 0; t < 4; ++t) if (ctx->colActive[cs[t]]) coverColumn(ctx,cs[t]);
}

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
                ctx.solRows[ctx.solCount++] = row;
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

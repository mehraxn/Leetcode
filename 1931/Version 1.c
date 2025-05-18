#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MOD 1000000007
#define MAX_PAT 243
#define MAX_M     5

typedef struct {
    int code;
    int row[MAX_M];
} Pattern;

int colorTheGrid(int m, int n)
{
    Pattern pat[MAX_PAT];
    int patCnt = 0;
    int maxCode = 1;
    for (int i = 0; i < m; ++i) maxCode *= 3;

    for (int code = 0; code < maxCode; ++code) {
        int tmp = code, ok = 1;
        int rows[MAX_M];
        for (int r = 0; r < m; ++r) {
            rows[r] = tmp % 3;
            if (r && rows[r] == rows[r - 1]) { ok = 0; break; }
            tmp /= 3;
        }
        if (!ok) continue;
        pat[patCnt].code = code;
        memcpy(pat[patCnt].row, rows, m * sizeof(int));
        ++patCnt;
    }

    static char compat[MAX_PAT][MAX_PAT];
    for (int i = 0; i < patCnt; ++i) {
        for (int j = 0; j < patCnt; ++j) {
            char ok = 1;
            for (int r = 0; r < m; ++r) {
                if (pat[i].row[r] == pat[j].row[r]) { ok = 0; break; }
            }
            compat[i][j] = ok;
        }
    }

    static int64_t dp[MAX_PAT], ndp[MAX_PAT];
    for (int i = 0; i < patCnt; ++i) dp[i] = 1;

    for (int col = 1; col < n; ++col) {
        for (int j = 0; j < patCnt; ++j) ndp[j] = 0;
        for (int prev = 0; prev < patCnt; ++prev)
            for (int next = 0; next < patCnt; ++next)
                if (compat[prev][next]) {
                    ndp[next] += dp[prev];
                    if (ndp[next] >= MOD) ndp[next] -= MOD;
                }
        memcpy(dp, ndp, patCnt * sizeof(int64_t));
    }

    long long ans = 0;
    for (int i = 0; i < patCnt; ++i) {
        ans += dp[i];
        if (ans >= MOD) ans -= MOD;
    }
    return (int)ans;
}

int main() {
    printf("%d\n", colorTheGrid(1, 1));
    printf("%d\n", colorTheGrid(1, 2));
    printf("%d\n", colorTheGrid(5, 5));
    return 0;
}

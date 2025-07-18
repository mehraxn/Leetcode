#include <stdlib.h>

int maximumLength(int* nums, int numsSize, int k) {
    int *dp = calloc(k * k, sizeof(int));
    int *freq = calloc(k, sizeof(int));
    int *new_dp = malloc(k * sizeof(int));
    int ans = 1;
    for (int i = 0; i < numsSize; i++) {
        int m = nums[i] % k;
        for (int r = 0; r < k; r++) {
            new_dp[r] = dp[r * k + m];
        }
        for (int r = 0; r < k; r++) {
            int c = (r - m + k) % k;
            int idx = r * k + c;
            if (dp[idx] > 0) {
                int val = dp[idx] + 1;
                if (val > new_dp[r]) new_dp[r] = val;
            }
        }
        for (int c = 0; c < k; c++) {
            if (freq[c] > 0) {
                int r2 = (c + m) % k;
                if (new_dp[r2] < 2) new_dp[r2] = 2;
            }
        }
        for (int r = 0; r < k; r++) {
            int idx2 = r * k + m;
            dp[idx2] = new_dp[r];
            if (dp[idx2] > ans) ans = dp[idx2];
        }
        freq[m]++;
    }
    free(dp);
    free(freq);
    free(new_dp);
    return ans;
}

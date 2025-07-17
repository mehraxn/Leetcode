#include <stdio.h>
#include <stdlib.h>

void count_validation(int *input, int input_size, int *final, int *final_size, int k);

void arr(int *val, int *sol, int *mark, int n, int target_len, int pos, int start, int *final_array, int *final_counter, int modk) {
    if (pos >= target_len) {
        count_validation(sol, pos, final_array, final_counter, modk);
        return;
    }

    for (int i = start; i < n; i++) {
        if (mark[i] == 0) {
            mark[i] = 1;
            sol[pos] = val[i];
            arr(val, sol, mark, n, target_len, pos + 1, i + 1, final_array, final_counter, modk);
            mark[i] = 0;
        }
    }
}

int maximumLength(int *nums, int numsSize, int k) {
    int *sol = malloc(numsSize * sizeof(int));
    int *mark = calloc(numsSize, 1);
    int *best_subseq = malloc(numsSize * sizeof(int));
    int best_len = 0;

    for (int len = 1; len <= numsSize; len++) {
        arr(nums, sol, mark, numsSize, len, 0, 0, best_subseq, &best_len, k);
    }

    free(sol);
    free(mark);
    free(best_subseq);
    return best_len;
}

void count_validation(int *input, int input_size, int *final, int *final_size, int k) {
    if (input_size < 2) return;

    int mod = (input[0] + input[1]) % k;
    for (int i = 1; i < input_size - 1; i++) {
        if ((input[i] + input[i + 1]) % k != mod)
            return;
    }

    if (*final_size < input_size) {
        for (int i = 0; i < input_size; i++)
            final[i] = input[i];
        *final_size = input_size;
    }
}

#include <stdio.h>
#include <stdlib.h>

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

long long countFairPairs(int* nums, int numsSize, int lower, int upper) {

    int* sorted = (int*)malloc(numsSize * sizeof(int));
    for (int i = 0; i < numsSize; i++) {
        sorted[i] = nums[i];
    }

    qsort(sorted, numsSize, sizeof(int), compare);

    long long count = 0;

    for (int i = 0; i < numsSize - 1; i++) {
        int target_lower = lower - sorted[i];
        int target_upper = upper - sorted[i];

        int low = i + 1;
        int high = numsSize - 1;
        int first_valid = numsSize;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (sorted[mid] >= target_lower) {
                first_valid = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        low = i + 1;
        high = numsSize - 1;
        int last_valid = i;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (sorted[mid] <= target_upper) {
                last_valid = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        // Add the count of valid pairs with this element
        if (first_valid <= last_valid) {
            count += (last_valid - first_valid + 1);
        }
    }

    free(sorted);
    return count;
}


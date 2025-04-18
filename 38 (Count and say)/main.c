#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void reverse(char str[], int length) {
    int start = 0, end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* intToString(int num) {
    char* str = (char*)malloc(12);
    if (str == NULL) return NULL;

    int i = 0;
    bool isNegative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0) {
        isNegative = true;
        if (num == -2147483648) {
            char minInt[] = "-2147483648";
            for (i = 0; minInt[i]; i++) {
                str[i] = minInt[i];
            }
            str[i] = '\0';
            return str;
        }
        num = -num;
    }

    while (num != 0) {
        int digit = num % 10;
        str[i++] = digit + '0';
        num /= 10;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';
    reverse(str, i);
    return str;
}

char* countAndSay(int n) {
    char* result = (char*)malloc(5000);
    strcpy(result, "1");

    for (int i = 1; i < n; i++) {
        char* current = result;
        char* next = (char*)malloc(5000);
        int pos = 0;

        for (int j = 0; current[j] != '\0';) {
            char digit = current[j];
            int count = 1;
            while (current[j] == current[j + 1]) {
                count++;
                j++;
            }
            char* countStr = intToString(count);
            int k = 0;
            while (countStr[k] != '\0') {
                next[pos++] = countStr[k++];
            }
            next[pos++] = digit;
            free(countStr);
            j++;
        }

        next[pos] = '\0';
        strcpy(result, next);
        free(next);
    }

    return result;
}

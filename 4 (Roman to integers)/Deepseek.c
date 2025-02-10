#include <stdio.h>
#include <stdlib.h>

int peripheral(char input) {
    switch(input) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

int romanToInt(char* s) {
    int sum = 0;
    int prev = 0;
    while (*s) {
        int current = peripheral(*s);
        sum += current;
        if (current > prev) {
            sum -= 2 * prev;
        }
        prev = current;
        s++;
    }
    return sum;
}
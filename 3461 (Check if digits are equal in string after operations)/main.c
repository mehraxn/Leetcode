#include <string.h>

bool hasSameDigits(char* s) {
    int length = strlen(s);
    char temp[length];

    while (length != 2) {
        for (int i = 0; i < length - 1; i++) {
            temp[i] = ((s[i] - '0' + s[i+1] - '0') % 10 + '0');
        }
        for (int i = 0; i < length - 1; i++) {
            s[i] = temp[i];
        }
        length--;
    }

    return s[0] == s[1];
}
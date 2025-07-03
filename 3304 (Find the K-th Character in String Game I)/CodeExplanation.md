# README: kthCharacter Game Explanation

## Problem Summary

Alice and Bob are playing a game where Alice starts with a string `word = "a"`. In each round, Alice performs the following operation:

* Generate a new string by converting each character in the current string to its next alphabetical character (wrapping around from 'z' to 'a').
* Append this newly generated string to the original.

For example:

* Round 1: "a" → next is "b" → word becomes "ab"
* Round 2: "ab" → next is "bc" → word becomes "abbc"
* Round 3: "abbc" → next is "bccd" → word becomes "abbcbccd"

Given a number `k`, the task is to return the `k`th character in the resulting string after enough operations that ensure the length of the string is at least `k`.

## Code Implementation

```cpp
char kthCharacter(int k) {
    int l = 1;
    while (l < k) l *= 2;

    char c = 'a';

    while (l > 1) {
        l /= 2;
        if (k > l) {
            k -= l;
            c = (c - 'a' + 1) % 26 + 'a';
        }
    }

    return c;
}
```

## Step-by-Step Explanation

### Step 1: Initialize `l` to the smallest power of 2 >= `k`

```cpp
int l = 1;
while (l < k) l *= 2;
```

* This loop finds the smallest power of 2 that is greater than or equal to `k`.
* This value `l` simulates the length of the string after a sufficient number of operations.
* For example, if `k = 10`, the values of `l` will go: 1, 2, 4, 8, 16 (since 16 >= 10).

### Step 2: Initialize character to `'a'`

```cpp
char c = 'a';
```

* Start from the first character in the original string, which is always `'a'`.

### Step 3: Simulate the recursive character tracking

```cpp
while (l > 1) {
    l /= 2;
    if (k > l) {
        k -= l;
        c = (c - 'a' + 1) % 26 + 'a';
    }
}
```

* Here, we simulate traversing a **binary tree structure** where each level doubles the string length.
* We split the current segment of the string in half (just like recursively dividing the sequence).
* If `k` is in the second half (`k > l` after division), then:

  * Subtract the first half size from `k` (i.e., move the index into the second half).
  * Increment the character `c` to its next alphabet (using modulo 26 to wrap 'z' to 'a').
* Otherwise, `k` lies in the first half, and we continue with the current character.

#### Why this works?

The trick is to recognize that every operation essentially builds a binary tree of transformations:

* `"a"` becomes `"ab"`
* `"ab"` becomes `"abbc"`
* `"abbc"` becomes `"abbcbccd"`
* The left part always stays the same.
* The right part is a transformation of the left, i.e., each character +1 in alphabet.
* Thus, you can traverse this structure recursively (or iteratively in this case) to find the `k`th character without building the entire string.

### Step 4: Return the result

```cpp
return c;
```

* After narrowing down through recursive halves, we return the character `c` which is the value at position `k`.

## Time Complexity

* O(log k): Each iteration divides `l` by 2, so the number of iterations is proportional to `log k`.
* This is extremely efficient even for the upper limit `k = 500`.

## Examples

### Example 1:

```cpp
k = 5
```

* Binary search path: 1 → 2 → 4 → 8
* Final result: `'b'`

### Example 2:

```cpp
k = 10
```

* Path: 1 → 2 → 4 → 8 → 16
* Final result: `'c'`

## Final Notes

This solution cleverly avoids full string simulation and utilizes a binary tree-like reduction combined with modular arithmetic to efficiently determine the character at a specific position.

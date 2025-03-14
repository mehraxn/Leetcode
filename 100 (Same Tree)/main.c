#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Definition for a binary tree node.
struct TreeNode {
    int val;                    // Stores the value of the node
    struct TreeNode *left;       // Pointer to the left child node
    struct TreeNode *right;      // Pointer to the right child node
};

// Preorder traversal function to compare two trees
void pre_order(struct TreeNode* p, struct TreeNode* q, int *flag) {
    // If both nodes are NULL, return (they are identical at this point)
    if ((p == NULL) && (q == NULL))
        return;

    // If one is NULL and the other is not, trees are not identical
    if ((p == NULL) && (q != NULL)) {
        (*flag) = 0;
        return;
    }
    if ((q == NULL) && (p != NULL)) {
        (*flag) = 0;
        return;
    }

    // If values of the current nodes do not match, trees are not identical
    if (p->val != q->val)
        (*flag) = 0;

    // Recursively check left subtrees
    pre_order(p->left, q->left, flag);
    // Recursively check right subtrees
    pre_order(p->right, q->right, flag);

    return;
}

// Function to check if two binary trees are the same
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    int flag = 1; // Assume trees are identical initially
    pre_order(p, q, &flag); // Call preorder traversal to compare trees

    // If flag is set to 0, trees are not identical
    if (flag == 0)
        return false;
    else
        return true;
}


#include <stdio.h>
#include <stdlib.h>

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

int maxDepth(struct TreeNode* root) {
    int l, r;
    if (root == NULL)
        return 0;
    l = maxDepth(root->left);
    r = maxDepth(root->right);
    return (l > r) ? (l + 1) : (r + 1);
}

struct TreeNode* newNode(int val) {
    struct TreeNode* node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int main() {
    struct TreeNode* root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);

    int depth = maxDepth(root);
    printf("Maximum Depth of the Tree: %d\n", depth);

    return 0;
}

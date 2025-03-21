#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct TreeNode{
    int val ;
    struct TreeNode *left;
    struct TreeNode * right;
};

void check(struct TreeNode * root,int target,int * flag);

void pre_order(struct TreeNode *root,int target,int *flag){
    if (root==NULL)
        return;

    check(root,target,flag);

    return;
}


void check(struct TreeNode *root, int target, int *flag) {
    if (root == NULL)
        return;
    else if (root->val == target && root->left == NULL && root->right == NULL) {
        *flag = 1;
        return;
    }
    check(root->left, target - root->val, flag);
    check(root->right, target - root->val, flag);
}

bool hasPathSum(struct TreeNode* root, int targetSum) {
    int flag=0;
    pre_order(root,targetSum,&flag);

    if(flag==1)
        return true;
    else
        return false;

}
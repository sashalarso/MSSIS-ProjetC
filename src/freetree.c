#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "table.h"

void freeTree(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    free(root->hash);
    free(root->clair);
    free(root);
}

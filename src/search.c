#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "table.h"

void searchInTree(TreeNode *root, const char *target_hash) {
    if (root == NULL) {
        return;
    }

    int compare = strcmp(target_hash, root->hash);
    if (compare == 0) {
        printf("MATCH %s %s\n", target_hash, root->clair);
    } else if (compare < 0) {
        searchInTree(root->left, target_hash);
    } else {
        searchInTree(root->right, target_hash);
    }
}

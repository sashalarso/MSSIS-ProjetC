#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "table.h"

void insert(TreeNode **root, const char *hash, const char *clair) {
    if (*root == NULL) {
        *root = (TreeNode *)malloc(sizeof(TreeNode));
        (*root)->hash = strdup(hash);
        (*root)->clair = strdup(clair);
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else {
        int compare = strcmp(hash, (*root)->hash);
        if (compare < 0) {
            insert(&((*root)->left), hash, clair);
        } else if (compare > 0) {
            insert(&((*root)->right), hash, clair);
        }
    }
}

#include <stdio.h>
#include <string.h>
#include "table.h"

void lookup_in_table(const char *target_hash, TreeNode *root) {
    searchInTree(root, target_hash);
}
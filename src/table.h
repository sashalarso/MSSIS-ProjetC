#ifndef TABLE_H
#define TABLE_H

typedef struct TreeNode {
    char *hash;
    char *clair;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

void insert(TreeNode **root, const char *hash, const char *clair);
void searchInTree(TreeNode *root, const char *target_hash);
void freeTree(TreeNode *root);
TreeNode* load_table_from_file(const char *filename);
void sha256_hash_string(const char *string, unsigned char *output);

#endif

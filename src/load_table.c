#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"


// Fonction pour charger la table de correspondance chaîne-condensat (T3C) depuis un fichier
TreeNode* load_table_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier T3C");
        return NULL;
    }

    char line[256];
    TreeNode *root = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *hash = strtok(line, " -> ");
        char *clair = strtok(NULL, " -> ");

        if (hash != NULL && clair != NULL) {
            insert(&root, hash, clair);
        }
    }

    fclose(file);
    return root;
}
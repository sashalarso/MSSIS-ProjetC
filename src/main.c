#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include "table.h"

// Fonction pour générer un hachage SHA-256



int main(int argc, char *argv[]) {
    TreeNode *root = NULL;

    if ( argc != 3) {
        fprintf(stderr, "Utilisation : %s -G  <fichier_T3C> OU %s -L <fichier_T3C>\n", argv[0], argv[0]);
        return 1;
    }

    if (argc == 3 && strcmp(argv[1], "-G") == 0) {
        // Mode G (génération de condensats)
        const char *output_file = argv[2];

        FILE *output = fopen(output_file, "w");
        if (output == NULL) {
            perror("Erreur lors de l'ouverture du fichier de sortie T3C");
            return 1;
        }

        char line[256];
        int j = 0;
        while (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0';
            unsigned char sha256_hash[EVP_MAX_MD_SIZE];
            sha256_hash_string(line, sha256_hash);

            j++;
            if (j % 10000 == 0) {
                printf("INFO %d hashes written \n", j);
            }
            for (int i = 0; i < EVP_MD_size(EVP_get_digestbyname("sha256")); i++) {
                fprintf(output, "%02x", sha256_hash[i]);
            }
            fprintf(output, " -> %s ", line);
            fprintf(output, "\n");
        }

        fclose(output);

        printf("Génération de condensats terminée. Les résultats ont été écrits dans %s.\n", output_file);
    } else if (argc == 3 && strcmp(argv[1], "-L") == 0) {
        // Mode L (recherche parmi une liste de condensats)
        root = load_table_from_file(argv[2]);
        
        if (root) {
            char hash[256];
            while (fgets(hash, sizeof(hash), stdin)) {
                hash[strcspn(hash, "\n")] = '\0';
                lookup_in_table(hash, root);
            }
            freeTree(root);
        }
    } else {
        fprintf(stderr, "Mode invalide. Utilisation : G (generate) OU L (lookup)\n");
        return 1;
    }

    return 0;
}


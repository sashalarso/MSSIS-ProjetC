#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

// Fonction pour générer un hachage SHA-256
void sha256_hash_string(const char *string, unsigned char *output) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;

    OpenSSL_add_all_digests();
    md = EVP_get_digestbyname("sha256");
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, string, strlen(string));
    EVP_DigestFinal_ex(mdctx, output, NULL);
    EVP_MD_CTX_free(mdctx);
    EVP_cleanup();
}

// Fonction pour charger la table de correspondance chaîne-condensat (T3C) depuis un fichier
int load_table_from_file(const char *filename, char ***table, int *num_entries) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier T3C");
        return 0;
    }

    char line[256];
    int max_entries = 100; // Un nombre arbitraire initial de lignes
    *table = (char **)malloc(max_entries * sizeof(char *));
    if (*table == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        fclose(file);
        return 0;
    }

    *num_entries = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        (*table)[(*num_entries)++] = strdup(line);

        if (*num_entries >= max_entries) {
            // Réallouer la mémoire pour plus de lignes si nécessaire
            max_entries *= 2;
            *table = (char **)realloc(*table, max_entries * sizeof(char *));
            if (*table == NULL) {
                perror("Erreur lors de la réallocation de mémoire");
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return 1;
}

// Fonction pour rechercher une chaîne parmi les condensats dans la table T3C
int lookup_in_table(const char *target_hash, char **table, int num_entries, char **found_strings) {
    int found = 0;

    for (int i = 0; i < num_entries; i++) {
        char *line = table[i];
        char *chaine = strtok(line, " -> ");
        char *condensat = strtok(NULL, " -> ");

        

        if (condensat != NULL && chaine != NULL && strcmp(condensat, target_hash) == 0) {
            
            found = 1;
            found_strings[i] = strdup(chaine);
            printf("%s",found_strings[i]);
        }
    }

    return found;
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 2) {
        fprintf(stderr, "Utilisation : %s G <fichier_dictionnaire> <fichier_T3C> OU %s L <fichier_T3C>\n", argv[0], argv[0]);
        return 1;
    }

    if (argc == 4 && strcmp(argv[1], "G") == 0) {
        // Mode G (génération de condensats)
        const char *input_file = argv[2];
        const char *output_file = argv[3];

        FILE *input = fopen(input_file, "r");
        if (input == NULL) {
            perror("Erreur lors de l'ouverture du fichier dictionnaire");
            return 1;
        }

        FILE *output = fopen(output_file, "w");
        if (output == NULL) {
            perror("Erreur lors de l'ouverture du fichier de sortie T3C");
            fclose(input);
            return 1;
        }

        char line[256];
        while (fgets(line, sizeof(line), input)) {
            line[strcspn(line, "\n")] = '\0'; // Supprimer le caractère de saut de ligne
            unsigned char sha256_hash[EVP_MAX_MD_SIZE]; // Utilisation de EVP_MAX_MD_SIZE

            sha256_hash_string(line, sha256_hash);

            fprintf(output, "%s -> ", line);
            for (int i = 0; i < EVP_MD_size(EVP_get_digestbyname("sha256")); i++) {
                fprintf(output, "%02x", sha256_hash[i]);
            }
            fprintf(output, "\n");
        }

        fclose(input);
        fclose(output);

        printf("Génération de condensats terminée. Les résultats ont été écrits dans %s.\n", output_file);
    } else if (argc == 2 && strcmp(argv[1], "L") == 0) {
        // Mode L (recherche parmi une liste de condensats)
        char **table = NULL;
        int num_entries = 0;

        if (!load_table_from_file("table_de_correspondance.txt", &table, &num_entries)) {
            fprintf(stderr, "Erreur lors du chargement de la table T3C.\n");
            return 1;
        }

        char target_hash[65]; // Pour stocker le condensat cible
        printf("Entrez le condensat à rechercher (en hexadécimal) : ");
        if (scanf("%64s", target_hash) != 1) {
            fprintf(stderr, "Erreur lors de la saisie du condensat.\n");
            return 1;
        }

        char *found_strings[num_entries];
        for (int i = 0; i < num_entries; i++) {
            found_strings[i] = NULL; // Initialize to NULL
        }
        int found = lookup_in_table(target_hash, table, num_entries, found_strings);

        if (found) {
            printf("Chaîne(s) correspondante(s) trouvée(s) :\n");
            printf("%s\n", found_strings[0]);
            for (int i = 0; i < num_entries; i++) {
                if (found_strings[i] != NULL) {
                    printf("%s\n", found_strings[i]);
                    free(found_strings[i]);
                }
            }
        } else {
            printf("Aucune correspondance trouvée pour le condensat spécifié.\n");
        }

        // Libérer la mémoire allouée
        for (int i = 0; i < num_entries; i++) {
            free(table[i]);
        }
        free(table);
    } else {
        fprintf(stderr, "Mode invalide. Utilisation : G (generate) OU L (lookup)\n");
        return 1;
    }

    return 0;
}

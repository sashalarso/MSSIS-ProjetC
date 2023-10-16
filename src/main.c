#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

// Fonction pour générer un hachage SHA-256

typedef struct {
    char *hash;    // Le condensat (hash)
    char *chaine;  // La chaîne claire associée
} Entry;

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
int load_table_from_file(const char *filename, Entry **table, int *num_entries) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier T3C");
        return 0;
    }

    char line[256];
    int max_entries = 100000000; // Un nombre arbitraire initial de lignes
    *table = (Entry *)malloc(max_entries * sizeof(Entry));
    if (*table == NULL) {
        perror("Erreur lors de l'allocation de mémoire");
        fclose(file);
        return 0;
    }

    *num_entries = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        char *hash = strtok(line, " -> ");
        char *chaine = strtok(NULL, " -> ");

        if (hash != NULL && chaine != NULL) {
            (*table)[*num_entries].hash = strdup(hash);
            (*table)[*num_entries].chaine = strdup(chaine);
            (*num_entries)++;

            if (*num_entries >= max_entries) {
                // Réallouer la mémoire pour plus d'entrées si nécessaire
                max_entries *= 2;
                *table = (Entry *)realloc(*table, max_entries * sizeof(Entry));
                if (*table == NULL) {
                    perror("Erreur lors de la réallocation de mémoire");
                    fclose(file);
                    return 0;
                }
            }
        }
    }

    fclose(file);
    return 1;
}

void lookup_in_table(const char *target_hash, Entry *table, int num_entries) {
    for (int i = 0; i < num_entries; i++) {
        if (strcmp(target_hash, table[i].hash) == 0) {
            printf("MATCH %s %s\n", target_hash, table[i].chaine);
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 2 && argc !=3) {
        fprintf(stderr, "Utilisation : %s -G <fichier_dictionnaire> <fichier_T3C> OU %s -L <fichier_T3C>\n", argv[0], argv[0]);
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
        int j=0;
        while (fgets(line, sizeof(line), stdin)) {
            line[strcspn(line, "\n")] = '\0'; 
            unsigned char sha256_hash[EVP_MAX_MD_SIZE]; 

            sha256_hash_string(line, sha256_hash);

            
            j++;
            if (j%50==0){
        	printf("INFO %d hashes written \n",j);
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
        Entry **table = NULL;
        int num_entries = 0;

        if (!load_table_from_file(argv[2], &table, &num_entries)) {
            fprintf(stderr, "Erreur lors du chargement de la table T3C.\n");
            return 1;
        }

        
        
    

        char hash[256];
        while(fgets(hash,sizeof(hash),stdin)){
            hash[strcspn(hash, "\n")] = '\0';
            lookup_in_table(hash, table, num_entries);
        }
       
              

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

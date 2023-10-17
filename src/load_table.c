#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"


// Fonction pour charger la table de correspondance chaîne-condensat (T3C) depuis un fichier
int load_table_from_file(const char *filename, Entry **table, int *num_entries) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier T3C");
        return 0;
    }

    char line[256];
    int max_entries=100;
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

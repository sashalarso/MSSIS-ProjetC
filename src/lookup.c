#include <stdio.h>
#include <string.h>
#include "table.h"

void lookup_in_table(const char *target_hash, Entry *table, int num_entries) {
    for (int i = 0; i < num_entries; i++) {
        if (strcmp(target_hash, table[i].hash) == 0) {
            printf("MATCH %s %s\n", target_hash, table[i].chaine);
        }
    }
}
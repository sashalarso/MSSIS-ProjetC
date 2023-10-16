#ifndef TABLE_H
#define TABLE_H

typedef struct {
    char *hash;
    char *chaine;
} Entry;

int load_table_from_file(const char *filename, Entry **table, int *num_entries);
void lookup_in_table(const char *filename,Entry *table,int num_entries,int max_entries);


#endif

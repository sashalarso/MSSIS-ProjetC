#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

typedef struct {
    char* key;
    int value;
} item;

item* searchPassword(item* items, int size, const char* key) {
    
    for (size_t i=0; i<size; i++) {
        //printf("%s %s \n",items[i].key,key);
        if (strcmp(items[i].key, key) == 0) {
            return &items[i];
        }
    }
    return NULL;
}

int insertItem(item* items, int size,const char* key){
    items[size].key="test";
    items[size].value=2;
    size++;
    return size;
}

void md5_hash_from_string (char *string, char *hash)
{
    int i;
    char unsigned md5[MD5_DIGEST_LENGTH] = {0};

    MD5((const unsigned char *)string, strlen(string), md5);

    for (i=0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2*i, "%02x", md5[i]);
    }
}

void sha256_hash_string(const char *string, char outputBuffer[65]) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}

int main(void) {
    int num_items = 0;
    item items[] = {};
    
    num_items=insertItem(items,num_items,"test");
    //printf("%d",items[0].value);
    item* found = searchPassword(items, num_items, "test");
    if (!found) {
        return 1;
    }
    printf("linear_search: value of 'bob' is %d\n", found->value);
    //printf("%d", items[0].value);
    char string[255] = "Hello Worlde";
    char md5_hash[2*MD5_DIGEST_LENGTH+1] = "";
    md5_hash_from_string(string, md5_hash);
    printf("%s\n", md5_hash);


    const char *input_string = "Votre_chaine_a_hasher";
    char output_hash[65];

    sha256_hash_string(input_string, output_hash);

    printf("Chaîne originale : %s\n", input_string);
    printf("Hachage SHA-256 : %s\n", output_hash);
   
    return 0;
}

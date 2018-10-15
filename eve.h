#ifndef EVE_H_
#define EVE_H_

typedef struct {
    int shifts[25];
    char *infile;
    char *outfile;
    char *cipher;
    char *ciphertext;
    char *encode;
    char *hash;
} Namespace;

#endif //EVE_H_

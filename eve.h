#ifndef EVE_H_
#define EVE_H_

typedef struct {
    int shifts[25];
    char *infile;
    char *outfile;
    char *mode;
    char *cipher;
    char *encoding;
    char *hash;
    char *etext;
} Namespace;

#endif //EVE_H_

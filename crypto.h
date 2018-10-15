#ifndef CRYPTO_H_
#define CRYPTO_H_

#include "eve.h"

char *setup_plaintext(const char *infile, const char *ctext);
int free_plaintext(char *ptext);
int total_char_count(const char *infile);
void run_caesar(char *ptext, Namespace *parser);
void caesar(char *ctext, char *ptext, const int space, const int *shift);

#endif // CRYPTO_H_

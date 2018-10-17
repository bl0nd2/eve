#include <stdio.h>
#include <string.h>

#include "output.h"
#include "eve.h"

void output(const char *ptext, Namespace *parser) {
    /*  Handle plaintext output. */
    if (parser->outfile)
        printf("outputted to a file.\n");
    else {
        if (strcmp(parser->mode, "cipher") == 0) {
            if (strcmp(parser->cipher, "caesar") == 0) {
                static int shift = 0;
                printf("ROT-%d: %s\n", parser->shifts[shift++], ptext);
            }
        }
        else if (strcmp(parser->mode, "encoding") == 0) {
            printf("%s\n", ptext);
        }
        else if (strcmp(parser->mode, "hash") == 0) {
            printf("hash output\n");
        }
    }
}

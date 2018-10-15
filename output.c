#include <stdio.h>

#include "output.h"

void output(const char *ptext, const char *outfile) {
    /*  Handle plaintext output. */
    if (outfile)
        printf("outputted to a file.\n");
    else
        printf("%s\n", ptext);
}

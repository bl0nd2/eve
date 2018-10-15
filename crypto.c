#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "crypto.h"
#include "eve.h"
#include "output.h"

#define MOD(a,b) (((a % b) + b) % b)  // C's native mod doesn't properly handle negative numbers (e.g., -1 % 26)


/***********************************
 *          Crypto Setup           *
 ***********************************/
char *setup_plaintext(Namespace *parser) {
    /*  Allocate the appropriate amount of memory to plaintext. */
    char *plaintext;

    if (! parser->infile) {
        if (strcmp(parser->cipher, "caesar") == 0)
            plaintext = malloc(sizeof(char) * strlen(parser->ciphertext));
        else {
            printf("Assign proper memory to plaintext\n");
            exit(1);
        }
    }
    else
        plaintext = malloc(sizeof(char) * total_char_count(parser->infile));

    if (plaintext == NULL) {
        puts("error: unsuccessful memory allocation for 'plaintext'");
        exit(1);
    }

    return plaintext;
}

int free_plaintext(char *ptext) {
    /*  Free plaintext strings in functions. */
    free(ptext);
    return 0;
}

int total_char_count(const char *infile) {
    /*  Return total numbers of characters in the input file. */
    FILE *fp;
    int count;
    char buff[255];

    fp = fopen(infile, "r");
    while (fgets(buff, 255, (FILE*)fp))
        count += strlen(buff);
    fclose(fp);

    return count;
}


/***********************************
 *             Crypto              *
 ***********************************/
void run_caesar(char *ptext, Namespace *parser) {
    /*  Set up and run Caesar cipher decoder. */
    int *shift, spacing;

    if (! parser->infile) {
        for (shift = parser->shifts; *shift; shift++) {
            caesar(parser->ciphertext, ptext, 0, shift);
            output(ptext, parser->outfile);
        }
    }
    else {
        char line[255];
        FILE *fp;

        for (shift = parser->shifts; *shift; shift++) {
            spacing = 0;
            fp = fopen(parser->infile, "r");
            while (fgets(line, 255, (FILE*)fp)) {
                caesar(line, ptext, spacing, shift);
                spacing += strlen(line);
            }
            output(ptext, parser->outfile);
            fclose(fp);
        }
    }
}

void caesar(char *ctext, char *ptext, const int spacing, const int *shift) {
    /*  Decode Caesar ciphers. */
    int i, ascii_val, start_val;

    for (i = 0; i < strlen(ctext); i++) {
        ascii_val = ctext[i];

        if (isalpha(ascii_val)) {
            start_val = isupper(ascii_val) ? 65 : 97;
            ptext[i + spacing] = start_val + MOD((ascii_val - start_val - *shift), 26);
        }
        else
            ptext[i + spacing] = ascii_val;
    }
}

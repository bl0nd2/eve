#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "crypto.h"
#include "eve.h"
#include "output.h"

#define MOD(a,b) (((a % b) + b) % b)


/***********************************
 *          Crypto Setup           *
 ***********************************/
char *setup_plaintext(const char *infile, const char *ctext) {
    /* Allocate memory to 'plaintext' */
    char *plaintext;

    if (! infile)
        plaintext = malloc(sizeof(char) * strlen(ctext));
    else
        plaintext = malloc(sizeof(char) * total_char_count(infile));

    if (plaintext == NULL) {
        puts("error: unsuccessful memory allocation for 'plaintext'");
        exit(1);
    }

    return plaintext;
}

int free_plaintext(char *ptext) {
    /* Free plaintext strings in cipher functions. */
    free(ptext);
    return 0;
}

int total_char_count(const char *infile) {
    /* Return total numbers of characters in the input file. */
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

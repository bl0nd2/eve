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
            plaintext = malloc(sizeof(char) * strlen(parser->etext));
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
            caesar(parser->etext, ptext, 0, shift);
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

void run_base64(char *ptext, Namespace *parser) {
    /*  Use MIME Base64 scheme
     *  Newlines and whitespaces are ignored on decoding
     *
     *  Normal example:
     *    "Man" would be TWFu (as bytes: M=77, a=97, n=110), or 
     *      binary values M=01001101, a=01100001, n=01101110.
     *      Together, that's 010011010110000101101110.
     *    Groups of 6 bits (2^6 = 64 binary values) are converted
     *      into numbers left to right (in this case, there are 4
     *      numbers for a 24-bit string).
     *    These numbers are converted into their corresponding Base64 values.
     *
     *  Padding example:
     *    = can be added to make the last encoded block contain 4 Base64 characters.
     *
     *    "Ma" is only 16 bits, which'll all get captured in the 1st 3 Base64
     *      digits (18 bits). The 2 least significant bits of the last 6-bits
     *      turn out to be 0's and when decoding, we'll discard those, along
     *      with any padding characters.
     *
     *    2 == indicates that the last group contained only 1 byte, 1 == indicates
     *      it contained 2 bytes.
     *
     *
     *  Decoding:
     *    4 characters are typically converted back to 3 bytes. The only exceptions
     *      are when padding exists. 1 = means the 4 characters will decode to only
     *      2 bytes, while == means 1 byte.
     *    
     */
    printf("running base64\n");
}

void base64(char *ctext, char *ptext) {
    printf("base64\n");
}

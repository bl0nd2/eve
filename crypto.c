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
        if (strcmp(parser->mode, "cipher") == 0)
            plaintext = malloc(sizeof(char) * strlen(parser->etext));
        else if (strcmp(parser->mode, "encoding") == 0) {
            if (strcmp(parser->encoding, "base64") == 0) {
                // apparently encoded base64 is about 1.37N
                // turns out 1.37 cuts off the plaintext for longer encodings so...
                plaintext = malloc(sizeof(char) * ((double) strlen(parser->etext) / 1.3));
            }
        }
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
            output(ptext, parser);
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
            output(ptext, parser);
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
    int len_str = strlen(parser->etext);

    if (!parser->infile) {
        base64(parser->etext, len_str, ptext);
        output(ptext, parser);
    }
    else
        printf("running base64\n");
}

void base64(char *ctext, int len_str, char *ptext) {
    int i, j, k = 0; 
  
    // stores the bitstream. 
    int num = 0; 
  
    // count_bits stores current 
    // number of bits in num. 
    int count_bits = 0; 
  
    // selects 4 characters from 
    // encoded string at a time. 
    // find the position of each encoded 
    // character in char_set and stores in num. 
    for (i = 0; i < len_str; i += 4) { 
        num = 0, count_bits = 0; 
        for (j = 0; j < 4; j++) { 
            // make space for 6 bits. 
            if (ctext[i + j] != '=') { 
                num = num << 6; 
                count_bits += 6; 
            } 
  
            /* Finding the position of each encoded  
            character in char_set  
            and storing in "num", use OR  
            '|' operator to store bits.*/
  
            // encoded[i + j] = 'E', 'E' - 'A' = 5 
            // 'E' has 5th position in char_set. 
            if (ctext[i + j] >= 'A' && ctext[i + j] <= 'Z') 
                num = num | (ctext[i + j] - 'A'); 
  
            // encoded[i + j] = 'e', 'e' - 'a' = 5, 
            // 5 + 26 = 31, 'e' has 31st position in char_set. 
            else if (ctext[i + j] >= 'a' && ctext[i + j] <= 'z') 
                num = num | (ctext[i + j] - 'a' + 26); 
  
            // encoded[i + j] = '8', '8' - '0' = 8 
            // 8 + 52 = 60, '8' has 60th position in char_set. 
            else if (ctext[i + j] >= '0' && ctext[i + j] <= '9') 
                num = num | (ctext[i + j] - '0' + 52); 
  
            // '+' occurs in 62nd position in char_set. 
            else if (ctext[i + j] == '+') 
                num = num | 62; 
  
            // '/' occurs in 63rd position in char_set. 
            else if (ctext[i + j] == '/') 
                num = num | 63; 
  
            // ( str[i + j] == '=' ) remove 2 bits 
            // to delete appended bits during encoding. 
            else { 
                num = num >> 2; 
                count_bits -= 2; 
            } 
        } 
  
        while (count_bits != 0) { 
            count_bits -= 8; 
  
            // 255 in binary is 11111111 
            ptext[k++] = (num >> count_bits) & 255; 
        } 
    } 
  
    // place NULL character to mark end of string. 
    ptext[k] = '\0'; 
} 

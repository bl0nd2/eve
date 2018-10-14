#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <argp.h>

#include "crypto.h"

/* Global variables */

static int arg_shifts[25];
static char *arg_infile     = NULL;
static char *arg_outfile    = NULL;
static char *arg_cipher     = NULL;
static char *arg_encode     = NULL;
static char *arg_hash       = NULL;
static char *arg_ciphertext = NULL;

/* Prototypes */
static int parse_opt(int key, char *arg, struct argp_state *state);
static int opt_error();
static int setup_shifts(char *arg);
static int help(void);
static int version(void);


/***********************************
 *            Parser               *
 ***********************************/
static int parse_opt(int key, char *arg, struct argp_state *state) {
    switch(key) {
        case 'h':
            return help();
        case 900:
            return version();
        case 'i': {
            arg_infile = arg;
            break;
        }
        case 'o': {
            arg_outfile = arg;
            break;
        }
        case 's': {
            setup_shifts(arg);
            break;
        }
        case 901: {
            arg_cipher = "caesar";
            arg_ciphertext = arg;
            break;
        }
    }

    return 0;
}

static int setup_shifts(char *arg) {
    int shift_count = 0;
    char *token;

    while( (token = strsep(&arg,",")) != NULL ) {
        if (strcmp(token, "all") == 0) {
            for (shift_count = 0; shift_count < 25; shift_count++)
                arg_shifts[shift_count] = shift_count + 1;
            break;
        }
        else if (atoi(token) > 0 && atoi(token) < 26)
            arg_shifts[shift_count++] = atoi(token);
        else {
            puts("error: invalid shift");
            exit(1);
        }
    }
    
    return 0;
}

static int opt_error() {
    if (arg_cipher) {
        if (!arg_ciphertext && !arg_infile) {
            printf("error: missing ciphertext.\n");
            return 1;
        }
        else if (arg_ciphertext && arg_infile) {
            printf("error: option \"--%s\" must be empty if option "
                   "\"--infile\" is specified\n", arg_cipher);
            return 1;
        }
        else if (arg_shifts[0] == 0) {
            printf("error: no shift(s) given.\n");
            return 1;
        }
    }

    if (arg_encode) {
        printf("handling encoding...\n");
        return 1;
    }

    if (arg_hash) {
        printf("handling hash...\n");
        return 1;
    }

    return 0;
}

static int help(void) {
    printf("usage: eve [options] <mode>[=<ciphertext>]\n"
           "\nModes:\n"
           "  ciphers      --caesar\n"
           "  encodings    --base64\n"
           "  hashes       --md5\n"
           "\nOptions:"
           "\n   general\n"
           "     -i, --infile  INFILE              Specify file to be read\n"
           "     -o, --outfile OUTFILE             Specify file to be written to\n"
           "     -h, --help                        Show this help menu\n"
           "         --version                     Show program version\n"
           "\n   cryptography\n"
           "     -s, --shift   SHIFT[,SHIFT...]    Specify shift(s) to be used\n\n"
    );

    return 0;
}

static int version(void) {
    printf("eve version 0.0.1\n");
    return 0;
}


/***********************************
 *              Main               *
 ***********************************/
int main(int argc, char *argv[]) {
    struct argp_option options[] = {
        {"help",    'h', 0,            0,                   "Show this help menu"},
        {"version", 900, 0,            0,                   "Show program version"},
        {"caesar",  901, "CIPHERTEXT", OPTION_ARG_OPTIONAL, "Decode shift ciphers"},
        {"infile",  'i', "INFILE",     0,                   "Specify file to be read"},
        {"outfile", 'o', "OUTFILE",    0,                   "Specify file to be written to"},
        {"shift",   's', "SHIFT",      0,                   "Specify the shift(s) to be used"},
        {0}
    };

    struct argp argp = {options, parse_opt};
    int r = argp_parse(&argp, argc, argv, 0, 0, 0);

    assert(r == 0);

    if (opt_error())
        exit(1);

    if (arg_cipher) {
        if (strcmp(arg_cipher, "caesar") == 0) {
            char *ptext = setup_plaintext(arg_infile, arg_ciphertext);
            run_caesar(ptext, arg_shifts, arg_ciphertext, arg_infile, arg_outfile);
            free_plaintext(ptext);
        }
    }
}


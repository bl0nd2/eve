#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <argp.h>

#include "crypto.h"
#include "eve.h"


/* Prototypes */
static int parse_opt(int key, char *arg, struct argp_state *state);
static int opt_error(Namespace parser);
static int setup_shifts(char *arg, Namespace *parser);
static void help(void);
static void version(void);


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

    Namespace parser = {
        {0},   // shifts[25]
        NULL,  // infile
        NULL,  // outfile
        NULL,  // cipher
        NULL,  // ciphertext
        NULL,  // encode
        NULL   // hash
    };

    struct argp argp = {options, parse_opt};
    int r = argp_parse(&argp, argc, argv, 0, 0, &parser);

    assert(r == 0);

    if (opt_error(parser))
        exit(1);

    if (parser.cipher) {
        if (strcmp(parser.cipher, "caesar") == 0) {
            char *ptext = setup_plaintext(&parser);
            run_caesar(ptext, &parser);
            free_plaintext(ptext);
        }
    }
}

static int parse_opt(int key, char *arg, struct argp_state *state) {
    /*  Set parser struct fields based on command-line arguments. */
    Namespace *parser = state->input; 

    switch(key) {
        case 'h':
            help();
        case 900:
            version();
        case 'i': {
            parser->infile = arg;
            break;
        }
        case 'o': {
            parser->outfile = arg;
            break;
        }
        case 's': {
            setup_shifts(arg, parser);
            break;
        }
        case 901: {
            parser->cipher = "caesar";
            parser->ciphertext = arg;
            break;
        }
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static int setup_shifts(char *arg, Namespace *parser) {
    /*  Separate shifts and add them to the shift array. */
    int shift_count = 0;
    char *token;
    
    while( (token = strsep(&arg,",")) != NULL ) {
        if (strcmp(token, "all") == 0) {
            for (shift_count = 0; shift_count < 25; shift_count++)
                parser->shifts[shift_count] = shift_count + 1;
            break;
        }
        else if (atoi(token) > 0 && atoi(token) < 26)
            parser->shifts[shift_count++] = atoi(token);
        else {
            puts("error: invalid shift");
            exit(1);
        }
    }
    
    return 0;
}

static int opt_error(Namespace parser) {
    /*  Handle invalid parser options.  */
    if ((!parser.cipher && !parser.encode && !parser.hash) &&
        (parser.infile || parser.outfile)) {
        puts("error: no mode specified.");
        return 1;
    }

    if (parser.cipher) {
        if (parser.shifts[0] == 0) {
            puts("error: no shift(s) given.");
            return 1;
        }
        else if (parser.ciphertext && parser.infile) {
            printf("error: option \"--%s\" must be empty if option "
                   "\"--infile\" is specified\n", parser.cipher);
            return 1;
        }
        else if ((!parser.ciphertext && !parser.infile) ||
                 strcmp(parser.ciphertext, " "))
        {
            puts("error: missing ciphertext.");
            return 1;
        }
    }

    if (parser.encode) {
        puts("handling encoding...");
        return 1;
    }

    if (parser.hash) {
        puts("handling hash...");
        return 1;
    }

    return 0;
}

static void help(void) {
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
    exit(0);
}

static void version(void) {
    printf("eve version 0.0.1\n");
    exit(0);
}

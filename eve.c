#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <argp.h>

/* Global variables */
static int arg_shift[26];
static char *arg_infile = NULL;
static char *arg_outfile = NULL;
static char *arg_cipher = NULL;
static char *arg_ciphertext = NULL;
static const char *program_invocation_short_name = "eve";
static const char *program_help_link = "https://github.com/bl0nd/eve";

/* Prototypes */
static int help(void);
static int version(void);
static int caesar(char const *infile);
static int check_case(char const cvalue, char const pvalue); 
static int free_ciphers(char *plntxt);
static int output(char const *plntxt);
static int parse_opt(int key, char *arg, struct argp_state *state);


static int help(void) {
    printf("usage: %s [--version] [--help] <mode> ciphertext\n\n"
           "Decode encrypted or encoded text.\n"
           "\n   Cryptography options\n"
           "\t    --caesar    Decode shift ciphers\n"
           "\t-s, --shift     Specify shift(s) to be used\n"
           "\n   General options\n"
           "\t-i, --infile    Specify file to be read\n"
           "\t-o, --outfile   Specify file to be written to\n"
           "\t-h, --help      Show this help menu\n"
           "\t    --version   Show program version\n"
           "\nSee %s for details.\n"
           , program_invocation_short_name
           , program_help_link
    );

    return 0;
}

static int version(void) {
    printf("eve version 0.0.1\n");
    return 0;
}

static int caesar(char const *infile) {
    /* IF:   An infile isn't specified.
     * ELSE: An infile is specified.
     * DO:   Decode and output to STDIN, or to a file. */
    if (! infile) {
        int i, *shift;
        char *plaintext = malloc(sizeof(char) * strlen(arg_ciphertext));
        if (plaintext == NULL) return 1;

        /* Decode cipher in every shift given */
        for (shift = arg_shift; *shift != 0; shift++) {
            for (i = 0; i < strlen(arg_ciphertext); i++) {
                int ascii_value = arg_ciphertext[i];

                if (isalpha(ascii_value)) {
                    char test_char = ascii_value + (26 - *shift);
                    if (isalpha(test_char) && check_case(ascii_value, test_char))
                        plaintext[i] = test_char;
                    else
                        plaintext[i] = ascii_value - *shift;
                  }
                else
                    plaintext[i] = ascii_value;
            }
            plaintext[strlen(arg_ciphertext)] = '\0';

            output(plaintext);
        }
        free_ciphers(plaintext);
    }
    else {
        if (arg_ciphertext) {
            printf("error: option \"--caesar\" must be empty if option "
                   "\"--infile\" is specified\n");
            return 1;
        }
        else {
            printf("TODO\n");
        }
    }

    return 0;
}

static int check_case(char const cvalue, char const pvalue) {
    if ((isupper(cvalue) && isupper(pvalue)) ||
        (islower(cvalue) && islower(pvalue)))
        return 1;
    else
        return 0;
}

static int free_ciphers(char *plntxt) {
    /* Free plaintext strings in cipher functions. */
    free(plntxt);
    return 0;
}

static int output(char const *plntxt) {
    /* Handle plaintext output. */
    if (arg_infile) 
        printf("outputted to a file\n.");
    else
        printf("%s\n", plntxt);

    return 0;
}

static int parse_opt(int key, char *arg, struct argp_state *state) {
    // printf("key: %c\n\n", key);

    switch(key)
    {
        case 'h':
            return help();

        case 900:
            return version();

        case 'i':
        {
            arg_infile = arg;
            // printf("%s\n", arg_infile);
            break;
        }

        case 'o':
        {
            arg_outfile = arg;
            // printf("%s\n", arg_infile);
            break;
        }

        case 's':
        {
            int shift_count;
            char *token;

            /* Get list of shifts */
            while( (token = strsep(&arg,",")) != NULL ) {
                if (atoi(token) == 0)
                    continue;
                if (shift_count < 26)
                    arg_shift[shift_count++] = atoi(token);
                else {
                    puts("error: too many shifts.");
                    return 1;
                }
            }
            break;
        }

        case 901:
        {
            arg_cipher = "caesar";
            if (arg)
                arg_ciphertext = arg;
            else {
                printf("error: missing ciphertext.\n");
                return 1;
            }
            // printf("%s\n%s\n", arg_cipher, arg_ciphertext);
            break;
        }

        // default:
        //  printf("Unhandled option\n");
    }

    return 0;
}

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

    if (arg_cipher) {
        if (strcmp(arg_cipher, "caesar") == 0)
            caesar(arg_infile);
    }
    /*else {*/
        /*puts("error: missing cipher");*/
        /*return 1;*/
    /*}*/
}


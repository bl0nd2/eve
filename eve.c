#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <argp.h>

#define MOD(a,b) (((a % b) + b) % b)

/* Global variables */

static int arg_shifts[25];
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
    if (arg_shifts[0] == 0) {
        printf("error: no shift(s) given.\n");
        return 1;
    }

    if (! infile) {
        int i, *shift, ascii_value, start_value;
        char test_char;
        char *plaintext = malloc(sizeof(char) * strlen(arg_ciphertext));
        if (plaintext == NULL) return 1;

        /* Decode cipher in every shift given */
        for (shift = arg_shifts; *shift != 0; shift++) {
            for (i = 0; i < strlen(arg_ciphertext); i++) {
                ascii_value = arg_ciphertext[i];

                if (isalpha(ascii_value)) {
                    start_value = isupper(ascii_value) ? 65 : 97;
                    plaintext[i] = start_value + MOD((ascii_value - start_value - *shift), 26);
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
            int shift_count = 0;
            char *token;

            /* Get list of shifts */
            while( (token = strsep(&arg,",")) != NULL ) {
                if (strcmp(token, "all") == 0) {
                    for (shift_count = 0; shift_count < 25; shift_count++)
                        arg_shifts[shift_count] = shift_count + 1;
                    break;
                }
                else if (atoi(token) == 0)
                    continue;
                else if (atoi(token) < 0 || atoi(token) > 25) {
                    printf("error: invalid shift \"%s\"\n", token);
                    return 1;
                }
                else {
                    arg_shifts[shift_count++] = atoi(token);
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
    else {
        puts("error: missing cipher");
        return 1;
    }
}


#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <argp.h>

/* Global variables */
static char *arg_infile = NULL;
static char *arg_outfile = NULL;
static char *arg_cipher = NULL;
static char *arg_ciphertext = NULL;
static const char *program_invocation_short_name = "eve";
static const char *program_help_link = "https://github.com/bl0nd/eve";

/* Prototypes */
static int help(void);
static int version(void);
static int caesar(char const *file_input);
static int check_case(char letter1, char letter2);
static int parse_opt(int key, char *arg, struct argp_state *state);


static int help(void) {
	printf("usage: %s [--version] [--help] <mode> ciphertext\n\n"
		   "Decode encrypted or encoded text.\n"
		   "\n   Cryptography options\n"
		   "\t    --caesar    Decode shift ciphers\n"
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
	if (! infile) {
	    int i, shift;
	    char plaintext[strlen(arg_ciphertext)];

	    shift = 13;

	    for (i = 0; i < strlen(arg_ciphertext); i++)
	    {
		    int ascii_value = arg_ciphertext[i];

		    if ((ascii_value >= 65 && ascii_value <= 90) ||
			    (ascii_value >= 97 && ascii_value <= 122)
			) {
			    char test_char = ascii_value + (26 - shift);
			    if (isalpha(test_char) &&
			    	check_case(arg_ciphertext[i], test_char)
			    )
				    plaintext[i] = test_char;
			    else
				    plaintext[i] = ascii_value - shift;
		    }
		    else
			    plaintext[i] = arg_ciphertext[i];

	    }
	    plaintext[strlen(arg_ciphertext)] = '\0';

	    printf("%s\n", plaintext);
	    return 0;
	}
	else {
		if (arg_ciphertext) {
			printf("error: option \"--caesar\" must be empty if option "
				   "\"--infile\" is specified\n");
			return 1;
		}
  	}

	return 0;
}

static int check_case(char cvalue, char pvalue)
{
	if ((isupper(cvalue) && isupper(pvalue)) ||
		(islower(cvalue) && islower(pvalue)))
		return 1;
	else
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

		case 901:
		{
			arg_cipher = "caesar";
			arg_ciphertext = arg;
			// printf("%s\n%s\n", arg_cipher, arg_ciphertext);
			break;
		}

		// default:
		// 	printf("Unhandled option\n");
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
		{0}
	};

	struct argp argp = {options, parse_opt};
	int r = argp_parse(&argp, argc, argv, 0, 0, 0);

	if (arg_cipher) {
		if (strcmp(arg_cipher, "caesar") == 0)
			caesar(arg_infile);
	}
	else {
		puts("error: missing cipher");
		return 1;
	}
}


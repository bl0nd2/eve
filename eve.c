#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
// #include <argp.h>
#include <getopt.h>
#include <unistd.h>

/* Global variables */
static char *arg_cipher = "";
static char *arg_ciphertext = NULL;
static const char *program_invocation_short_name = "eve";
static const char *program_help_link = "https://github.com/bl0nd/eve";

/* Prototypes */
static int help(void);
static int version(void);
static int caesar(void);
static int check_case(char letter1, char letter2);


static int help(void) {
	printf("usage: %s [--version] [--help] <mode> ciphertext\n\n"
		   "Decode encrypted or encoded text.\n\n"
		   "  -h, --help      Show this help menu\n"
		   "      --caesar    Decode shift ciphers\n"
		   "      --version   Show program version\n"
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

static int caesar(void) {
	int i, shift;
	shift = 13;
	char plaintext[strlen(arg_ciphertext)];
	plaintext[strlen(arg_ciphertext)] = '\n';

	for (i = 0; i < strlen(arg_ciphertext); i++)
	{
		char cipher_value = arg_ciphertext[i]
		int ascii_value = arg_ciphertext[i];

		if ((ascii_value >= 65 && ascii_value <= 90) ||
			(ascii_value >= 97 && ascii_value <= 122))
		{
			char test_char = ascii_value + (26 - shift);
			if (isalpha(test_char) && check_case(cipher_value, test_char))
				plaintext[i] = test_char;
			else
				plaintext[i] = ascii_value - shift;
		}
		else
			plaintext[i] = arg_ciphertext[i];
	}
	printf("%s\n", plaintext);

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

static int parse_argv(int argc, char *argv[]) {
	// long options
	enum {
		ARG_VERSION,
		ARG_CAESAR
	};

	static const struct option options[] = {
		{ "help",    no_argument,       NULL, 'h'		  },
		{ "version", no_argument,       NULL, ARG_VERSION },
		{ "caesar",  required_argument, NULL, ARG_CAESAR  },
		{}
	};

	int c;

	assert(argc >= 0);
	assert(argv);

	while ((c = getopt_long(argc, argv, "h", options, NULL)) >= 0)

		switch(c) {

			case 'h':
				return help();

			case ARG_VERSION:
				return version();

			case ARG_CAESAR:
				arg_cipher = "caesar";
				arg_ciphertext = optarg;
				break;

			default:
				printf("Unhandled option\n");
		}

	return 1;
}

int main(int argc, char *argv[]) {
	int r;
	r = parse_argv(argc, argv);

	if (strcmp(arg_cipher, "caesar") == 0)
	{
		printf("Starting Caesar decode...\n\n");
		caesar();
	}
	return 0;
}
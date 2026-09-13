#include "cli/options.h"

#include "common/checked.h"
#include "common/compiler.h"
#include "common/status.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char const ITPLD_HELP_MESSAGE[]	   = "itpld linker\n"
				     "Usage: itpld [options] file...\n"
				     "Options:\n"
				     "\t-h\t\tprint this message (only short option available)\n"
				     "\t-v\t\tprint the version (only short option available)\n"
				     "\t-o\t\tpath to the output executable file\n"
				     "\t-e\t\tentry symbol name\n"
				     "\t[no key]\tpath to the input ELF file";
char const ITPLD_VERSION_MESSAGE[] = "itpld version " ITPLD_VERSION;

void
itpld_cliopts_init(itpld_cli_options_t * opts)
{
	static char const DEFAULT_OUTPUT_PATH[]	 = "a.out";
	static char const DEFAULT_ENTRY_SYMBOL[] = "_start";

	if (opts == NULL) return;

	opts->output_path = DEFAULT_OUTPUT_PATH;

	opts->input_paths      = NULL;
	opts->input_paths_size = 0;

	opts->entry_symbol = DEFAULT_ENTRY_SYMBOL;

	opts->action = ITPLD_CLI_ACTION_LINK;
}

void
itpld_cliopts_destroy(itpld_cli_options_t * opts)
{
	if (opts == NULL) return;

	opts->output_path = NULL;

	free((void *)opts->input_paths);
	opts->input_paths      = NULL;
	opts->input_paths_size = 0;

	opts->entry_symbol = NULL;

	opts->action = ITPLD_CLI_ACTION_LINK;
}

static void
itpld_fill_clierr(itpld_cli_error_t * clierr, char const * ctx, char const * msg)
{
	if (clierr != NULL) {
		clierr->context = ctx;
		clierr->message = msg;
	}
}

static itpld_status_t
itpld_cliopts_validate_parse_args(int argc, char ** argv, itpld_cli_options_t * opts, itpld_cli_error_t * err)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	if (opts == NULL) {
		itpld_fill_clierr(err, "cli", "'opts' argument is null");
		stat = ITPLD_STATUS_INVAL_ARG;
	}

	if (argc < 1) {
		itpld_fill_clierr(err, "cli", "too few arguments");
		stat = ITPLD_STATUS_INVAL_ARG;
	}

	if (argv == NULL) {
		itpld_fill_clierr(err, "cli", "'argv' argument is null");
		stat = ITPLD_STATUS_INVAL_ARG;
	} else {
		if (argv[0] == NULL) {
			itpld_fill_clierr(err, "cli", "'argv[0]' argument is null");
			stat = ITPLD_STATUS_INVAL_ARG;
		}
	}

	return stat;
}

static itpld_status_t
itpld_cliopts_append_input(itpld_cli_options_t * opts, char const * token, itpld_cli_error_t * err)
{
	size_t newsz;
	if (!itpld_size_add(opts->input_paths_size, 1, &newsz)) {
		itpld_fill_clierr(err, token, "failed to increase the size of internal buffer for new input file path");
		return ITPLD_STATUS_OVERFLOW;
	}
	size_t newsz_bytes;
	if (!itpld_size_mul(newsz, sizeof(*opts->input_paths), &newsz_bytes)) {
		itpld_fill_clierr(err, token, "failed to increase the size of internal buffer for new input file path");
		return ITPLD_STATUS_OVERFLOW;
	}
	char const ** new_input_paths = (char const **)realloc((void *)opts->input_paths, newsz_bytes);
	if (new_input_paths == NULL) {
		itpld_fill_clierr(err, token, "failed to allocate memory for new input file path");
		return ITPLD_STATUS_OUT_OF_MEM;
	}

	new_input_paths[opts->input_paths_size] = token;

	opts->input_paths      = new_input_paths;
	opts->input_paths_size = newsz;

	return ITPLD_STATUS_OK;
}

typedef enum itpld_cli_value_option { ITPLD_CLI_VALOPT_ENTRYSYM, ITPLD_CLI_VALOPT_OUTPUT } itpld_cli_value_option_t;

static itpld_status_t
itpld_cliopts_parse_value_option(
    size_t *		     idx,
    size_t		     argc,
    char **		     argv,
    itpld_cli_options_t *    cliopts,
    itpld_cli_value_option_t valopt,
    bool *		     seen,
    itpld_cli_error_t *	     err
)
{
	char const * token = argv[*idx];

	if (*idx + 1 >= argc) {
		switch (valopt) {
		case ITPLD_CLI_VALOPT_ENTRYSYM:
			itpld_fill_clierr(err, token, "expected entry symbol name");
			break;
		case ITPLD_CLI_VALOPT_OUTPUT:
			itpld_fill_clierr(err, token, "expected output file path");
			break;
		default:
			itpld_dead_code();
		}

		return ITPLD_STATUS_INVAL_ARG;
	}

	if (*seen) {
		switch (valopt) {
		case ITPLD_CLI_VALOPT_ENTRYSYM:
			itpld_fill_clierr(err, token, "duplicate entry option");
			break;
		case ITPLD_CLI_VALOPT_OUTPUT:
			itpld_fill_clierr(err, token, "duplicate output option");
			break;
		default:
			itpld_dead_code();
		}

		return ITPLD_STATUS_INVAL_ARG;
	}

	char const * arg = argv[++(*idx)];

	if (arg == NULL) {
		itpld_fill_clierr(err, token, "null argument detected");
		return ITPLD_STATUS_INVAL_ARG;
	}
	if (arg[0] == '\0') {
		switch (valopt) {
		case ITPLD_CLI_VALOPT_ENTRYSYM:
			itpld_fill_clierr(err, token, "empty entry symbol");
			break;
		case ITPLD_CLI_VALOPT_OUTPUT:
			itpld_fill_clierr(err, token, "empty output file path");
			break;
		default:
			itpld_dead_code();
		}

		return ITPLD_STATUS_INVAL_ARG;
	}
	if (arg[0] == '-') {
		switch (valopt) {
		case ITPLD_CLI_VALOPT_ENTRYSYM:
			itpld_fill_clierr(err, token, "expected entry symbol name, got option");
			break;
		case ITPLD_CLI_VALOPT_OUTPUT:
			itpld_fill_clierr(err, token, "expected output file path, got option");
			break;
		default:
			itpld_dead_code();
		}

		return ITPLD_STATUS_INVAL_ARG;
	}

	switch (valopt) {
	case ITPLD_CLI_VALOPT_ENTRYSYM:
		cliopts->entry_symbol = arg;
		break;
	case ITPLD_CLI_VALOPT_OUTPUT:
		cliopts->output_path = arg;
		break;
	default:
		itpld_dead_code();
	}
	*seen = true;

	return ITPLD_STATUS_OK;
}

typedef enum itpld_cli_token_result { ITPLD_CLI_TOKEN_CONTINUE, ITPLD_CLI_TOKEN_STOP } itpld_cli_token_result_t;

static itpld_status_t
itpld_cliopts_parse_token(
    size_t *		       idx,
    size_t		       argc,
    char **		       argv,
    bool *		       output_seen,
    bool *		       entry_seen,
    itpld_cli_options_t *      opts,
    itpld_cli_token_result_t * res,
    itpld_cli_error_t *	       err
)
{
	char const * token = argv[*idx];

	if (token == NULL) {
		itpld_fill_clierr(err, "cli", "null option detected");
		*res = ITPLD_CLI_TOKEN_STOP;
		return ITPLD_STATUS_INVAL_ARG;
	}

	if (token[0] == '\0') {
		itpld_fill_clierr(err, "cli", "empty option detected");
		*res = ITPLD_CLI_TOKEN_STOP;
		return ITPLD_STATUS_INVAL_ARG;
	}

	if (strcmp(token, "-h") == 0) {
		opts->action = ITPLD_CLI_ACTION_HELP;
		*res	     = ITPLD_CLI_TOKEN_STOP;
		return ITPLD_STATUS_OK;
	}

	if (strcmp(token, "-v") == 0) {
		opts->action = ITPLD_CLI_ACTION_VERSION;
		*res	     = ITPLD_CLI_TOKEN_STOP;
		return ITPLD_STATUS_OK;
	}

	itpld_status_t stat = ITPLD_STATUS_OK;

	if (strcmp(token, "-e") == 0) {
		stat =
		    itpld_cliopts_parse_value_option(idx, argc, argv, opts, ITPLD_CLI_VALOPT_ENTRYSYM, entry_seen, err);
		*res = stat == ITPLD_STATUS_OK ? ITPLD_CLI_TOKEN_CONTINUE : ITPLD_CLI_TOKEN_STOP;
		return stat;
	}

	if (strcmp(token, "-o") == 0) {
		stat =
		    itpld_cliopts_parse_value_option(idx, argc, argv, opts, ITPLD_CLI_VALOPT_OUTPUT, output_seen, err);
		*res = stat == ITPLD_STATUS_OK ? ITPLD_CLI_TOKEN_CONTINUE : ITPLD_CLI_TOKEN_STOP;
		return stat;
	}

	if (token[0] == '-') {
		itpld_fill_clierr(err, token, "unknown option");
		*res = ITPLD_CLI_TOKEN_STOP;
		return ITPLD_STATUS_INVAL_ARG;
	}

	*res = ITPLD_CLI_TOKEN_CONTINUE;
	return itpld_cliopts_append_input(opts, token, err);
}

itpld_status_t
itpld_cliopts_parse(int argc, char ** argv, itpld_cli_options_t * opts, itpld_cli_error_t * err)
{
	itpld_fill_clierr(err, NULL, NULL);

	itpld_status_t stat = itpld_cliopts_validate_parse_args(argc, argv, opts, err);
	if (stat != ITPLD_STATUS_OK) return stat;

	size_t argcnt;
	if (!itpld_int_size_cast(argc, &argcnt)) {
		itpld_fill_clierr(err, "cli", "'int -> size_t' cast error");
		return ITPLD_STATUS_OVERFLOW;
	}

	itpld_cli_options_t parsed;
	itpld_cliopts_init(&parsed);

	bool output_seen = false;
	bool entry_seen	 = false;

	size_t curarg = 1;
	while (curarg < argcnt) {
		itpld_cli_token_result_t tokenres;
		stat = itpld_cliopts_parse_token(
		    &curarg, argcnt, argv, &output_seen, &entry_seen, &parsed, &tokenres, err
		);
		if (tokenres == ITPLD_CLI_TOKEN_STOP) goto mrproper;

		curarg++;
	}

	if (parsed.action == ITPLD_CLI_ACTION_LINK && parsed.input_paths_size == 0) {
		itpld_fill_clierr(err, "cli", "expected input file paths, got no input file paths");
		stat = ITPLD_STATUS_INVAL_ARG;
		goto mrproper;
	}

	itpld_cliopts_destroy(opts);

	*opts = parsed;
	return stat;
mrproper:
	switch (parsed.action) {
	case ITPLD_CLI_ACTION_HELP:
	case ITPLD_CLI_ACTION_VERSION:
		itpld_cliopts_destroy(opts);
		opts->action = parsed.action;
	default:
		itpld_cliopts_destroy(&parsed);
	}
	return stat;
}

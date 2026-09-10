#include "cli/options.h"

#include "common/checked.h"
#include "common/status.h"

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
itpld_fill_clierr(itpld_cli_error_t * clierr, char const * arg, char const * msg)
{
	if (clierr != NULL) {
		clierr->argument = arg;
		clierr->message	 = msg;
	}
}

itpld_status_t
itpld_cliopts_parse(int argc, char ** argv, itpld_cli_options_t * opts, itpld_cli_error_t * err)
{
	itpld_fill_clierr(err, NULL, NULL);

	if (opts == NULL) {
		itpld_fill_clierr(err, NULL, "'opts' argument is null");
		return ITPLD_STATUS_INVAL_ARG;
	}

	if (argc < 1) {
		itpld_fill_clierr(err, NULL, "too few arguments");
		return ITPLD_STATUS_INVAL_ARG;
	}

	if (argv == NULL) {
		itpld_fill_clierr(err, NULL, "'argv' argument is null");
		return ITPLD_STATUS_INVAL_ARG;
	}

	size_t argcnt;
	if (!itpld_int_size_cast(argc, &argcnt)) {
		itpld_fill_clierr(err, NULL, "'int -> size_t' cast error");
		return ITPLD_STATUS_OVERFLOW;
	}

	itpld_cli_options_t cliopts;
	itpld_cliopts_init(&cliopts);

	itpld_status_t stat = ITPLD_STATUS_OK;

	bool output_seen = false;
	bool entry_seen	 = false;

	size_t curarg = 1;
	while (curarg < argcnt) {
		char const * token = argv[curarg];
		if (token == NULL) {
			itpld_fill_clierr(err, NULL, "null option detected");
			stat = ITPLD_STATUS_INVAL_ARG;
			goto mrproper;
		}
		if (token[0] == '\0') {
			itpld_fill_clierr(err, NULL, "empty option detected");
			stat = ITPLD_STATUS_INVAL_ARG;
			goto mrproper;
		}

		if (strcmp(token, "-h") == 0) {
			cliopts.action = ITPLD_CLI_ACTION_HELP;
			stat	       = ITPLD_STATUS_OK;
			goto mrproper;
		}
		if (strcmp(token, "-v") == 0) {
			cliopts.action = ITPLD_CLI_ACTION_VERSION;
			stat	       = ITPLD_STATUS_OK;
			goto mrproper;
		}

		if (strcmp(token, "-e") == 0) {
			if (curarg + 1 >= argcnt) {
				itpld_fill_clierr(err, token, "expected entry symbol name");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}
			if (entry_seen) {
				itpld_fill_clierr(err, token, "duplicate entry option");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}

			char const * entrysym = argv[++curarg];
			if (entrysym == NULL) {
				itpld_fill_clierr(err, token, "null argument detected");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}
			if (entrysym[0] == '\0') {
				itpld_fill_clierr(err, token, "empty entry symbol");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}
			if (entrysym[0] == '-') {
				itpld_fill_clierr(err, token, "expected entry symbol name, got option");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}

			cliopts.entry_symbol = entrysym;
			entry_seen	     = true;
		} else if (strcmp(token, "-o") == 0) {
			if (curarg + 1 >= argcnt) {
				itpld_fill_clierr(err, token, "expected output file path");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}
			if (output_seen) {
				itpld_fill_clierr(err, token, "duplicate output option");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}

			char const * output_path = argv[++curarg];
			if (output_path == NULL) {
				itpld_fill_clierr(err, token, "null argument detected");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}
			if (output_path[0] == '\0') {
				itpld_fill_clierr(err, token, "empty output file path");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}
			if (output_path[0] == '-') {
				itpld_fill_clierr(err, token, "expected output file path, got option");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}

			cliopts.output_path = output_path;
			output_seen	    = true;
		} else {
			if (token[0] == '-') {
				itpld_fill_clierr(err, token, "unknown option");
				stat = ITPLD_STATUS_INVAL_ARG;
				goto mrproper;
			}

			size_t newsz;
			if (!itpld_size_add(cliopts.input_paths_size, 1, &newsz)) {
				itpld_fill_clierr(
				    err, token, "failed to increase the size of internal buffer for new input file path"
				);
				stat = ITPLD_STATUS_OVERFLOW;
				goto mrproper;
			}
			size_t newsz_bytes;
			if (!itpld_size_mul(newsz, sizeof(*cliopts.input_paths), &newsz_bytes)) {
				itpld_fill_clierr(
				    err, token, "failed to increase the size of internal buffer for new input file path"
				);
				stat = ITPLD_STATUS_OVERFLOW;
				goto mrproper;
			}
			char const ** new_input_paths =
			    (char const **)realloc((void *)cliopts.input_paths, newsz_bytes);
			if (new_input_paths == NULL) {
				itpld_fill_clierr(err, token, "failed to allocate memory for new input file path");
				stat = ITPLD_STATUS_OUT_OF_MEM;
				goto mrproper;
			}

			new_input_paths[cliopts.input_paths_size] = token;

			cliopts.input_paths	 = new_input_paths;
			cliopts.input_paths_size = newsz;
		}

		curarg++;
	}

	if (cliopts.action == ITPLD_CLI_ACTION_LINK && cliopts.input_paths_size == 0) {
		itpld_fill_clierr(err, NULL, "expected input file paths, got no input file paths");
		stat = ITPLD_STATUS_INVAL_ARG;
		goto mrproper;
	}

	itpld_cliopts_destroy(opts);

	*opts = cliopts;
	return stat;
mrproper:
	switch (cliopts.action) {
	case ITPLD_CLI_ACTION_HELP:
	case ITPLD_CLI_ACTION_VERSION:
		itpld_cliopts_destroy(opts);
		opts->action = cliopts.action;
	default:
		itpld_cliopts_destroy(&cliopts);
	}
	return stat;
}

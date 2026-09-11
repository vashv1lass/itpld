#include "options_test.h"

#include "cli/options.h"
#include "common/compiler.h"
#include "common/status.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
itpld_cliopts_init_test_ok(void)
{
	char const *	    rsp;
	itpld_cli_options_t opts = { .output_path      = "uninitialized",
				     .input_paths      = &rsp,
				     .input_paths_size = 176,
				     .entry_symbol     = "uninitialized",
				     .action	       = ITPLD_CLI_ACTION_VERSION };

	itpld_cliopts_init(&opts);
	assert(
	    strcmp(opts.output_path, "a.out") == 0 && strcmp(opts.entry_symbol, "_start") == 0 &&
	    opts.input_paths == NULL && opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_init_test_null_guards(void)
{
	itpld_cliopts_init(NULL);
}

void
itpld_cliopts_init_test(void)
{
	itpld_cliopts_init_test_ok();
	itpld_cliopts_init_test_null_guards();
}

static void
itpld_cliopts_destroy_test_ok(void)
{
	itpld_cli_options_t opts = { .output_path      = "a.out",
				     .input_paths      = NULL,
				     .input_paths_size = 0,
				     .entry_symbol     = "_start",
				     .action	       = ITPLD_CLI_ACTION_LINK };

	opts.input_paths = (char const **)malloc(16 * sizeof(*opts.input_paths));
	assert(opts.input_paths != NULL);
	opts.input_paths_size = 16;

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_destroy_test_null_guards(void)
{
	itpld_cliopts_destroy(NULL);
}

static void
itpld_cliopts_destroy_test_twice(void)
{
	itpld_cli_options_t opts = { .output_path      = "a.out",
				     .input_paths      = NULL,
				     .input_paths_size = 0,
				     .entry_symbol     = "_start",
				     .action	       = ITPLD_CLI_ACTION_LINK };

	opts.input_paths = (char const **)malloc(16 * sizeof(*opts.input_paths));
	assert(opts.input_paths != NULL);
	opts.input_paths_size = 16;

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_destroy_test_empty(void)
{
	itpld_cli_options_t opts = { 0 };

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

void
itpld_cliopts_destroy_test(void)
{
	itpld_cliopts_destroy_test_ok();
	itpld_cliopts_destroy_test_null_guards();
	itpld_cliopts_destroy_test_twice();
	itpld_cliopts_destroy_test_empty();
}

static void
itpld_cliopts_parse_test_ok_default(void)
{
	int		    argc   = 4;
	char *		    argv[] = { "itpld", "in0.o", "in1.o", "in2.o" };
	itpld_cli_options_t opts   = { .output_path	 = "a.out",
				       .input_paths	 = NULL,
				       .input_paths_size = 0,
				       .entry_symbol	 = "_start",
				       .action		 = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err	   = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_OK);
	assert(
	    strcmp(opts.output_path, "a.out") == 0 && strcmp(opts.entry_symbol, "_start") == 0 &&
	    opts.input_paths_size == 3 && opts.action == ITPLD_CLI_ACTION_LINK
	);
	for (size_t i = 0; i < opts.input_paths_size; i++) {
		char curin[16];
		ITPLD_RETVAL_IGNORED(snprintf(curin, sizeof(curin), "in%zu.o", i));

		assert(strcmp(curin, opts.input_paths[i]) == 0);
	}
	assert(err.context == NULL && err.message == NULL);

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_oe_ok(void)
{
	int		    argc_oe   = 7;
	char *		    argv_oe[] = { "itpld", "-e", "constr", "-o", "exec", "in0.o", "in1.o" };
	itpld_cli_options_t opts_oe   = { .output_path	    = "a.out",
					  .input_paths	    = NULL,
					  .input_paths_size = 0,
					  .entry_symbol	    = "_start",
					  .action	    = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_oe    = { 0 };

	assert(itpld_cliopts_parse(argc_oe, argv_oe, &opts_oe, &err_oe) == ITPLD_STATUS_OK);
	assert(
	    strcmp(opts_oe.output_path, "exec") == 0 && strcmp(opts_oe.entry_symbol, "constr") == 0 &&
	    opts_oe.input_paths_size == 2 && opts_oe.action == ITPLD_CLI_ACTION_LINK
	);
	for (size_t i = 0; i < opts_oe.input_paths_size; i++) {
		char curin[16];
		ITPLD_RETVAL_IGNORED(snprintf(curin, sizeof(curin), "in%zu.o", i));

		assert(strcmp(curin, opts_oe.input_paths[i]) == 0);
	}
	assert(err_oe.context == NULL && err_oe.message == NULL);

	itpld_cliopts_destroy(&opts_oe);
	assert(
	    opts_oe.output_path == NULL && opts_oe.entry_symbol == NULL && opts_oe.input_paths == NULL &&
	    opts_oe.input_paths_size == 0 && opts_oe.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_oe_duplicate(void)
{
	int		    argc_e   = 7;
	char *		    argv_e[] = { "itpld", "-e", "constr0", "-e", "constr1", "in0.o", "in1.o" };
	itpld_cli_options_t opts_e   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_e    = { 0 };

	int		    argc_o   = 7;
	char *		    argv_o[] = { "itpld", "-o", "exec0", "-o", "exec1", "in0.o", "in1.o" };
	itpld_cli_options_t opts_o   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_o    = { 0 };

	assert(itpld_cliopts_parse(argc_e, argv_e, &opts_e, &err_e) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_e.context, "-e") == 0 && strcmp(err_e.message, "duplicate entry option") == 0);

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_o.context, "-o") == 0 && strcmp(err_o.message, "duplicate output option") == 0);

	itpld_cliopts_destroy(&opts_e);
	assert(
	    opts_e.output_path == NULL && opts_e.entry_symbol == NULL && opts_e.input_paths == NULL &&
	    opts_e.input_paths_size == 0 && opts_e.action == ITPLD_CLI_ACTION_LINK
	);

	itpld_cliopts_destroy(&opts_o);
	assert(
	    opts_o.output_path == NULL && opts_o.entry_symbol == NULL && opts_o.input_paths == NULL &&
	    opts_o.input_paths_size == 0 && opts_o.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_oe_missing(void)
{
	int		    argc_e   = 4;
	char *		    argv_e[] = { "itpld", "in0.o", "in1.o", "-e" };
	itpld_cli_options_t opts_e   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_e    = { 0 };

	int		    argc_o   = 4;
	char *		    argv_o[] = { "itpld", "in0.o", "in1.o", "-o" };
	itpld_cli_options_t opts_o   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_o    = { 0 };

	assert(itpld_cliopts_parse(argc_e, argv_e, &opts_e, &err_e) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_e.context, "-e") == 0 && strcmp(err_e.message, "expected entry symbol name") == 0);

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_o.context, "-o") == 0 && strcmp(err_o.message, "expected output file path") == 0);

	itpld_cliopts_destroy(&opts_e);
	assert(
	    opts_e.output_path == NULL && opts_e.entry_symbol == NULL && opts_e.input_paths == NULL &&
	    opts_e.input_paths_size == 0 && opts_e.action == ITPLD_CLI_ACTION_LINK
	);

	itpld_cliopts_destroy(&opts_o);
	assert(
	    opts_o.output_path == NULL && opts_o.entry_symbol == NULL && opts_o.input_paths == NULL &&
	    opts_o.input_paths_size == 0 && opts_o.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_oe_null_arg(void)
{
	int		    argc_e   = 5;
	char *		    argv_e[] = { "itpld", "in0.o", "in1.o", "-e", NULL };
	itpld_cli_options_t opts_e   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_e    = { 0 };

	int		    argc_o   = 5;
	char *		    argv_o[] = { "itpld", "in0.o", "in1.o", "-o", NULL };
	itpld_cli_options_t opts_o   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_o    = { 0 };

	assert(itpld_cliopts_parse(argc_e, argv_e, &opts_e, &err_e) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_e.context, "-e") == 0 && strcmp(err_e.message, "null argument detected") == 0);

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_o.context, "-o") == 0 && strcmp(err_o.message, "null argument detected") == 0);

	itpld_cliopts_destroy(&opts_e);
	assert(
	    opts_e.output_path == NULL && opts_e.entry_symbol == NULL && opts_e.input_paths == NULL &&
	    opts_e.input_paths_size == 0 && opts_e.action == ITPLD_CLI_ACTION_LINK
	);

	itpld_cliopts_destroy(&opts_o);
	assert(
	    opts_o.output_path == NULL && opts_o.entry_symbol == NULL && opts_o.input_paths == NULL &&
	    opts_o.input_paths_size == 0 && opts_o.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_oe_empty_arg(void)
{
	int		    argc_e   = 5;
	char *		    argv_e[] = { "itpld", "in0.o", "in1.o", "-e", "" };
	itpld_cli_options_t opts_e   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_e    = { 0 };

	int		    argc_o   = 5;
	char *		    argv_o[] = { "itpld", "in0.o", "in1.o", "-o", "" };
	itpld_cli_options_t opts_o   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_o    = { 0 };

	assert(itpld_cliopts_parse(argc_e, argv_e, &opts_e, &err_e) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_e.context, "-e") == 0 && strcmp(err_e.message, "empty entry symbol") == 0);

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_o.context, "-o") == 0 && strcmp(err_o.message, "empty output file path") == 0);

	itpld_cliopts_destroy(&opts_e);
	assert(
	    opts_e.output_path == NULL && opts_e.entry_symbol == NULL && opts_e.input_paths == NULL &&
	    opts_e.input_paths_size == 0 && opts_e.action == ITPLD_CLI_ACTION_LINK
	);

	itpld_cliopts_destroy(&opts_o);
	assert(
	    opts_o.output_path == NULL && opts_o.entry_symbol == NULL && opts_o.input_paths == NULL &&
	    opts_o.input_paths_size == 0 && opts_o.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_oe_key(void)
{
	int		    argc_e   = 5;
	char *		    argv_e[] = { "itpld", "in0.o", "in1.o", "-e", "-o" };
	itpld_cli_options_t opts_e   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_e    = { 0 };

	int		    argc_o   = 5;
	char *		    argv_o[] = { "itpld", "in0.o", "in1.o", "-o", "-e" };
	itpld_cli_options_t opts_o   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_o    = { 0 };

	assert(itpld_cliopts_parse(argc_e, argv_e, &opts_e, &err_e) == ITPLD_STATUS_INVAL_ARG);
	assert(
	    strcmp(err_e.context, "-e") == 0 && strcmp(err_e.message, "expected entry symbol name, got option") == 0
	);

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err_o.context, "-o") == 0 && strcmp(err_o.message, "expected output file path, got option") == 0);

	itpld_cliopts_destroy(&opts_e);
	assert(
	    opts_e.output_path == NULL && opts_e.entry_symbol == NULL && opts_e.input_paths == NULL &&
	    opts_e.input_paths_size == 0 && opts_e.action == ITPLD_CLI_ACTION_LINK
	);

	itpld_cliopts_destroy(&opts_o);
	assert(
	    opts_o.output_path == NULL && opts_o.entry_symbol == NULL && opts_o.input_paths == NULL &&
	    opts_o.input_paths_size == 0 && opts_o.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_null_opt(void)
{
	int		    argc   = 3;
	char *		    argv[] = { "itpld", NULL, "in0.o" };
	itpld_cli_options_t opts   = { .output_path	 = "a.out",
				       .input_paths	 = NULL,
				       .input_paths_size = 0,
				       .entry_symbol	 = "_start",
				       .action		 = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err	   = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err.context, "cli") == 0 && strcmp(err.message, "null option detected") == 0);

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_empty_opt(void)
{
	int		    argc   = 3;
	char *		    argv[] = { "itpld", "", "in0.o" };
	itpld_cli_options_t opts   = { .output_path	 = "a.out",
				       .input_paths	 = NULL,
				       .input_paths_size = 0,
				       .entry_symbol	 = "_start",
				       .action		 = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err	   = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err.context, "cli") == 0 && strcmp(err.message, "empty option detected") == 0);

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_unknown_opt(void)
{
	int		    argc   = 4;
	char *		    argv[] = { "itpld", "in0.o", "-x", "a.out" };
	itpld_cli_options_t opts   = { .output_path	 = "a.out",
				       .input_paths	 = NULL,
				       .input_paths_size = 0,
				       .entry_symbol	 = "_start",
				       .action		 = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err	   = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err.context, "-x") == 0 && strcmp(err.message, "unknown option") == 0);

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_hv(void)
{
	int		    argc_h   = 4;
	char *		    argv_h[] = { "itpld", "in0.o", "-h", "in1.o" };
	itpld_cli_options_t opts_h   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_h    = { 0 };

	int		    argc_v   = 4;
	char *		    argv_v[] = { "itpld", "in0.o", "-v", "in1.o" };
	itpld_cli_options_t opts_v   = { .output_path	   = "a.out",
					 .input_paths	   = NULL,
					 .input_paths_size = 0,
					 .entry_symbol	   = "_start",
					 .action	   = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err_v    = { 0 };

	assert(itpld_cliopts_parse(argc_h, argv_h, &opts_h, &err_h) == ITPLD_STATUS_OK);
	assert(opts_h.action == ITPLD_CLI_ACTION_HELP);
	assert(err_h.context == NULL && err_h.message == NULL);

	assert(itpld_cliopts_parse(argc_v, argv_v, &opts_v, &err_v) == ITPLD_STATUS_OK);
	assert(opts_v.action == ITPLD_CLI_ACTION_VERSION);
	assert(err_v.context == NULL && err_v.message == NULL);

	itpld_cliopts_destroy(&opts_h);
	assert(
	    opts_h.output_path == NULL && opts_h.entry_symbol == NULL && opts_h.input_paths == NULL &&
	    opts_h.input_paths_size == 0 && opts_h.action == ITPLD_CLI_ACTION_LINK
	);

	itpld_cliopts_destroy(&opts_v);
	assert(
	    opts_v.output_path == NULL && opts_v.entry_symbol == NULL && opts_v.input_paths == NULL &&
	    opts_v.input_paths_size == 0 && opts_v.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_no_input(void)
{
	int		    argc   = 5;
	char *		    argv[] = { "itpld", "-e", "constr", "-o", "exec" };
	itpld_cli_options_t opts   = { .output_path	 = "a.out",
				       .input_paths	 = NULL,
				       .input_paths_size = 0,
				       .entry_symbol	 = "_start",
				       .action		 = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err	   = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(
	    strcmp(err.context, "cli") == 0 &&
	    strcmp(err.message, "expected input file paths, got no input file paths") == 0
	);

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_preserves(void)
{
	itpld_cli_options_t opts = { .output_path      = "exec",
				     .input_paths      = NULL,
				     .input_paths_size = 2,
				     .entry_symbol     = "constr",
				     .action	       = ITPLD_CLI_ACTION_LINK };
	opts.input_paths	 = (char const **)malloc(opts.input_paths_size * sizeof(*opts.input_paths));
	assert(opts.input_paths != NULL);
	opts.input_paths[0] = "in0.o";
	opts.input_paths[1] = "in1.o";

	int		  argc	 = 3;
	char *		  argv[] = { "itpld", NULL, "garbage" };
	itpld_cli_error_t err	 = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(strcmp(err.context, "cli") == 0 && strcmp(err.message, "null option detected") == 0);

	assert(
	    strcmp(opts.output_path, "exec") == 0 && strcmp(opts.entry_symbol, "constr") == 0 &&
	    opts.input_paths_size == 2 && opts.action == ITPLD_CLI_ACTION_LINK
	);
	for (size_t i = 0; i < opts.input_paths_size; i++) {
		char curin[16];
		ITPLD_RETVAL_IGNORED(snprintf(curin, sizeof(curin), "in%zu.o", i));

		assert(strcmp(curin, opts.input_paths[i]) == 0);
	}

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_parse_test_replaces(void)
{
	itpld_cli_options_t opts = { .output_path      = "exec",
				     .input_paths      = NULL,
				     .input_paths_size = 2,
				     .entry_symbol     = "constr",
				     .action	       = ITPLD_CLI_ACTION_LINK };
	opts.input_paths	 = (char const **)malloc(opts.input_paths_size * sizeof(*opts.input_paths));
	assert(opts.input_paths != NULL);
	opts.input_paths[0] = "in0.o";
	opts.input_paths[1] = "in1.o";

	int		  argc	 = 4;
	char *		  argv[] = { "itpld", "in0.o", "in1.o", "in2.o" };
	itpld_cli_error_t err	 = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_OK);
	assert(
	    strcmp(opts.output_path, "a.out") == 0 && strcmp(opts.entry_symbol, "_start") == 0 &&
	    opts.input_paths_size == 3 && opts.action == ITPLD_CLI_ACTION_LINK
	);
	for (size_t i = 0; i < opts.input_paths_size; i++) {
		char curin[16];
		ITPLD_RETVAL_IGNORED(snprintf(curin, sizeof(curin), "in%zu.o", i));

		assert(strcmp(curin, opts.input_paths[i]) == 0);
	}

	itpld_cliopts_destroy(&opts);
	assert(
	    opts.output_path == NULL && opts.entry_symbol == NULL && opts.input_paths == NULL &&
	    opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

void
itpld_cliopts_parse_test(void)
{
	itpld_cliopts_parse_test_ok_default();
	itpld_cliopts_parse_test_oe_ok();
	itpld_cliopts_parse_test_oe_missing();
	itpld_cliopts_parse_test_oe_duplicate();
	itpld_cliopts_parse_test_oe_empty_arg();
	itpld_cliopts_parse_test_oe_null_arg();
	itpld_cliopts_parse_test_oe_key();
	itpld_cliopts_parse_test_null_opt();
	itpld_cliopts_parse_test_empty_opt();
	itpld_cliopts_parse_test_unknown_opt();
	itpld_cliopts_parse_test_hv();
	itpld_cliopts_parse_test_no_input();
	itpld_cliopts_parse_test_preserves();
	itpld_cliopts_parse_test_replaces();

	// TODO: add null guards test
}

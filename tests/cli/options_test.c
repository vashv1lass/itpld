#include "cli/options_test.h"

#include "cli/options.h"
#include "common/status.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
itpld_cliopts_init_test_ok_(void)
{
	char const *	    rsp;
	itpld_cli_options_t opts = { .output_path      = "\xDE\xAD\xBE\xEF",
				     .input_paths      = &rsp,
				     .input_paths_size = 176,
				     .entry_symbol     = "\xDE\xAD\xBE\xEF",
				     .action	       = ITPLD_CLI_ACTION_VERSION };

	itpld_cliopts_init(&opts);
	assert(
	    strcmp(opts.output_path, "a.out") == 0 && strcmp(opts.entry_symbol, "_start") == 0 &&
	    opts.input_paths == NULL && opts.input_paths_size == 0 && opts.action == ITPLD_CLI_ACTION_LINK
	);
}

static void
itpld_cliopts_init_test_null_guards_(void)
{
	itpld_cliopts_init(NULL);
}

void
itpld_cliopts_init_test(void)
{
	itpld_cliopts_init_test_ok_();
	itpld_cliopts_init_test_null_guards_();
}

static itpld_cli_options_t
itpld_allocate_cliopts_fixture_(void)
{
	itpld_cli_options_t opts = { .output_path      = "exec",
				     .input_paths      = NULL,
				     .input_paths_size = 2,
				     .entry_symbol     = "constr",
				     .action	       = ITPLD_CLI_ACTION_LINK };

	opts.input_paths = (char const **)malloc(opts.input_paths_size * sizeof(*opts.input_paths));
	assert(opts.input_paths != NULL);
	opts.input_paths[0] = "in0.o";
	opts.input_paths[1] = "in1.o";

	return opts;
}

static bool
itpld_check_cliopts_empty_(itpld_cli_options_t opts)
{
	return (bool)(opts.output_path == NULL && opts.input_paths == NULL && opts.input_paths_size == 0 &&
		      opts.entry_symbol == NULL && opts.action == ITPLD_CLI_ACTION_LINK);
}

static void
itpld_cliopts_destroy_test_ok_(void)
{
	itpld_cli_options_t opts = itpld_allocate_cliopts_fixture_();

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_destroy_test_null_guards_(void)
{
	itpld_cliopts_destroy(NULL);
}

static void
itpld_cliopts_destroy_test_twice_(void)
{
	itpld_cli_options_t opts = itpld_allocate_cliopts_fixture_();

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_destroy_test_empty_(void)
{
	itpld_cli_options_t opts = { 0 };

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

void
itpld_cliopts_destroy_test(void)
{
	itpld_cliopts_destroy_test_ok_();
	itpld_cliopts_destroy_test_null_guards_();
	itpld_cliopts_destroy_test_twice_();
	itpld_cliopts_destroy_test_empty_();
}

static bool
itpld_check_clierr_(itpld_cli_error_t err, char const * expected_ctx, char const * expected_msg)
{
	if (expected_ctx == NULL || expected_msg == NULL) return false;
	return (bool)(strcmp(err.context, expected_ctx) == 0 && strcmp(err.message, expected_msg) == 0);
}

static bool
itpld_check_no_clierr_(itpld_cli_error_t err)
{
	return (bool)(err.context == NULL && err.message == NULL);
}

static bool
itpld_check_cliopts_equal_(
    itpld_cli_options_t cliopts,
    char const *	expected_output_path,
    char const **	expected_input_paths,
    size_t		expected_input_paths_size,
    char const *	expected_entry_symbol,
    itpld_cli_action_t	expected_action
)
{
	if (cliopts.output_path == NULL || expected_output_path == NULL) return false;
	if (cliopts.input_paths == NULL || expected_input_paths == NULL) return false;
	if (cliopts.entry_symbol == NULL || expected_entry_symbol == NULL) return false;

	if (strcmp(cliopts.output_path, expected_output_path) == 0 &&
	    strcmp(cliopts.entry_symbol, expected_entry_symbol) == 0 &&
	    cliopts.input_paths_size == expected_input_paths_size && cliopts.action == expected_action) {
		for (size_t i = 0; i < expected_input_paths_size; i++) {
			if (cliopts.input_paths[i] == NULL || expected_input_paths[i] == NULL) return false;
			if (strcmp(cliopts.input_paths[i], expected_input_paths[i]) != 0) return false;
		}

		return true;
	}

	return false;
}

static void
itpld_cliopts_parse_test_ok_default_(void)
{
	int		    argc   = 4;
	char *		    argv[] = { "itpld", "in0.o", "in1.o", "in2.o" };
	itpld_cli_options_t opts   = { .output_path	 = "a.out",
				       .input_paths	 = NULL,
				       .input_paths_size = 0,
				       .entry_symbol	 = "_start",
				       .action		 = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err	   = { 0 };

	char const * input_paths[] = { "in0.o", "in1.o", "in2.o" };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_OK);
	assert(itpld_check_cliopts_equal_(opts, "a.out", input_paths, 3, "_start", ITPLD_CLI_ACTION_LINK));
	assert(itpld_check_no_clierr_(err));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_oe_ok_(void)
{
	int		    argc   = 7;
	char *		    argv[] = { "itpld", "-e", "constr", "-o", "exec", "in0.o", "in1.o" };
	itpld_cli_options_t opts   = { .output_path	 = "a.out",
				       .input_paths	 = NULL,
				       .input_paths_size = 0,
				       .entry_symbol	 = "_start",
				       .action		 = ITPLD_CLI_ACTION_LINK };
	itpld_cli_error_t   err	   = { 0 };

	char const * input_paths[] = { "in0.o", "in1.o" };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_OK);
	assert(itpld_check_cliopts_equal_(opts, "exec", input_paths, 2, "constr", ITPLD_CLI_ACTION_LINK));
	assert(itpld_check_no_clierr_(err));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_oe_duplicate_(void)
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
	assert(itpld_check_clierr_(err_e, "-e", "duplicate entry option"));

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err_o, "-o", "duplicate output option"));

	itpld_cliopts_destroy(&opts_e);
	assert(itpld_check_cliopts_empty_(opts_e));

	itpld_cliopts_destroy(&opts_o);
	assert(itpld_check_cliopts_empty_(opts_o));
}

static void
itpld_cliopts_parse_test_oe_missing_(void)
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
	assert(itpld_check_clierr_(err_e, "-e", "expected entry symbol name"));

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err_o, "-o", "expected output file path"));

	itpld_cliopts_destroy(&opts_e);
	assert(itpld_check_cliopts_empty_(opts_e));

	itpld_cliopts_destroy(&opts_o);
	assert(itpld_check_cliopts_empty_(opts_o));
}

static void
itpld_cliopts_parse_test_oe_null_arg_(void)
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
	assert(itpld_check_clierr_(err_e, "-e", "null argument detected"));

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err_o, "-o", "null argument detected"));

	itpld_cliopts_destroy(&opts_e);
	assert(itpld_check_cliopts_empty_(opts_e));

	itpld_cliopts_destroy(&opts_o);
	assert(itpld_check_cliopts_empty_(opts_o));
}

static void
itpld_cliopts_parse_test_oe_empty_arg_(void)
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
	assert(itpld_check_clierr_(err_e, "-e", "empty entry symbol"));

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err_o, "-o", "empty output file path"));

	itpld_cliopts_destroy(&opts_e);
	assert(itpld_check_cliopts_empty_(opts_e));

	itpld_cliopts_destroy(&opts_o);
	assert(itpld_check_cliopts_empty_(opts_o));
}

static void
itpld_cliopts_parse_test_oe_key_(void)
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
	assert(itpld_check_clierr_(err_e, "-e", "expected entry symbol name, got option"));

	assert(itpld_cliopts_parse(argc_o, argv_o, &opts_o, &err_o) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err_o, "-o", "expected output file path, got option"));

	itpld_cliopts_destroy(&opts_e);
	assert(itpld_check_cliopts_empty_(opts_e));

	itpld_cliopts_destroy(&opts_o);
	assert(itpld_check_cliopts_empty_(opts_o));
}

static void
itpld_cliopts_parse_test_null_opt_(void)
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
	assert(itpld_check_clierr_(err, "cli", "null option detected"));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_empty_opt_(void)
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
	assert(itpld_check_clierr_(err, "cli", "empty option detected"));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_unknown_opt_(void)
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
	assert(itpld_check_clierr_(err, "-x", "unknown option"));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_hv_(void)
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
	assert(itpld_check_no_clierr_(err_h));

	assert(itpld_cliopts_parse(argc_v, argv_v, &opts_v, &err_v) == ITPLD_STATUS_OK);
	assert(opts_v.action == ITPLD_CLI_ACTION_VERSION);
	assert(itpld_check_no_clierr_(err_v));

	itpld_cliopts_destroy(&opts_h);
	assert(itpld_check_cliopts_empty_(opts_h));

	itpld_cliopts_destroy(&opts_v);
	assert(itpld_check_cliopts_empty_(opts_v));
}

static void
itpld_cliopts_parse_test_no_input_(void)
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
	assert(itpld_check_clierr_(err, "cli", "expected input file paths, got no input file paths"));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_preserves_(void)
{
	itpld_cli_options_t opts = itpld_allocate_cliopts_fixture_();

	char const * input_paths[] = { "in0.o", "in1.o" };

	int		  argc	 = 3;
	char *		  argv[] = { "itpld", NULL, "garbage" };
	itpld_cli_error_t err	 = { 0 };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err, "cli", "null option detected"));
	assert(itpld_check_cliopts_equal_(opts, "exec", input_paths, 2, "constr", ITPLD_CLI_ACTION_LINK));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_replaces_(void)
{
	itpld_cli_options_t opts = itpld_allocate_cliopts_fixture_();

	int		  argc	 = 4;
	char *		  argv[] = { "itpld", "in0.o", "in1.o", "in2.o" };
	itpld_cli_error_t err	 = { 0 };

	char const * input_paths[] = { "in0.o", "in1.o", "in2.o" };

	assert(itpld_cliopts_parse(argc, argv, &opts, &err) == ITPLD_STATUS_OK);
	assert(itpld_check_cliopts_equal_(opts, "a.out", input_paths, 3, "_start", ITPLD_CLI_ACTION_LINK));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

static void
itpld_cliopts_parse_test_arg_guards_(void)
{
	itpld_cli_options_t opts = itpld_allocate_cliopts_fixture_();

	int		  argc	 = 4;
	char *		  argv[] = { "itpld", "in0.o", "in1.o", "in2.o" };
	itpld_cli_error_t err	 = { 0 };

	char * argv_inval_exec_null[] = { NULL, "in0.o", "in1.o", "in2.o" };

	char const * input_paths[] = { "in0.o", "in1.o", "in2.o" };

	assert(itpld_cliopts_parse(argc, argv, NULL, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err, "cli", "'opts' argument is null"));

	assert(itpld_cliopts_parse(-1, argv, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err, "cli", "too few arguments"));

	assert(itpld_cliopts_parse(argc, NULL, &opts, &err) == ITPLD_STATUS_INVAL_ARG);
	assert(itpld_check_clierr_(err, "cli", "'argv' argument is null"));

	assert(itpld_cliopts_parse(argc, argv, &opts, NULL) == ITPLD_STATUS_OK);
	assert(itpld_check_cliopts_equal_(opts, "a.out", input_paths, 3, "_start", ITPLD_CLI_ACTION_LINK));

	assert(itpld_cliopts_parse(argc, argv_inval_exec_null, &opts, &err));
	assert(itpld_check_clierr_(err, "cli", "'argv[0]' argument is null"));

	itpld_cliopts_destroy(&opts);
	assert(itpld_check_cliopts_empty_(opts));
}

void
itpld_cliopts_parse_test(void)
{
	itpld_cliopts_parse_test_ok_default_();
	itpld_cliopts_parse_test_oe_ok_();
	itpld_cliopts_parse_test_oe_missing_();
	itpld_cliopts_parse_test_oe_duplicate_();
	itpld_cliopts_parse_test_oe_empty_arg_();
	itpld_cliopts_parse_test_oe_null_arg_();
	itpld_cliopts_parse_test_oe_key_();
	itpld_cliopts_parse_test_null_opt_();
	itpld_cliopts_parse_test_empty_opt_();
	itpld_cliopts_parse_test_unknown_opt_();
	itpld_cliopts_parse_test_hv_();
	itpld_cliopts_parse_test_no_input_();
	itpld_cliopts_parse_test_preserves_();
	itpld_cliopts_parse_test_replaces_();
	itpld_cliopts_parse_test_arg_guards_();
}

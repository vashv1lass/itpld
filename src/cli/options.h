#ifndef ITPLD_CLI_OPTIONS_H_
#define ITPLD_CLI_OPTIONS_H_

#include "common/status.h"

#include <stddef.h>

typedef enum itpld_cli_action {
	ITPLD_CLI_ACTION_LINK,
	ITPLD_CLI_ACTION_VERSION,
	ITPLD_CLI_ACTION_HELP
} itpld_cli_action_t;

typedef struct itpld_cli_options {
	char const * output_path;

	char const ** input_paths;
	size_t	      input_paths_size;

	char const * entry_symbol;

	itpld_cli_action_t action;
} itpld_cli_options_t;

typedef struct itpld_cli_error {
	char const * context;
	char const * message;
} itpld_cli_error_t;

extern char const ITPLD_HELP_MESSAGE[];
extern char const ITPLD_VERSION_MESSAGE[];

void
itpld_cliopts_init(itpld_cli_options_t * opts);

void
itpld_cliopts_destroy(itpld_cli_options_t * opts);

/*
 *  parser grammar
 *  -h - help (only short)
 *  -v - version (only short)
 *  -o OUTPUT
 *  -e ENTRY_SYMBOL
 *  no key = input file
 *  -- option is not supported!
 *
 *  opts must be initialized via itpld_cliopts_init() before calling parse
 *  first -h or -v option terminate parsing and ignore all following arguments
 *  if parsing fails, nothing will be written to opts
 *  when there is -h or -v options, the only valid value will be opts->action
 */
itpld_status_t
itpld_cliopts_parse(int argc, char ** argv, itpld_cli_options_t * opts, itpld_cli_error_t * err);

#endif /* ITPLD_CLI_OPTIONS_H_ */

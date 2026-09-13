#include "itpld/itpld.h"

#include "cli/options.h"
#include "common/compiler.h"
#include "common/diagnostics.h"
#include "common/status.h"

#include <stdio.h>
#include <stdlib.h>

int
itpld_run(int argc, char ** argv)
{
	itpld_cli_options_t cliopts;
	itpld_cli_error_t   clierr;

	itpld_cliopts_init(&cliopts);

	itpld_status_t stat	= itpld_cliopts_parse(argc, argv, &cliopts, &clierr);
	int	       exitcode = EXIT_SUCCESS;

	if (stat == ITPLD_STATUS_OK) {
		switch (cliopts.action) {
		case ITPLD_CLI_ACTION_HELP:
			puts(ITPLD_HELP_MESSAGE);
			goto mrproper;
		case ITPLD_CLI_ACTION_VERSION:
			puts(ITPLD_VERSION_MESSAGE);
			goto mrproper;
		case ITPLD_CLI_ACTION_LINK:
			stat	 = ITPLD_STATUS_UNSUPPORTED;
			exitcode = EXIT_FAILURE;
			itpld_error_report(stderr, stat, "linker-pipeline", "not implemented yet");
			goto mrproper;
		default:
			itpld_dead_code();
		}
	} else {
		exitcode = EXIT_FAILURE;
		itpld_error_report(stderr, stat, clierr.context, clierr.message);
		goto mrproper;
	}

mrproper:
	itpld_cliopts_destroy(&cliopts);
	return exitcode;
}

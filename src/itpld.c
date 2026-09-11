#include "itpld/itpld.h"

#include "cli/options.h"
#include "common/compiler.h"
#include "common/diagnostics.h"
#include "common/status.h"

#include <errno.h>
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
			exitcode = EXIT_SUCCESS;
			goto mrproper;
		case ITPLD_CLI_ACTION_VERSION:
			puts(ITPLD_VERSION_MESSAGE);
			exitcode = EXIT_SUCCESS;
			goto mrproper;
		case ITPLD_CLI_ACTION_LINK:
			exitcode = EXIT_SUCCESS;
			goto mrproper;
		default:
			ITPLD_DEAD_CODE();
		}
	} else {
		itpld_error_report(stderr, stat, clierr.context, clierr.message);

		switch (stat) {
		case ITPLD_STATUS_INVAL_ARG:
		case ITPLD_STATUS_INVAL_FMT:
			exitcode = EINVAL;
			goto mrproper;
		case ITPLD_STATUS_OUT_OF_MEM:
			exitcode = ENOMEM;
			goto mrproper;
		case ITPLD_STATUS_IO_ERROR:
			exitcode = EIO;
			goto mrproper;
		case ITPLD_STATUS_RELOC_TYPE_UNSUPPORTED:
			exitcode = ENOSYS;
			goto mrproper;
		case ITPLD_STATUS_OVERFLOW:
			exitcode = EOVERFLOW;
			goto mrproper;
		default:
			ITPLD_DEAD_CODE();
		}
	}

mrproper:
	itpld_cliopts_destroy(&cliopts);
	return exitcode;
}

#include "testconf.h"

#include "cli/options_test.h"
#include "common/checked_test.h"
#include "common/compiler.h"
#include "common/diagnostics_test.h"
#include "common/file_test.h"
#include "common/utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sync the test registry with the test list in tests/CMakeLists.txt
itpld_test_registry_entry_t const ITPLD_TEST_REGISTRY[] = {
	{ .name = "in_range",	      .func = itpld_in_range_test	  },
	{ .name = "int_size_cast",   .func = itpld_int_size_cast_test   },
	{ .name = "long_size_cast",  .func = itpld_long_size_cast_test  },
	{ .name = "size_add",	      .func = itpld_size_add_test	  },
	{ .name = "size_mul",	      .func = itpld_size_mul_test	  },
	{ .name = "filebuf_init",	  .func = itpld_filebuf_init_test	  },
	{ .name = "filebuf_destroy", .func = itpld_filebuf_destroy_test },
	{ .name = "file_read",       .func = itpld_file_read_test	    },
	{ .name = "status_str",	.func = itpld_status_str_test      },
	{ .name = "error_report",	  .func = itpld_error_report_test	  },
	{ .name = "cliopts_init",	  .func = itpld_cliopts_init_test	  },
	{ .name = "cliopts_destroy", .func = itpld_cliopts_destroy_test },
	{ .name = "cliopts_parse",   .func = itpld_cliopts_parse_test   }
};

int
main(int argc, char ** argv)
{
	if (argc != 2) {
		ITPLD_RETVAL_IGNORED(fprintf(stderr, "usage: %s <test name>\n", argv[0]));
		return EXIT_FAILURE;
	}

	char const * testname = argv[1];
	for (size_t i = 0; i < ITPLD_STATIC_ARRAY_SIZE(ITPLD_TEST_REGISTRY); i++)
		if (strcmp(testname, ITPLD_TEST_REGISTRY[i].name) == 0) {
			ITPLD_TEST_REGISTRY[i].func();
			return EXIT_SUCCESS;
		}

	ITPLD_RETVAL_IGNORED(fprintf(stderr, "unknown test: %s. available test names:\n", argv[1]));
	for (size_t i = 0; i < ITPLD_STATIC_ARRAY_SIZE(ITPLD_TEST_REGISTRY); i++)
		ITPLD_RETVAL_IGNORED(fprintf(stderr, "%s\n", ITPLD_TEST_REGISTRY[i].name));
	return EXIT_FAILURE;
}

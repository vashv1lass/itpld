#include "diagnostics_test.h"

#include "common/compiler.h"
#include "common/diagnostics.h"
#include "common/status.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void
itpld_status_str_test(void)
{
	assert(strcmp(itpld_status_str(ITPLD_STATUS_OK), "ok") == 0);
	assert(strcmp(itpld_status_str(ITPLD_STATUS_INVAL_ARG), "invalid argument") == 0);
	assert(strcmp(itpld_status_str(ITPLD_STATUS_OUT_OF_MEM), "out of memory") == 0);
	assert(strcmp(itpld_status_str(ITPLD_STATUS_IO_ERROR), "I/O error") == 0);
	assert(strcmp(itpld_status_str(ITPLD_STATUS_INVAL_FMT), "invalid format") == 0);
	assert(strcmp(itpld_status_str(ITPLD_STATUS_RELOC_TYPE_UNSUPPORTED), "unsupported relocation type") == 0);
	assert(strcmp(itpld_status_str(ITPLD_STATUS_OVERFLOW), "overflow error") == 0);
	/* NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange) */
	assert(strcmp(itpld_status_str((itpld_status_t)-1), "unknown error") == 0);
}

static void
itpld_error_report_test_ok(void)
{
	FILE * fd = tmpfile();
	assert(fd != NULL);

	itpld_error_report(fd, ITPLD_STATUS_OK, "unit testing diagnostics (general case)", "everything is fine");

	ITPLD_RETVAL_IGNORED(fflush(fd));
	ITPLD_RETVAL_IGNORED(fseek(fd, 0L, SEEK_SET));

	char buf[256] = { 0 };
	assert(fgets(buf, sizeof(buf), fd) != NULL);

	assert(strstr(buf, "unit testing diagnostics"));
	assert(strstr(buf, "ok"));
	assert(strstr(buf, "everything is fine"));

	ITPLD_RETVAL_IGNORED(fclose(fd));
}

static void
itpld_error_report_test_null_guards(void)
{
	itpld_error_report(
	    NULL, ITPLD_STATUS_INVAL_ARG, "itpld_error_report_test_null_guards()", "file descriptor is null"
	);

	FILE * fd = tmpfile();
	assert(fd != NULL);

	itpld_error_report(fd, ITPLD_STATUS_INVAL_ARG, NULL, "ctx is null");
	itpld_error_report(fd, ITPLD_STATUS_INVAL_ARG, "unit testing diagnostics (msg is null)", NULL);

	ITPLD_RETVAL_IGNORED(fflush(fd));
	ITPLD_RETVAL_IGNORED(fseek(fd, 0L, SEEK_SET));

	char buf[16] = { 0 };
	assert(fgets(buf, sizeof(buf), fd) == NULL && feof(fd));

	ITPLD_RETVAL_IGNORED(fclose(fd));
}

void
itpld_error_report_test(void)
{
	itpld_error_report_test_ok();
	itpld_error_report_test_null_guards();
}

#include "common/diagnostics.h"

#include "common/compiler.h"
#include "common/status.h"

#include <stdio.h>

char const *
itpld_status_str(itpld_status_t stat)
{
	switch (stat) {
	case ITPLD_STATUS_OK:
		return "ok";
	case ITPLD_STATUS_INVAL_ARG:
		return "invalid argument";
	case ITPLD_STATUS_OUT_OF_MEM:
		return "out of memory";
	case ITPLD_STATUS_IO_ERROR:
		return "I/O error";
	case ITPLD_STATUS_INVAL_FMT:
		return "invalid format";
	case ITPLD_STATUS_RELOC_TYPE_UNSUPPORTED:
		return "unsupported relocation type";
	case ITPLD_STATUS_OVERFLOW:
		return "overflow error";
	default:
		return "unknown error";
	}
}

void
itpld_error_report(FILE * restrict stream, itpld_status_t stat, char const * restrict ctx, char const * restrict msg)
{
	if (stream != NULL && ctx != NULL && msg != NULL) {
		ITPLD_RETVAL_UNUSED(fprintf(stream, "itpld error: %s: %s: %s\n", ctx, itpld_status_str(stat), msg));
	}
}

#ifndef ITPLD_COMMON_DIAGNOSTICS_H_
#define ITPLD_COMMON_DIAGNOSTICS_H_

#include "common/status.h"

#include <stdio.h>

char const *
itpld_status_str(itpld_status_t stat);

/* invalid arguments are silently ignored (defensive no-op) */
void
itpld_error_report(FILE * restrict stream, itpld_status_t stat, char const * restrict ctx, char const * restrict msg);

#endif /* ITPLD_COMMON_DIAGNOSTICS_H_ */

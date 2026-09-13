#ifndef ITPLD_COMMON_COMPILER_H_
#define ITPLD_COMMON_COMPILER_H_

#include <stdio.h>
#include <stdlib.h>

#define ITPLD_PARAM_UNUSED(param)  ((void)(param))
#define ITPLD_RETVAL_IGNORED(call) ((void)(call))

static inline void
itpld_dead_code(void)
{
	ITPLD_RETVAL_IGNORED(fprintf(stderr, "itpld FATAL ERROR: dead code reached. Aborting..."));
	abort();
}

#endif /* ITPLD_COMMON_COMPILER_H_ */

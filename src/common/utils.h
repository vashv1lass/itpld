#ifndef ITPLD_COMMON_UTILS_H_
#define ITPLD_COMMON_UTILS_H_

#include "common/status.h"

/* number of elements in an actual array. do not pass a pointer. */
#define ITPLD_STATIC_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define ITPLD_TRY(expr)                                                                                                \
	do {                                                                                                           \
		itpld_status_t stat_ = (expr);                                                                         \
		if (stat_ != ITPLD_STATUS_OK) return stat_;                                                            \
	} while (0)

#endif /* ITPLD_COMMON_UTILS_H_ */

#ifndef ITPLD_CHECKED_H_
#define ITPLD_CHECKED_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline bool
itpld_in_range(size_t offset, size_t sz, size_t bufsz)
{
	return (bool)(offset <= bufsz && sz <= bufsz - offset);
}

static inline bool
itpld_size_add(size_t x, size_t y, size_t * res)
{
	if (res == NULL) {
		return false;
	}

	*res = x + y;
	return *res >= x;
}

static inline bool
itpld_size_mul(size_t x, size_t y, size_t * res)
{
	if (res == NULL) {
		return false;
	}

	*res = x * y;
	return (bool)(x == 0 || y <= SIZE_MAX / x);
}

#endif /* ITPLD_CHECKED_H_ */

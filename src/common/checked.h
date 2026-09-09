#ifndef ITPLD_COMMON_CHECKED_H_
#define ITPLD_COMMON_CHECKED_H_

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static inline bool
itpld_in_range(size_t offset, size_t sz, size_t bufsz)
{
	return (bool)(offset <= bufsz && sz <= bufsz - offset);
}

static inline bool
itpld_int_size_cast(int x, size_t * xsz)
{
	if (x < 0 || xsz == NULL) {
		return false;
	}
#if INT_MAX > SIZE_MAX
	if (x > SIZE_MAX) {
		return false;
	}
#endif
	*xsz = (size_t)x;
	return true;
}

static inline bool
itpld_long_size_cast(long x, size_t * xsz)
{
	if (x < 0 || xsz == NULL) {
		return false;
	}
#if LONG_MAX > SIZE_MAX
	if (x > SIZE_MAX) {
		return false;
	}
#endif
	*xsz = (size_t)x;
	return true;
}

/* returns true on success and false if res == NULL or arithmetic overflows */
static inline bool
itpld_size_add(size_t x, size_t y, size_t * res)
{
	if (res == NULL || y > SIZE_MAX - x) return false;

	*res = x + y;
	return true;
}

/* returns true on success and false if res == NULL or arithmetic overflows */
static inline bool
itpld_size_mul(size_t x, size_t y, size_t * res)
{
	if (res == NULL || (x != 0 && y > SIZE_MAX / x)) return false;

	*res = x * y;
	return true;
}

#endif /* ITPLD_COMMON_CHECKED_H_ */

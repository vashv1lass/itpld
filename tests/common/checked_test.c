#include "checked_test.h"

#include "common/checked.h"

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

static void
itpld_in_range_test_true(void)
{
	assert(itpld_in_range(0, 0, 0));
	assert(itpld_in_range(0, 1, 1));
	assert(itpld_in_range(1, 0, 1));
	assert(itpld_in_range(2, 3, 5));
	assert(itpld_in_range(SIZE_MAX, 0, SIZE_MAX));
}

static void
itpld_in_range_test_false(void)
{
	assert(!itpld_in_range(5, 2, 3));
	assert(!itpld_in_range(1, 1, 1));
	assert(!itpld_in_range(5, 1, 5));
	assert(!itpld_in_range(4, 2, 5));
	assert(!itpld_in_range(0, SIZE_MAX, 5));
}

static void
itpld_in_range_test_overflow(void)
{
	assert(!itpld_in_range(SIZE_MAX, 5, SIZE_MAX));
	assert(!itpld_in_range(SIZE_MAX, 1, SIZE_MAX));
}

void
itpld_in_range_test(void)
{
	itpld_in_range_test_true();
	itpld_in_range_test_false();
	itpld_in_range_test_overflow();
}

static void
itpld_int_size_cast_test_ok(void)
{
#if INT_MAX > SIZE_MAX
	int    x0 = SIZE_MAX + 1;
	size_t x0sz;
	assert(itpld_int_size_cast(x0, &x0sz));
	assert(x0 == x0sz);
#endif
	int    x1 = 176;
	size_t x1sz;
	assert(itpld_int_size_cast(x1, &x1sz));
	assert((int)x1 == (int)x1sz);
}

static void
itpld_int_size_cast_test_bad(void)
{
#if INT_MAX > SIZE_MAX
	int    x0 = INT_MAX;
	size_t x0sz;
	assert(!itpld_int_size_cast(x0, &x0sz));
#endif
	int    x1 = -1;
	size_t x1sz;
	assert(!itpld_int_size_cast(x1, &x1sz));
}

static void
itpld_int_size_cast_test_null_guards(void)
{
	int x = 0;
	assert(!itpld_int_size_cast(x, NULL));
}

void
itpld_int_size_cast_test(void)
{
	itpld_int_size_cast_test_ok();
	itpld_int_size_cast_test_bad();
	itpld_int_size_cast_test_null_guards();
}

static void
itpld_long_size_cast_test_ok(void)
{
#if LONG_MAX > SIZE_MAX
	long   x0 = SIZE_MAX + 1;
	size_t x0sz;
	assert(itpld_long_size_cast(x0, &x0sz));
	assert(x0 == x0sz);
#endif
	long   x1 = 176;
	size_t x1sz;
	assert(itpld_long_size_cast(x1, &x1sz));
	assert((long)x1 == (long)x1sz);
}

static void
itpld_long_size_cast_test_bad(void)
{
#if LONG_MAX > SIZE_MAX
	long   x0 = LONG_MAX;
	size_t x0sz;
	assert(!itpld_long_size_cast(x0, &x0sz));
#endif
	long   x1 = -1;
	size_t x1sz;
	assert(!itpld_long_size_cast(x1, &x1sz));
}

static void
itpld_long_size_cast_test_null_guards(void)
{
	long x = 0;
	assert(!itpld_long_size_cast(x, NULL));
}

void
itpld_long_size_cast_test(void)
{
	itpld_long_size_cast_test_ok();
	itpld_long_size_cast_test_bad();
	itpld_long_size_cast_test_null_guards();
}

static void
itpld_size_add_test_ok(void)
{
	size_t sum;

	assert(itpld_size_add(1, 2, &sum) && sum == 3);
	assert(itpld_size_add(0, SIZE_MAX, &sum) && sum == SIZE_MAX);
}

static void
itpld_size_add_test_overflow(void)
{
	size_t sum = 176;

	assert(!itpld_size_add(SIZE_MAX, 1, &sum) && sum == 176);
	assert(!itpld_size_add(SIZE_MAX - 1, 2, &sum) && sum == 176);
}

static void
itpld_size_add_test_null_guards(void)
{
	assert(!itpld_size_add(0, 0, NULL));
}

void
itpld_size_add_test(void)
{
	itpld_size_add_test_ok();
	itpld_size_add_test_overflow();
	itpld_size_add_test_null_guards();
}

static void
itpld_size_mul_test_ok(void)
{
	size_t prod;

	assert(itpld_size_mul(0, SIZE_MAX, &prod) && prod == 0);
	assert(itpld_size_mul(1, SIZE_MAX, &prod) && prod == SIZE_MAX);
	assert(itpld_size_mul(2, 3, &prod) && prod == 6);
}

static void
itpld_size_mul_test_overflow(void)
{
	size_t prod = 176;

	assert(!itpld_size_mul(SIZE_MAX, 2, &prod) && prod == 176);
	assert(!itpld_size_mul(SIZE_MAX / 2 + 1, 2, &prod) && prod == 176);
}

static void
itpld_size_mul_test_null_guards(void)
{
	assert(!itpld_size_mul(1, 1, NULL));
}

void
itpld_size_mul_test(void)
{
	itpld_size_mul_test_ok();
	itpld_size_mul_test_overflow();
	itpld_size_mul_test_null_guards();
}

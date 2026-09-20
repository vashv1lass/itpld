#include "elf/elf_bytes_test.h"

#include "common/status.h"
#include "elf/elf_bytes.h"
#include "elf/elf_types.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static itpld_elf_bytes_t
itpld_allocate_bytes_fixture_(void)
{
	static itpld_elf_ubyte_t const default_data[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xAB, 0xAD, 0x1D, 0xEA,
							  0xCA, 0xFE, 0xBA, 0xBE, 0xBA, 0xAD, 0xF0, 0x0D };

	itpld_elf_bytes_t bytes = { 0 };

	bytes.bytescnt = sizeof(default_data);
	bytes.data     = (itpld_elf_ubyte_t *)malloc(bytes.bytescnt * sizeof(*bytes.data));
	assert(bytes.data != NULL);

	memcpy((itpld_elf_ubyte_t *)bytes.data, default_data, bytes.bytescnt);

	return bytes;
}

static void
itpld_destroy_bytes_fixture_(itpld_elf_bytes_t * bytes)
{
	free((void *)bytes->data);
	bytes->data = NULL;

	bytes->bytescnt = 0;
}

/* clang-format off */
#define ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS(type)                                                                \
	static void                                                                                                    \
        itpld_elf_read_##type##_test_guards_(void)                                                                     \
	{                                                                                                              \
		itpld_elf_bytes_t    bytes = itpld_allocate_bytes_fixture_();                                          \
		itpld_elf_##type##_t val;                                                                              \
                                                                                                                       \
		assert(itpld_elf_read_##type(bytes, 0, NULL) == ITPLD_STATUS_INVAL_ARG);                               \
		size_t bad_offset = bytes.bytescnt - sizeof(val) + 1;                                                  \
		assert(itpld_elf_read_##type(bytes, bad_offset, &val) == ITPLD_STATUS_INVAL_ARG);                      \
                                                                                                                       \
		itpld_destroy_bytes_fixture_(&bytes);                                                                  \
		assert(itpld_elf_read_##type(bytes, 0, &val) == ITPLD_STATUS_INVAL_ARG);                               \
	}
/* clang-format on */

ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS(ubyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS(utwobyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS(fourbyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS(ufourbyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS(eightbyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS(ueightbyte)

#undef ITPLD_DEFINE_ELF_READ_GENERIC_TEST_GUARDS

/* clang-format off */
#define ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK(type, offset, expected)                                                  \
	static void                                                                                                    \
	itpld_elf_read_##type##_test_ok_(void)                                                                         \
	{                                                                                                              \
		itpld_elf_bytes_t bytes = itpld_allocate_bytes_fixture_();                                             \
                                                                                                                       \
		itpld_elf_##type##_t val;                                                                              \
		assert(itpld_elf_read_##type(bytes, offset, &val) == ITPLD_STATUS_OK);                                 \
		assert(val == (itpld_elf_##type##_t)(expected));                                                       \
                                                                                                                       \
		itpld_destroy_bytes_fixture_(&bytes);                                                                  \
	}
/* clang-format on */

ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK(ubyte, 8, 0xCA)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK(utwobyte, 8, 0xFECA)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK(fourbyte, 8, 0xBEBAFECA)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK(ufourbyte, 8, 0xBEBAFECA)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK(eightbyte, 8, 0x0DF0ADBABEBAFECALL)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK(ueightbyte, 8, 0x0DF0ADBABEBAFECAULL)

#undef ITPLD_DEFINE_ELF_READ_GENERIC_TEST_OK

/* clang-format off */
#define ITPLD_DEFINE_ELF_READ_GENERIC_TEST(type)                                                                       \
	void                                                                                                           \
	itpld_elf_read_##type##_test(void)                                                                             \
	{                                                                                                              \
		itpld_elf_read_##type##_test_ok_();                                                                    \
		itpld_elf_read_##type##_test_guards_();                                                                \
	}
/* clang-format on */

ITPLD_DEFINE_ELF_READ_GENERIC_TEST(ubyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST(utwobyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST(fourbyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST(ufourbyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST(eightbyte)
ITPLD_DEFINE_ELF_READ_GENERIC_TEST(ueightbyte)

#undef ITPLD_DEFINE_ELF_READ_GENERIC_TEST

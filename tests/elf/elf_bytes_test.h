#ifndef ITPLD_TESTS_ELF_ELF_BYTES_TEST_H_
#define ITPLD_TESTS_ELF_ELF_BYTES_TEST_H_

/* clang-format off */
#define ITPLD_DECLARE_ELF_READ_GENERIC_TEST(type)                                                   \
    void                                                                                            \
    itpld_elf_read_##type##_test(void)
/* clang-format on */

ITPLD_DECLARE_ELF_READ_GENERIC_TEST(ubyte);
ITPLD_DECLARE_ELF_READ_GENERIC_TEST(utwobyte);
ITPLD_DECLARE_ELF_READ_GENERIC_TEST(fourbyte);
ITPLD_DECLARE_ELF_READ_GENERIC_TEST(ufourbyte);
ITPLD_DECLARE_ELF_READ_GENERIC_TEST(eightbyte);
ITPLD_DECLARE_ELF_READ_GENERIC_TEST(ueightbyte);

#endif /* ITPLD_TESTS_ELF_ELF_BYTES_TEST_H_ */

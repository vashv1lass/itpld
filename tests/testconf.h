#ifndef ITPLD_TESTS_TESTCONF_H_
#define ITPLD_TESTS_TESTCONF_H_

typedef void (*test_f)(void);

typedef struct itpld_test_registry_entry {
	char const * const name;
	test_f const	   func;
} itpld_test_registry_entry_t;

extern itpld_test_registry_entry_t const ITPLD_TEST_REGISTRY[];

#endif /* ITPLD_TESTS_TESTCONF_H_ */

#include "test_helpers/helpers.h"

#include "common/checked.h"

#include <stdio.h>
#include <stdlib.h>

char *
itpld_test_fixture_path(char const * category, char const * name)
{
	int    written = snprintf(NULL, 0, "%s/%s/%s", ITPLD_TESTS_FIXTURES_DIR, category, name);
	size_t pathsz;

	if (!itpld_int_size_cast(written, &pathsz)) return NULL;
	if (!itpld_size_add(pathsz, 1, &pathsz)) return NULL;

	char * path = malloc(pathsz * sizeof(*path));
	if (path == NULL) return NULL;
	if (snprintf(path, pathsz, "%s/%s/%s", ITPLD_TESTS_FIXTURES_DIR, category, name) != written) {
		free(path);
		return NULL;
	}

	return path;
}

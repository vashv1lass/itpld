#include "common/file_test.h"

#include "common/file.h"
#include "common/status.h"
#include "common/types.h"
#include "test_helpers/helpers.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void
itpld_filebuf_init_test_ok_(void)
{
	itpld_uchar_t	    rsp;
	itpld_file_buffer_t buf = { .data = &rsp, .size = 176 };

	itpld_filebuf_init(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

static void
itpld_filebuf_init_test_null_guards_(void)
{
	itpld_filebuf_init(NULL);
}

void
itpld_filebuf_init_test(void)
{
	itpld_filebuf_init_test_ok_();
	itpld_filebuf_init_test_null_guards_();
}

static itpld_file_buffer_t
itpld_allocate_filebuf_fixture_(void)
{
	itpld_file_buffer_t buf = { 0 };

	buf.data = malloc(16 * sizeof(*buf.data));
	assert(buf.data != NULL);

	buf.size = 16;

	return buf;
}

static void
itpld_filebuf_destroy_test_ok_(void)
{
	itpld_file_buffer_t buf = itpld_allocate_filebuf_fixture_();

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

static void
itpld_filebuf_destroy_test_null_guards_(void)
{
	itpld_filebuf_destroy(NULL);
}

static void
itpld_filebuf_destroy_test_twice_(void)
{
	itpld_file_buffer_t buf = itpld_allocate_filebuf_fixture_();

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

static void
itpld_filebuf_destroy_test_empty_(void)
{
	itpld_file_buffer_t buf = { 0 };

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

void
itpld_filebuf_destroy_test(void)
{
	itpld_filebuf_destroy_test_ok_();
	itpld_filebuf_destroy_test_null_guards_();
	itpld_filebuf_destroy_test_twice_();
	itpld_filebuf_destroy_test_empty_();
}

static void
itpld_assert_filebuf_equals_(itpld_file_buffer_t buf, itpld_uchar_t const * expected, size_t expectedsz)
{
	assert(buf.size == expectedsz);

	if (expectedsz == 0) {
		assert(buf.data == NULL && expected == NULL);
		return;
	}

	assert(buf.data != NULL && expected != NULL);
	assert(memcmp(buf.data, expected, expectedsz) == 0);
}

static itpld_uchar_t const FIXTURES_FILE0_BIN_DATA[] = { 0x00, 0x01, 0x02, 0x7F, 0x80, 0xFF };
static size_t const	   FIXTURES_FILE0_BIN_SIZE   = 6;

static itpld_uchar_t const FIXTURES_FILE1_BIN_DATA[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static size_t const	   FIXTURES_FILE1_BIN_SIZE   = 10;

static void
itpld_file_read_test_ok_(void)
{
	char * path = itpld_test_fixture_path("file", "file0.bin");
	assert(path != NULL);

	itpld_file_buffer_t buf = { 0 };
	assert(itpld_file_read(path, &buf) == ITPLD_STATUS_OK);

	itpld_assert_filebuf_equals_(buf, FIXTURES_FILE0_BIN_DATA, FIXTURES_FILE0_BIN_SIZE);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_empty_(void)
{
	char * path = itpld_test_fixture_path("file", "empty.bin");
	assert(path != NULL);

	itpld_file_buffer_t buf = { 0 };
	assert(itpld_file_read(path, &buf) == ITPLD_STATUS_OK);
	itpld_assert_filebuf_equals_(buf, NULL, 0);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_noexist_(void)
{
	char * path = itpld_test_fixture_path("file", "noexist.bin");
	assert(path != NULL);

	itpld_file_buffer_t buf = { 0 };
	assert(itpld_file_read(path, &buf) == ITPLD_STATUS_IO_ERROR);
	assert(buf.data == NULL && buf.size == 0);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_null_guards_(void)
{
	char * path = itpld_test_fixture_path("file", "file0.bin");
	assert(path != NULL);

	itpld_file_buffer_t buf = { 0 };
	assert(itpld_file_read(NULL, &buf) == ITPLD_STATUS_INVAL_ARG);
	assert(buf.data == NULL && buf.size == 0);

	assert(itpld_file_read(path, NULL) == ITPLD_STATUS_INVAL_ARG);
	assert(buf.data == NULL && buf.size == 0);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_replaces_(void)
{
	char *		    path0 = itpld_test_fixture_path("file", "file0.bin");
	char *		    path1 = itpld_test_fixture_path("file", "file1.bin");
	itpld_file_buffer_t buf	  = { 0 };

	assert(itpld_file_read(path0, &buf) == ITPLD_STATUS_OK);
	itpld_assert_filebuf_equals_(buf, FIXTURES_FILE0_BIN_DATA, FIXTURES_FILE0_BIN_SIZE);

	assert(itpld_file_read(path1, &buf) == ITPLD_STATUS_OK);
	itpld_assert_filebuf_equals_(buf, FIXTURES_FILE1_BIN_DATA, FIXTURES_FILE1_BIN_SIZE);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path0);
	path0 = NULL;

	free(path1);
	path1 = NULL;
}

static void
itpld_file_read_test_preserves_(void)
{
	char *		    path0 = itpld_test_fixture_path("file", "file0.bin");
	char *		    path1 = itpld_test_fixture_path("file", "noexists.bin");
	itpld_file_buffer_t buf	  = { 0 };

	assert(itpld_file_read(path0, &buf) == ITPLD_STATUS_OK);
	itpld_assert_filebuf_equals_(buf, FIXTURES_FILE0_BIN_DATA, FIXTURES_FILE0_BIN_SIZE);

	assert(itpld_file_read(path1, &buf) == ITPLD_STATUS_IO_ERROR);
	itpld_assert_filebuf_equals_(buf, FIXTURES_FILE0_BIN_DATA, FIXTURES_FILE0_BIN_SIZE);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path0);
	path0 = NULL;

	free(path1);
	path1 = NULL;
}

void
itpld_file_read_test(void)
{
	itpld_file_read_test_ok_();
	itpld_file_read_test_empty_();
	itpld_file_read_test_noexist_();
	itpld_file_read_test_null_guards_();
	itpld_file_read_test_replaces_();
	itpld_file_read_test_preserves_();
}

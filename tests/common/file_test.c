#include "file_test.h"

#include "common/file.h"
#include "common/status.h"
#include "common/types.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void
itpld_filebuf_init_test_ok(void)
{
	itpld_uchar	    rsp;
	itpld_file_buffer_t buf = { .data = &rsp, .size = 176 };

	itpld_filebuf_init(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

static void
itpld_filebuf_init_test_null_guards(void)
{
	itpld_filebuf_init(NULL);
}

void
itpld_filebuf_init_test(void)
{
	itpld_filebuf_init_test_ok();
	itpld_filebuf_init_test_null_guards();
}

static void
itpld_filebuf_destroy_test_ok(void)
{
	itpld_file_buffer_t buf = { 0 };

	buf.data = malloc(16 * sizeof(*buf.data));
	assert(buf.data != NULL);
	buf.size = 16;

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

static void
itpld_filebuf_destroy_test_null_guards(void)
{
	itpld_filebuf_destroy(NULL);
}

static void
itpld_filebuf_destroy_test_twice(void)
{
	itpld_file_buffer_t buf = { 0 };

	buf.data = malloc(16 * sizeof(*buf.data));
	assert(buf.data != NULL);
	buf.size = 16;

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

static void
itpld_filebuf_destroy_test_empty(void)
{
	itpld_file_buffer_t buf = { 0 };

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);
}

void
itpld_filebuf_destroy_test(void)
{
	itpld_filebuf_destroy_test_ok();
	itpld_filebuf_destroy_test_null_guards();
	itpld_filebuf_destroy_test_twice();
	itpld_filebuf_destroy_test_empty();
}

static char *
itpld_fixture_filepath(char const * file)
{
	int written = snprintf(NULL, 0, "%s/%s", ITPLD_TESTS_FIXTURES_DIR "/file", file);
	assert(written >= 0);
#if INT_MAX >= SIZE_MAX
	assert(written < SIZE_MAX);
#endif

	size_t pathsz = (size_t)written + 1;
	char * path   = malloc(pathsz * sizeof(char));
	assert(path != NULL);
	assert(snprintf(path, pathsz, "%s/%s", ITPLD_TESTS_FIXTURES_DIR "/file", file) == written);
	path[pathsz - 1] = '\0';

	return path;
}

static void
itpld_file_read_test_ok(void)
{
	char *		    path = itpld_fixture_filepath("file0.bin");
	itpld_file_buffer_t buf	 = { 0 };

	assert(itpld_file_read(path, &buf) == ITPLD_STATUS_OK);
	assert(buf.data != NULL && buf.size == 6);

	itpld_uchar sample_strbuf[] = { 0x00, 0x01, 0x02, 0x7F, 0x80, 0xFF };
	for (size_t i = 0; i < buf.size; i++) assert(sample_strbuf[i] == buf.data[i]);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_empty(void)
{
	char *		    path = itpld_fixture_filepath("empty.bin");
	itpld_file_buffer_t buf	 = { 0 };

	assert(itpld_file_read(path, &buf) == ITPLD_STATUS_OK);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_noexist(void)
{
	char *		    path = itpld_fixture_filepath("noexist.bin");
	itpld_file_buffer_t buf	 = { 0 };

	assert(itpld_file_read(path, &buf) == ITPLD_STATUS_IO_ERROR);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_null_guards(void)
{
	char *		    path = itpld_fixture_filepath("file0.bin");
	itpld_file_buffer_t buf	 = { 0 };

	assert(itpld_file_read(NULL, &buf) == ITPLD_STATUS_INVAL_ARG);
	assert(buf.data == NULL && buf.size == 0);

	assert(itpld_file_read(path, NULL) == ITPLD_STATUS_INVAL_ARG);
	assert(buf.data == NULL && buf.size == 0);

	free(path);
	path = NULL;
}

static void
itpld_file_read_test_replaces(void)
{
	char *		    path0 = itpld_fixture_filepath("file0.bin");
	char *		    path1 = itpld_fixture_filepath("file1.bin");
	itpld_file_buffer_t buf	  = { 0 };

	assert(itpld_file_read(path0, &buf) == ITPLD_STATUS_OK);
	assert(buf.data != NULL && buf.size == 6);

	itpld_uchar sample_strbuf0[] = { 0x00, 0x01, 0x02, 0x7F, 0x80, 0xFF };
	for (size_t i = 0; i < buf.size; i++) assert(sample_strbuf0[i] == buf.data[i]);

	assert(itpld_file_read(path1, &buf) == ITPLD_STATUS_OK);
	assert(buf.data != NULL && buf.size == 10);

	itpld_uchar sample_strbuf1[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	for (size_t i = 0; i < buf.size; i++) assert(sample_strbuf1[i] == buf.data[i]);

	itpld_filebuf_destroy(&buf);
	assert(buf.data == NULL && buf.size == 0);

	free(path0);
	path0 = NULL;

	free(path1);
	path1 = NULL;
}

static void
itpld_file_read_test_preserves(void)
{
	char *		    path0 = itpld_fixture_filepath("file0.bin");
	char *		    path1 = itpld_fixture_filepath("noexist.bin");
	itpld_file_buffer_t buf	  = { 0 };

	assert(itpld_file_read(path0, &buf) == ITPLD_STATUS_OK);
	assert(buf.data != NULL && buf.size == 6);

	itpld_uchar sample_strbuf0[] = { 0x00, 0x01, 0x02, 0x7F, 0x80, 0xFF };
	for (size_t i = 0; i < buf.size; i++) assert(sample_strbuf0[i] == buf.data[i]);

	assert(itpld_file_read(path1, &buf) == ITPLD_STATUS_IO_ERROR);
	assert(buf.data != NULL && buf.size == 6);

	for (size_t i = 0; i < buf.size; i++) assert(sample_strbuf0[i] == buf.data[i]);

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
	itpld_file_read_test_ok();
	itpld_file_read_test_empty();
	itpld_file_read_test_noexist();
	itpld_file_read_test_null_guards();
	itpld_file_read_test_replaces();
	itpld_file_read_test_preserves();
}

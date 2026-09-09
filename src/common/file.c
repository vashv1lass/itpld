#include "common/file.h"

#include "common/checked.h"
#include "common/compiler.h"
#include "common/status.h"
#include "common/types.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void
itpld_filebuf_init(itpld_file_buffer_t * buf)
{
	if (buf == NULL) return;

	buf->data = NULL;
	buf->size = 0;
}

void
itpld_filebuf_destroy(itpld_file_buffer_t * buf)
{
	if (buf == NULL) return;

	free(buf->data);
	buf->data = NULL;

	buf->size = 0;
}

itpld_status_t
itpld_file_read(char const * restrict path, itpld_file_buffer_t * restrict outbuf)
{
	if (outbuf == NULL || path == NULL) return ITPLD_STATUS_INVAL_ARG;

	FILE * fd = fopen(path, "rb");
	if (fd == NULL) return ITPLD_STATUS_IO_ERROR;

	itpld_file_buffer_t buf;
	itpld_filebuf_init(&buf);

	itpld_status_t stat = ITPLD_STATUS_OK;

	if (fseek(fd, 0L, SEEK_END) != 0) {
		stat = ITPLD_STATUS_IO_ERROR;
		goto mrproper;
	}
	long offset = ftell(fd);
	if (offset < 0) {
		stat = ITPLD_STATUS_IO_ERROR;
		goto mrproper;
	}
	if (fseek(fd, 0L, SEEK_SET) != 0) {
		stat = ITPLD_STATUS_IO_ERROR;
		goto mrproper;
	}

	if (offset != 0) {
		if (!itpld_long_size_cast(offset, &buf.size)) {
			stat = ITPLD_STATUS_OVERFLOW;
			goto mrproper;
		}
		size_t bufsz_bytes;
		if (!itpld_size_mul(buf.size, sizeof(*buf.data), &bufsz_bytes)) {
			stat = ITPLD_STATUS_OVERFLOW;
			goto mrproper;
		}

		buf.data = malloc(bufsz_bytes);
		if (buf.data == NULL) {
			stat = ITPLD_STATUS_OUT_OF_MEM;
			goto mrproper;
		}

		if (fread(buf.data, sizeof(itpld_uchar), buf.size, fd) != buf.size) {
			stat = ITPLD_STATUS_IO_ERROR;
			goto mrproper;
		}
	}

	if (fclose(fd) != 0) {
		fd   = NULL;
		stat = ITPLD_STATUS_IO_ERROR;
		goto mrproper;
	}
	fd = NULL;

	itpld_filebuf_destroy(outbuf);

	*outbuf = buf;
	return stat;
mrproper:
	itpld_filebuf_destroy(&buf);
	if (fd != NULL) ITPLD_RETVAL_IGNORED(fclose(fd));
	return stat;
}

#include "common/file.h"

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

	size_t	      bufsz = 0;
	itpld_uchar * buf   = NULL;
	if (offset != 0) {
#if LONG_MAX > SIZE_MAX
		if (offset > SIZE_MAX) {
			stat = ITPLD_STATUS_OVERFLOW;
			goto mrproper;
		}
#endif
		bufsz = (size_t)offset;
		buf   = malloc(bufsz * sizeof(*buf));
		if (buf == NULL) {
			stat = ITPLD_STATUS_OUT_OF_MEM;
			goto mrproper;
		}

		if (fread(buf, sizeof(itpld_uchar), bufsz, fd) != bufsz) {
			free(buf);
			stat = ITPLD_STATUS_IO_ERROR;
			goto mrproper;
		}
	}

	if (fclose(fd) != 0) {
		free(buf);
		return ITPLD_STATUS_IO_ERROR;
	}

	itpld_filebuf_destroy(outbuf);
	outbuf->data = buf;
	outbuf->size = bufsz;

	return stat;
mrproper:
	ITPLD_RETVAL_IGNORED(fclose(fd));
	return stat;
}

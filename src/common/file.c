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
	if (buf != NULL) {
		buf->data = NULL;
		buf->size = 0;
	}
}

void
itpld_filebuf_destroy(itpld_file_buffer_t * buf)
{
	if (buf != NULL) {
		free(buf->data);
		buf->data = NULL;

		buf->size = 0;
	}
}

itpld_status_t
itpld_file_read(char const * restrict path, itpld_file_buffer_t * restrict outbuf)
{
	itpld_filebuf_destroy(outbuf);
	itpld_filebuf_init(outbuf);

	if (outbuf == NULL || path == NULL) {
		return ITPLD_STATUS_INVAL_ARG;
	}

	FILE * fd = fopen(path, "rb");
	if (fd == NULL) {
		return ITPLD_STATUS_IO_ERROR;
	}

	if (fseek(fd, 0L, SEEK_END) != 0) {
		ITPLD_RETVAL_UNUSED(fclose(fd));
		return ITPLD_STATUS_IO_ERROR;
	}
	long offset = ftell(fd);
	if (offset < 0) {
		ITPLD_RETVAL_UNUSED(fclose(fd));
		return ITPLD_STATUS_IO_ERROR;
	}
	if (fseek(fd, 0L, SEEK_SET) != 0) {
		ITPLD_RETVAL_UNUSED(fclose(fd));
		return ITPLD_STATUS_IO_ERROR;
	}

	if (offset != 0) {
#if LONG_MAX > SIZE_MAX
		if (offset > SIZE_MAX) {
			ITPLD_RETVAL_UNUSED(fclose(fd));
			return ITPLD_STATUS_OVERFLOW;
		}
#endif
		size_t	      bufsz = (size_t)offset;
		itpld_uchar * buf   = (itpld_uchar *)calloc(bufsz, sizeof(itpld_uchar));
		if (buf == NULL) {
			ITPLD_RETVAL_UNUSED(fclose(fd));
			return ITPLD_STATUS_OUT_OF_MEM;
		}

		if (fread((void *)buf, sizeof(itpld_uchar), bufsz, fd) != bufsz) {
			free(buf);
			ITPLD_RETVAL_UNUSED(fclose(fd));
			return ITPLD_STATUS_IO_ERROR;
		}

		outbuf->data = buf;
		outbuf->size = bufsz;
	}

	if (fclose(fd) != 0) {
		itpld_filebuf_destroy(outbuf);
		return ITPLD_STATUS_IO_ERROR;
	}

	return ITPLD_STATUS_OK;
}

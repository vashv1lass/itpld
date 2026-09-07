#ifndef ITPLD_COMMON_FILE_H_
#define ITPLD_COMMON_FILE_H_

#include "common/status.h"
#include "common/types.h"

#include <stddef.h>

typedef struct itpld_file_buffer {
	itpld_uchar * data;
	size_t	      size;
} itpld_file_buffer_t;

/* initializes the buffer, must not be called on a live buffer */
void
itpld_filebuf_init(itpld_file_buffer_t * buf);

/*
 * deallocates (via free) old buffer data and reinitializes the buffer.
 * either buf->data == NULL, or buf->data points to heap allocation, else - UB
 */
void
itpld_filebuf_destroy(itpld_file_buffer_t * buf);

/*
 * safely replaces outbuf contents to those read from file on successful read and preserves its contents on failure.
 * buffer must-be initialized (itpld_filebuf_init() or zero-initialized) before the first call
 */
itpld_status_t
itpld_file_read(char const * restrict path, itpld_file_buffer_t * restrict outbuf);

#endif /* ITPLD_COMMON_FILE_H_ */

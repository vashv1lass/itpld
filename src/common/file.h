#ifndef ITPLD_FILE_H_
#define ITPLD_FILE_H_

#include "common/status.h"
#include "common/types.h"

#include <stddef.h>

typedef struct itpld_file_buffer {
	itpld_uchar * data;
	size_t	      size;
} itpld_file_buffer_t;

void
itpld_filebuf_init(itpld_file_buffer_t * buf);

void
itpld_filebuf_destroy(itpld_file_buffer_t * buf);

/* outbuf must be initialized with itpld_filebuf_init() or zero-initialized before the first call.
 * itpld_file_read replaces its contents.
 */
itpld_status_t
itpld_file_read(char const * restrict path, itpld_file_buffer_t * restrict outbuf);

#endif /* ITPLD_FILE_H_ */

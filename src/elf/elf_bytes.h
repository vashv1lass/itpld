#ifndef ITPLD_ELF_ELF_BYTES_H_
#define ITPLD_ELF_ELF_BYTES_H_

#include "common/status.h"
#include "elf/elf_types.h"

#include <stddef.h>

typedef struct itpld_elf_bytes {
	itpld_elf_ubyte_t const * data;
	size_t			  bytescnt;
} itpld_elf_bytes_t;

/* clang-format off */
#define ITPLD_DECLARE_ELF_READ_GENERIC(type)                                                                           \
	itpld_status_t                                                                                                 \
	itpld_elf_read_##type(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_##type##_t * out)
/* clang-format on */

/* Host byte endianness only! */
ITPLD_DECLARE_ELF_READ_GENERIC(ubyte);
ITPLD_DECLARE_ELF_READ_GENERIC(utwobyte);
ITPLD_DECLARE_ELF_READ_GENERIC(fourbyte);
ITPLD_DECLARE_ELF_READ_GENERIC(ufourbyte);
ITPLD_DECLARE_ELF_READ_GENERIC(eightbyte);
ITPLD_DECLARE_ELF_READ_GENERIC(ueightbyte);

#undef ITPLD_DECLARE_ELF_READ_GENERIC

#endif /* ITPLD_ELF_ELF_BYTES_H_ */

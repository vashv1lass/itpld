#ifndef ITPLD_ELF_ELF_BYTES_H_
#define ITPLD_ELF_ELF_BYTES_H_

#include "common/status.h"
#include "elf/elf_types.h"

#include <stddef.h>

typedef struct itpld_elf_bytes {
	itpld_elf_ubyte_t const * data;
	size_t			  bytescnt;
} itpld_elf_bytes_t;

itpld_status_t
itpld_elf_read_ubyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_ubyte_t * out);

/*
 * Host byte endianness only!
 */

itpld_status_t
itpld_elf_read_utwobyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_utwobyte_t * out);

itpld_status_t
itpld_elf_read_fourbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_fourbyte_t * out);

itpld_status_t
itpld_elf_read_ufourbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_ufourbyte_t * out);

itpld_status_t
itpld_elf_read_eightbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_eightbyte_t * out);

itpld_status_t
itpld_elf_read_ueightbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_ueightbyte_t * out);

#endif /* ITPLD_ELF_ELF_BYTES_H_ */

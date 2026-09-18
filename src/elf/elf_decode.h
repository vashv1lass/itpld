#ifndef ITPLD_ELF_ELF_DECODE_H_
#define ITPLD_ELF_ELF_DECODE_H_

#include "common/status.h"
#include "elf/elf_bytes.h"
#include "elf/elf_types.h"

itpld_status_t
itpld_elf_decode_ehdr(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out);

#endif /* ITPLD_ELF_ELF_DECODE_H_ */

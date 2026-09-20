#ifndef ITPLD_ELF_ELF_DECODE_H_
#define ITPLD_ELF_ELF_DECODE_H_

#include "common/status.h"
#include "elf/elf_bytes.h"
#include "elf/elf_types.h"

itpld_status_t
itpld_elf_decode_ehdr(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out);

itpld_status_t
itpld_elf_decode_shdr(itpld_elf_bytes_t bytes, size_t e_shoff, itpld_elf64_shdr_t * out);

itpld_status_t
itpld_elf_decode_sym(itpld_elf_bytes_t bytes, size_t sym_offset, itpld_elf64_sym_t * out);

itpld_status_t
itpld_elf_decode_rela(itpld_elf_bytes_t bytes, size_t rela_offset, itpld_elf64_rela_t * out);

itpld_status_t
itpld_elf_decode_phdr(itpld_elf_bytes_t bytes, size_t e_phoff, itpld_elf64_phdr_t * out);

#endif /* ITPLD_ELF_ELF_DECODE_H_ */

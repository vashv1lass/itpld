#ifndef ITPLD_ELF_ELF_TYPES_H_
#define ITPLD_ELF_ELF_TYPES_H_

#include "elf/elf_constants.h"

#include <stdint.h>

/* source: system v abi amd64 arch processor supplement (2025), figure 3.1 */
/* NOTE: type names are not same as specified! */
typedef uint8_t	 itpld_elf_ubyte_t;
typedef uint16_t itpld_elf_utwobyte_t;
typedef int32_t	 itpld_elf_fourbyte_t;
typedef uint32_t itpld_elf_ufourbyte_t;
typedef int64_t	 itpld_elf_eightbyte_t;
typedef uint64_t itpld_elf_ueightbyte_t;

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 1 */
/* NOTE: type names are not same as specified! */
typedef itpld_elf_ueightbyte_t itpld_elf64_addr_t;
typedef itpld_elf_ueightbyte_t itpld_elf64_off_t;
typedef itpld_elf_utwobyte_t   itpld_elf64_half_t;
typedef itpld_elf_ufourbyte_t  itpld_elf64_word_t;
typedef itpld_elf_fourbyte_t   itpld_elf64_sword_t;
typedef itpld_elf_ueightbyte_t itpld_elf64_xword_t;
typedef itpld_elf_eightbyte_t  itpld_elf64_sxword_t;

/* source: generic system v abi specification version 1.5 draft 2 (1998), figure 2 */
typedef struct itpld_elf64_ehdr {
	itpld_elf_ubyte_t  e_ident[ITPLD_EI_NIDENT]; /* ELF identification */
	itpld_elf64_half_t e_type;		     /* Object file type */
	itpld_elf64_half_t e_machine;		     /* Machine type */
	itpld_elf64_word_t e_version;		     /* Object file version */
	itpld_elf64_addr_t e_entry;		     /* Entry point address */
	itpld_elf64_off_t  e_phoff;		     /* Program header offset */
	itpld_elf64_off_t  e_shoff;		     /* Section header offset */
	itpld_elf64_word_t e_flags;		     /* Processor-specific flags */
	itpld_elf64_half_t e_ehsize;		     /* ELF header size */
	itpld_elf64_half_t e_phentsize;		     /* Size of program header entry */
	itpld_elf64_half_t e_phnum;		     /* Number of program header entries */
	itpld_elf64_half_t e_shentsize;		     /* Size of section header entry */
	itpld_elf64_half_t e_shnum;		     /* Number of section header entries */
	itpld_elf64_half_t e_shstrndx;		     /* Section name string table index */
} itpld_elf64_ehdr_t;

#endif /* ITPLD_ELF_ELF_TYPES_H_ */

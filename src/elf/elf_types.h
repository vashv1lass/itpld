#ifndef ITPLD_ELF_ELF_TYPES_H_
#define ITPLD_ELF_ELF_TYPES_H_

#include "common/types.h"
#include "elf/elf_constants.h"

/* source: system v abi amd64 arch processor supplement (2025), figure 3.1 */
/* NOTE: type names are not same as specified! */
typedef itpld_u8_t  itpld_elf_ubyte_t;
typedef itpld_u16_t itpld_elf_utwobyte_t;
typedef itpld_i32_t itpld_elf_fourbyte_t;
typedef itpld_u32_t itpld_elf_ufourbyte_t;
typedef itpld_i64_t itpld_elf_eightbyte_t;
typedef itpld_u64_t itpld_elf_ueightbyte_t;

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
/* These structures represent decoded ELF records. Mustn't be overlaid on raw file bytes */
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

#define ITPLD_ELF64_EHDR_SIZE sizeof(itpld_elf64_ehdr_t)

/* source: generic system v abi specification version 1.5 draft 2 (1998), figure 3 */
typedef struct itpld_elf64_shdr {
	itpld_elf64_word_t  sh_name;	  /* Section name */
	itpld_elf64_word_t  sh_type;	  /* Section type */
	itpld_elf64_xword_t sh_flags;	  /* Section attributes */
	itpld_elf64_addr_t  sh_addr;	  /* Virtual address in memory */
	itpld_elf64_off_t   sh_offset;	  /* Offset in file */
	itpld_elf64_xword_t sh_size;	  /* Size of section */
	itpld_elf64_word_t  sh_link;	  /* Link to other section */
	itpld_elf64_word_t  sh_info;	  /* Miscellaneous information */
	itpld_elf64_xword_t sh_addralign; /* Address alignment boundary */
	itpld_elf64_xword_t sh_entsize;	  /* Size of entries, if section has table */
} itpld_elf64_shdr_t;

#define ITPLD_ELF64_SHDR_SIZE sizeof(itpld_elf64_shdr_t)

/* source: generic system v abi specification version 1.5 draft 2 (1998), figure 4 */
typedef struct itpld_elf64_sym {
	itpld_elf64_word_t  st_name;  /* Symbol name */
	itpld_elf_ubyte_t   st_info;  /* Type and Binding attributes */
	itpld_elf_ubyte_t   st_other; /* Reserved */
	itpld_elf64_half_t  st_shndx; /* Section table index */
	itpld_elf64_addr_t  st_value; /* Symbol value */
	itpld_elf64_xword_t st_size;  /* Size of object (e.g., common) */
} itpld_elf64_sym_t;

#define ITPLD_ELF64_SYM_SIZE sizeof(itpld_elf64_sym_t)

/* source: generic system v abi specification draft (2013), Symbol Table */
#define ITPLD_ELF64_ST_VISIBILITY(o) ((o) & 0x3)

/* source: generic system v abi specification draft (2013), Symbol Table */
#define ITPLD_ELF64_ST_BIND(i)	  ((i) >> 4)
#define ITPLD_ELF64_ST_TYPE(i)	  ((i) & 0x0F)
#define ITPLD_ELF64_ST_INFO(b, t) (((b) << 4) + ((t) & 0x0F))

/* source: generic system v abi specification version 1.5 draft 2 (1998), figure 5 */
typedef struct itpld_elf64_rela {
	itpld_elf64_addr_t   r_offset;
	itpld_elf64_xword_t  r_info;
	itpld_elf64_sxword_t r_addend;
} itpld_elf64_rela_t;

#define ITPLD_ELF64_RELA_SIZE sizeof(itpld_elf64_rela_t)

/* source: generic system v abi specification version 1.5 draft 2 (1998), section 7 */
#define ITPLD_ELF64_R_SYM(i)	 ((i) >> 32)
#define ITPLD_ELF64_R_TYPE(i)	 ((i) & 0xFFFFFFFFL)
#define ITPLD_ELF64_R_INFO(s, t) (((s) << 32) + ((t) & 0xFFFFFFFFL))

/* source: generic system v abi specification version 1.5 draft 2 (1998), figure 6 */
typedef struct itpld_elf64_phdr {
	itpld_elf64_word_t  p_type;   /* Type of segment */
	itpld_elf64_word_t  p_flags;  /* Segment attributes */
	itpld_elf64_off_t   p_offset; /* Offset in file */
	itpld_elf64_addr_t  p_vaddr;  /* Virtual address in memory */
	itpld_elf64_addr_t  p_paddr;  /* Reserved */
	itpld_elf64_xword_t p_filesz; /* Size of segment in file */
	itpld_elf64_xword_t p_memsz;  /* Size of segment in memory */
	itpld_elf64_xword_t p_align;  /* Alignment of segment */
} itpld_elf64_phdr_t;

#define ITPLD_ELF64_PHDR_SIZE sizeof(itpld_elf64_phdr_t)

#endif /* ITPLD_ELF_ELF_TYPES_H_ */

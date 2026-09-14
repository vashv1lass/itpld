#ifndef ITPLD_ELF_ELF_CONSTANTS_H_
#define ITPLD_ELF_ELF_CONSTANTS_H_

/*
 * MVP: LP64, small code model only, static, non-pie, non-pic, no GOT, no PLT, no TLS, no dy linking
 */

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 2 */
#define ITPLD_EI_MAG0	    0
#define ITPLD_EI_MAG1	    1
#define ITPLD_EI_MAG2	    2
#define ITPLD_EI_MAG3	    3
#define ITPLD_EI_CLASS	    4
#define ITPLD_EI_DATA	    5
#define ITPLD_EI_VERSION    6
#define ITPLD_EI_OSABI	    7
#define ITPLD_EI_ABIVERSION 8
#define ITPLD_EI_PAD	    9
#define ITPLD_EI_NIDENT	    16

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 3 */
#define ITPLD_ELFCLASS64 2

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 4 */
#define ITPLD_ELFDATA2LSB 1

/* source: generic system v abi specification version 1.5 draft 2 (1998), section 3 */
#define ITPLD_EV_CURRENT 1

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 5 */
#define ITPLD_ELFOSABI_SYSV 0

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 6 */
#define ITPLD_ET_REL  1
#define ITPLD_ET_EXEC 2

/* source: system v abi amd64 arch processor supplement (2025), figure 3.1 */
#define ITPLD_EM_X86_64 62

/* source: man 5 elf */
#define ITPLD_EF_X86_64_NONE 0

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 7 */
#define ITPLD_SHN_UNDEF	 0
#define ITPLD_SHN_ABS	 0xFFF1
#define ITPLD_SHN_COMMON 0xFFF2

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 8 */
#define ITPLD_SHT_NULL	   0
#define ITPLD_SHT_PROGBITS 1
#define ITPLD_SHT_SYMTAB   2
#define ITPLD_SHT_STRTAB   3
#define ITPLD_SHT_RELA	   4
#define ITPLD_SHT_NOBITS   8

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 9 */
#define ITPLD_SHF_WRITE	    0x1
#define ITPLD_SHF_ALLOC	    0x2
#define ITPLD_SHF_EXECINSTR 0x4

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 14 */
#define ITPLD_STB_LOCAL	 0
#define ITPLD_STB_GLOBAL 1
#define ITPLD_STB_WEAK	 2

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 15 */
#define ITPLD_STT_NOTYPE  0
#define ITPLD_STT_OBJECT  1
#define ITPLD_STT_FUNC	  2
#define ITPLD_STT_SECTION 3
#define ITPLD_STT_FILE	  4

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 16 */
#define ITPLD_PT_NULL 0
#define ITPLD_PT_LOAD 1

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 17 */
#define ITPLD_PF_X 0x1
#define ITPLD_PF_W 0x2
#define ITPLD_PF_R 0x4

/* source: system v abi amd64 arch processor supplement (2025), table 4.9 */
#define ITPLD_R_X86_64_NONE  0
#define ITPLD_R_X86_64_64    1
#define ITPLD_R_X86_64_PC32  2
#define ITPLD_R_X86_64_PLT32 4 /* resolve as ITPLD_R_X86_64_PC32 in the MVP */
#define ITPLD_R_X86_64_32    10
#define ITPLD_R_X86_64_32S   11

#define ITPLD_EI_MAG0_VAL	'\x7F'
#define ITPLD_EI_MAG1_VAL	'E'
#define ITPLD_EI_MAG2_VAL	'L'
#define ITPLD_EI_MAG3_VAL	'F'
#define ITPLD_EI_CLASS_VAL	ITPLD_ELFCLASS64  /* source: amd64 supplement (section 4.1.1) */
#define ITPLD_EI_DATA_VAL	ITPLD_ELFDATA2LSB /* source: amd64 supplement (section 4.1.1) */
#define ITPLD_EI_VERSION_VAL	ITPLD_EV_CURRENT
#define ITPLD_EI_OSABI_VAL	ITPLD_ELFOSABI_SYSV
#define ITPLD_EI_ABIVERSION_VAL 0

#endif /* ITPLD_ELF_ELF_CONSTANTS_H_ */

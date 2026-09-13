#ifndef ITPLD_ELF_ELF_CONSTANTS_H_
#define ITPLD_ELF_ELF_CONSTANTS_H_

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

/* source: generic system v abi specification version 1.5 draft 2 (1998), table 5 */
#define ITPLD_ET_REL  1
#define ITPLD_ET_EXEC 2

/* source: system v abi amd64 arch processor supplement (2025), figure 3.1 */
#define ITPLD_EM_X86_64 62

/* source: man 5 elf */
#define ITPLD_EF_X86_64_NONE 0

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

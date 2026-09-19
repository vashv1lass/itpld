#include "elf/elf_decode.h"

#include "common/checked.h"
#include "common/status.h"
#include "common/utils.h"
#include "elf/elf_bytes.h"
#include "elf/elf_constants.h"
#include "elf/elf_types.h"

#include <stddef.h>

/* allowed may be NULL only when allowedsz is zero */
#define ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(type)                                                                   \
	static itpld_status_t itpld_elf_read_##type##_if_valid_(                                                       \
	    itpld_elf_bytes_t		 bytes,                                                                        \
	    size_t			 base,                                                                         \
	    size_t			 off,                                                                          \
	    itpld_elf_##type##_t *	 out,                                                                          \
	    itpld_elf_##type##_t const * allowed,                                                                      \
	    size_t			 allowedsz                                                                     \
	)                                                                                                              \
	{                                                                                                              \
		size_t pos;                                                                                            \
		if (!itpld_size_add(base, off, &pos)) return ITPLD_STATUS_OVERFLOW;                                    \
                                                                                                                       \
		itpld_elf_##type##_t val;                                                                              \
		ITPLD_TRY(itpld_elf_read_##type(bytes, pos, &val));                                                    \
                                                                                                                       \
		for (size_t i = 0; i < allowedsz; i++) {                                                               \
			if (allowed[i] == val) {                                                                       \
				*out = val;                                                                            \
				return ITPLD_STATUS_OK;                                                                \
			}                                                                                              \
		}                                                                                                      \
		return ITPLD_STATUS_INVAL_FMT;                                                                         \
	}

ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(ubyte)
ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(utwobyte)
/* ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(fourbyte) */
ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(ufourbyte)
/* ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(eightbyte) */
ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(ueightbyte)

#undef ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID

/* NOLINTBEGIN(readability-function-cognitive-complexity) */
itpld_status_t
itpld_elf_decode_ehdr(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (bytes.data == NULL || !itpld_in_range(0, ITPLD_ELF64_EHDR_SIZE, bytes.bytescnt))
		return ITPLD_STATUS_INVAL_FMT;

	itpld_elf64_ehdr_t decoded = { 0 };

	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 0, &decoded.e_ident[ITPLD_EI_MAG0], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG0_VAL }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 1, &decoded.e_ident[ITPLD_EI_MAG1], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG1_VAL }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 2, &decoded.e_ident[ITPLD_EI_MAG2], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG2_VAL }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 3, &decoded.e_ident[ITPLD_EI_MAG3], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG3_VAL }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 4, &decoded.e_ident[ITPLD_EI_CLASS], (itpld_elf_ubyte_t const[]){ ITPLD_ELFCLASS64 }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 5, &decoded.e_ident[ITPLD_EI_DATA], (itpld_elf_ubyte_t const[]){ ITPLD_ELFDATA2LSB }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 6, &decoded.e_ident[ITPLD_EI_VERSION], (itpld_elf_ubyte_t const[]){ ITPLD_EV_CURRENT }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 7, &decoded.e_ident[ITPLD_EI_OSABI], (itpld_elf_ubyte_t const[]){ ITPLD_ELFOSABI_NONE }, 1
	));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(
	    bytes, 0, 8, &decoded.e_ident[ITPLD_EI_ABIVERSION], (itpld_elf_ubyte_t const[]){ 0 }, 1
	));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(
	    bytes, 0, 16, &decoded.e_type, (itpld_elf_utwobyte_t const[]){ ITPLD_ET_REL }, 1
	));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(
	    bytes, 0, 18, &decoded.e_machine, (itpld_elf_utwobyte_t const[]){ ITPLD_EM_X86_64 }, 1
	));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(
	    bytes, 0, 20, &decoded.e_version, (itpld_elf_ufourbyte_t const[]){ ITPLD_EV_CURRENT }, 1
	));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, 0, 24, &decoded.e_entry, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, 0, 32, &decoded.e_phoff, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, 0, 40, &decoded.e_shoff, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(
	    bytes, 0, 48, &decoded.e_flags, (itpld_elf_ufourbyte_t const[]){ ITPLD_EF_X86_64_NONE }, 1
	));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(
	    bytes, 0, 52, &decoded.e_ehsize, (itpld_elf_utwobyte_t const[]){ ITPLD_ELF64_EHDR_SIZE }, 1
	));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(
	    bytes, 0, 54, &decoded.e_phentsize, (itpld_elf_utwobyte_t const[]){ ITPLD_ELF64_PHDR_SIZE }, 1
	));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 56, &decoded.e_phnum, NULL, 0));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(
	    bytes, 0, 58, &decoded.e_shentsize, (itpld_elf_utwobyte_t const[]){ ITPLD_ELF64_SHDR_SIZE }, 1
	));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 60, &decoded.e_shnum, NULL, 0));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 62, &decoded.e_shstrndx, NULL, 0));

	*out = decoded;
	return ITPLD_STATUS_OK;
}

itpld_status_t
itpld_elf_decode_shdr(itpld_elf_bytes_t bytes, size_t shoff, itpld_elf64_shdr_t * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (bytes.data == NULL || !itpld_in_range(shoff, ITPLD_ELF64_SHDR_SIZE, bytes.bytescnt))
		return ITPLD_STATUS_INVAL_FMT;

	itpld_elf64_shdr_t decoded = { 0 };

	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, shoff, 0, &decoded.sh_name, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(
	    bytes,
	    shoff,
	    4,
	    &decoded.sh_type,
	    (itpld_elf_ufourbyte_t const[]){ ITPLD_SHT_NULL,
					     ITPLD_SHT_PROGBITS,
					     ITPLD_SHT_SYMTAB,
					     ITPLD_SHT_STRTAB,
					     ITPLD_SHT_RELA,
					     ITPLD_SHT_NOBITS },
	    6
	));

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, shoff, 8, &decoded.sh_flags, NULL, 0));
	itpld_elf_ueightbyte_t supported_flags = ITPLD_SHF_WRITE | ITPLD_SHF_ALLOC | ITPLD_SHF_EXECINSTR;
	if ((decoded.sh_flags & ~supported_flags) != 0) return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, shoff, 16, &decoded.sh_addr, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, shoff, 24, &decoded.sh_offset, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, shoff, 32, &decoded.sh_size, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, shoff, 40, &decoded.sh_link, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, shoff, 44, &decoded.sh_info, NULL, 0));

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, shoff, 48, &decoded.sh_addralign, NULL, 0));
	if ((decoded.sh_addralign & (decoded.sh_addralign - 1)) != 0 && decoded.sh_addralign != 0)
		return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, shoff, 56, &decoded.sh_entsize, NULL, 0));

	*out = decoded;
	return ITPLD_STATUS_OK;
}
/* NOLINTEND(readability-function-cognitive-complexity) */

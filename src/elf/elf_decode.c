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
ITPLD_DEFINE_ELF_READ_GENERIC_IF_VALID(eightbyte)
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

	/* clang-format off */
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 0, &decoded.e_ident[ITPLD_EI_MAG0], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG0_VAL }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 1, &decoded.e_ident[ITPLD_EI_MAG1], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG1_VAL }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 2, &decoded.e_ident[ITPLD_EI_MAG2], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG2_VAL }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 3, &decoded.e_ident[ITPLD_EI_MAG3], (itpld_elf_ubyte_t const[]){ ITPLD_EI_MAG3_VAL }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 4, &decoded.e_ident[ITPLD_EI_CLASS], (itpld_elf_ubyte_t const[]){ ITPLD_ELFCLASS64 }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 5, &decoded.e_ident[ITPLD_EI_DATA], (itpld_elf_ubyte_t const[]){ ITPLD_ELFDATA2LSB }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 6, &decoded.e_ident[ITPLD_EI_VERSION], (itpld_elf_ubyte_t const[]){ ITPLD_EV_CURRENT }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 7, &decoded.e_ident[ITPLD_EI_OSABI], (itpld_elf_ubyte_t const[]){ ITPLD_ELFOSABI_NONE }, 1));
	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, 0, 8, &decoded.e_ident[ITPLD_EI_ABIVERSION], (itpld_elf_ubyte_t const[]){ 0 }, 1));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 16, &decoded.e_type, (itpld_elf_utwobyte_t const[]){ ITPLD_ET_REL }, 1));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 18, &decoded.e_machine, (itpld_elf_utwobyte_t const[]){ ITPLD_EM_X86_64 }, 1));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, 0, 20, &decoded.e_version, (itpld_elf_ufourbyte_t const[]){ ITPLD_EV_CURRENT }, 1));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, 0, 24, &decoded.e_entry, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, 0, 32, &decoded.e_phoff, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, 0, 40, &decoded.e_shoff, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, 0, 48, &decoded.e_flags, (itpld_elf_ufourbyte_t const[]){ ITPLD_EF_X86_64_NONE }, 1));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 52, &decoded.e_ehsize, (itpld_elf_utwobyte_t const[]){ ITPLD_ELF64_EHDR_SIZE }, 1));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 54, &decoded.e_phentsize, (itpld_elf_utwobyte_t const[]){ 0, ITPLD_ELF64_PHDR_SIZE }, 2));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 56, &decoded.e_phnum, NULL, 0));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 58, &decoded.e_shentsize, (itpld_elf_utwobyte_t const[]){ ITPLD_ELF64_SHDR_SIZE }, 1));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 60, &decoded.e_shnum, NULL, 0));
	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, 0, 62, &decoded.e_shstrndx, NULL, 0));
	/* clang-format on */

	*out = decoded;
	return ITPLD_STATUS_OK;
}

itpld_status_t
itpld_elf_decode_shdr(itpld_elf_bytes_t bytes, size_t e_shoff, itpld_elf64_shdr_t * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (bytes.data == NULL || !itpld_in_range(e_shoff, ITPLD_ELF64_SHDR_SIZE, bytes.bytescnt))
		return ITPLD_STATUS_INVAL_FMT;

	itpld_elf64_shdr_t decoded = { 0 };

	itpld_elf_ueightbyte_t const supported_flags = ITPLD_SHF_WRITE | ITPLD_SHF_ALLOC | ITPLD_SHF_EXECINSTR |
						       ITPLD_SHF_MERGE | ITPLD_SHF_STRINGS | ITPLD_SHF_INFO_LINK |
						       ITPLD_SHF_GROUP;

	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, e_shoff, 0, &decoded.sh_name, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(
	    bytes,
	    e_shoff,
	    4,
	    &decoded.sh_type,
	    (itpld_elf_ufourbyte_t const[]){ ITPLD_SHT_NULL,
					     ITPLD_SHT_PROGBITS,
					     ITPLD_SHT_SYMTAB,
					     ITPLD_SHT_STRTAB,
					     ITPLD_SHT_RELA,
					     ITPLD_SHT_NOBITS,
					     ITPLD_SHT_GROUP },
	    6
	));

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_shoff, 8, &decoded.sh_flags, NULL, 0));
	if ((decoded.sh_flags & ~supported_flags) != 0) return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_shoff, 16, &decoded.sh_addr, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_shoff, 24, &decoded.sh_offset, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_shoff, 32, &decoded.sh_size, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, e_shoff, 40, &decoded.sh_link, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, e_shoff, 44, &decoded.sh_info, NULL, 0));

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_shoff, 48, &decoded.sh_addralign, NULL, 0));
	if ((decoded.sh_addralign & (decoded.sh_addralign - 1)) != 0 && decoded.sh_addralign != 0)
		return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_shoff, 56, &decoded.sh_entsize, NULL, 0));

	*out = decoded;
	return ITPLD_STATUS_OK;
}

static bool
itpld_elf_validate_st_info_(itpld_elf_ubyte_t st_info)
{
	itpld_elf_ubyte_t stb = ITPLD_ELF64_ST_BIND(st_info);
	itpld_elf_ubyte_t stt = ITPLD_ELF64_ST_TYPE(st_info);

	switch (stb) {
	case ITPLD_STB_LOCAL:
	case ITPLD_STB_GLOBAL:
	case ITPLD_STB_WEAK:
		switch (stt) {
		case ITPLD_STT_NOTYPE:
		case ITPLD_STT_OBJECT:
		case ITPLD_STT_FUNC:
		case ITPLD_STT_SECTION:
		case ITPLD_STT_FILE:
			return true;
		default:
			return false;
		}
	default:
		return false;
	}
}

static bool
itpld_elf_validate_st_other_(itpld_elf_ubyte_t st_other)
{
	if ((st_other & ~0x03U) != 0) return false;

	itpld_elf_ubyte_t stvisibility = ITPLD_ELF64_ST_VISIBILITY(st_other);
	return (bool)(stvisibility == ITPLD_STV_DEFAULT || stvisibility == ITPLD_STV_HIDDEN);
}

itpld_status_t
itpld_elf_decode_sym(itpld_elf_bytes_t bytes, size_t sym_offset, itpld_elf64_sym_t * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (bytes.data == NULL || !itpld_in_range(sym_offset, ITPLD_ELF64_SYM_SIZE, bytes.bytescnt))
		return ITPLD_STATUS_INVAL_FMT;

	itpld_elf64_sym_t decoded = { 0 };

	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, sym_offset, 0, &decoded.st_name, NULL, 0));

	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, sym_offset, 4, &decoded.st_info, NULL, 0));
	if (!itpld_elf_validate_st_info_(decoded.st_info)) return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_ubyte_if_valid_(bytes, sym_offset, 5, &decoded.st_other, NULL, 0));
	if (!itpld_elf_validate_st_other_(decoded.st_other)) return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_utwobyte_if_valid_(bytes, sym_offset, 6, &decoded.st_shndx, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, sym_offset, 8, &decoded.st_value, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, sym_offset, 16, &decoded.st_size, NULL, 0));

	*out = decoded;
	return ITPLD_STATUS_OK;
}

static bool
itpld_elf_validate_r_info_(itpld_elf_ueightbyte_t r_info)
{
	itpld_elf_ueightbyte_t r_type = ITPLD_ELF64_R_TYPE(r_info);

	return (bool)(r_type == ITPLD_R_X86_64_NONE || r_type == ITPLD_R_X86_64_64 || r_type == ITPLD_R_X86_64_PC32 ||
		      r_type == ITPLD_R_X86_64_PLT32 || r_type == ITPLD_R_X86_64_32 || r_type == ITPLD_R_X86_64_32S);
}

itpld_status_t
itpld_elf_decode_rela(itpld_elf_bytes_t bytes, size_t rela_offset, itpld_elf64_rela_t * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (bytes.data == NULL || !itpld_in_range(rela_offset, ITPLD_ELF64_RELA_SIZE, bytes.bytescnt))
		return ITPLD_STATUS_INVAL_FMT;

	itpld_elf64_rela_t decoded = { 0 };

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, rela_offset, 0, &decoded.r_offset, NULL, 0));

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, rela_offset, 8, &decoded.r_info, NULL, 0));
	if (!itpld_elf_validate_r_info_(decoded.r_info)) return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_eightbyte_if_valid_(bytes, rela_offset, 16, &decoded.r_addend, NULL, 0));

	*out = decoded;
	return ITPLD_STATUS_OK;
}

itpld_status_t
itpld_elf_decode_phdr(itpld_elf_bytes_t bytes, size_t e_phoff, itpld_elf64_phdr_t * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (bytes.data == NULL || !itpld_in_range(e_phoff, ITPLD_ELF64_PHDR_SIZE, bytes.bytescnt))
		return ITPLD_STATUS_INVAL_FMT;

	itpld_elf64_phdr_t decoded = { 0 };

	itpld_elf_ueightbyte_t const supported_flags = ITPLD_PF_R | ITPLD_PF_W | ITPLD_PF_X;

	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(
	    bytes, e_phoff, 0, &decoded.p_type, (itpld_elf_ufourbyte_t const[]){ ITPLD_PT_NULL, ITPLD_PT_LOAD }, 2
	));

	ITPLD_TRY(itpld_elf_read_ufourbyte_if_valid_(bytes, e_phoff, 4, &decoded.p_flags, NULL, 0));
	if ((decoded.p_flags & ~supported_flags) != 0) return ITPLD_STATUS_INVAL_FMT;

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_phoff, 8, &decoded.p_offset, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_phoff, 16, &decoded.p_vaddr, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_phoff, 24, &decoded.p_paddr, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_phoff, 32, &decoded.p_filesz, NULL, 0));
	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_phoff, 40, &decoded.p_memsz, NULL, 0));

	ITPLD_TRY(itpld_elf_read_ueightbyte_if_valid_(bytes, e_phoff, 48, &decoded.p_align, NULL, 0));
	if ((decoded.p_align & (decoded.p_align - 1)) != 0 && decoded.p_align != 0) return ITPLD_STATUS_INVAL_FMT;

	*out = decoded;
	return ITPLD_STATUS_OK;
}
/* NOLINTEND(readability-function-cognitive-complexity) */

#include "elf/elf_bytes.h"

#include "common/checked.h"
#include "common/status.h"
#include "elf/elf_types.h"

#include <string.h>

#define ITPLD_DEFINE_ELF_READ_GENERIC(type)                                                                            \
	itpld_status_t itpld_elf_read_##type(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_##type##_t * out)       \
	{                                                                                                              \
		if (out == NULL || bytes.data == NULL) return ITPLD_STATUS_INVAL_ARG;                                  \
		if (!itpld_in_range(offset, sizeof(*out), bytes.bytescnt)) return ITPLD_STATUS_INVAL_FMT;              \
		memcpy(out, bytes.data + offset, sizeof(*out));                                                        \
		return ITPLD_STATUS_OK;                                                                                \
	}

ITPLD_DEFINE_ELF_READ_GENERIC(ubyte)
ITPLD_DEFINE_ELF_READ_GENERIC(utwobyte)
ITPLD_DEFINE_ELF_READ_GENERIC(fourbyte)
ITPLD_DEFINE_ELF_READ_GENERIC(ufourbyte)
ITPLD_DEFINE_ELF_READ_GENERIC(eightbyte)
ITPLD_DEFINE_ELF_READ_GENERIC(ueightbyte)

#undef ITPLD_DEFINE_ELF_READ_GENERIC

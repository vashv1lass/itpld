#include "elf/elf_bytes.h"

#include "common/checked.h"
#include "common/status.h"
#include "elf/elf_types.h"

#include <string.h>

static itpld_status_t
itpld_elf_read_bytes_(itpld_elf_bytes_t bytes, size_t offset, size_t outsz, void * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (!itpld_in_range(offset, outsz, bytes.bytescnt) || bytes.data == NULL) return ITPLD_STATUS_INVAL_FMT;

	memcpy(out, bytes.data + offset, outsz);

	return ITPLD_STATUS_OK;
}

itpld_status_t
itpld_elf_read_ubyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_ubyte_t * out)
{
	return itpld_elf_read_bytes_(bytes, offset, sizeof(*out), out);
}

itpld_status_t
itpld_elf_read_utwobyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_utwobyte_t * out)
{
	return itpld_elf_read_bytes_(bytes, offset, sizeof(*out), out);
}

itpld_status_t
itpld_elf_read_fourbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_fourbyte_t * out)
{
	return itpld_elf_read_bytes_(bytes, offset, sizeof(*out), out);
}

itpld_status_t
itpld_elf_read_ufourbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_ufourbyte_t * out)
{
	return itpld_elf_read_bytes_(bytes, offset, sizeof(*out), out);
}

itpld_status_t
itpld_elf_read_eightbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_eightbyte_t * out)
{
	return itpld_elf_read_bytes_(bytes, offset, sizeof(*out), out);
}

itpld_status_t
itpld_elf_read_ueightbyte(itpld_elf_bytes_t bytes, size_t offset, itpld_elf_ueightbyte_t * out)
{
	return itpld_elf_read_bytes_(bytes, offset, sizeof(*out), out);
}

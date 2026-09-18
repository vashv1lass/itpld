#include "elf/elf_decode.h"

#include "common/status.h"
#include "elf/elf_bytes.h"
#include "elf/elf_constants.h"
#include "elf/elf_types.h"

static itpld_status_t
itpld_elf_decode_ehdr_ei_magic_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ubyte_t ei_mag0_val;
	stat = itpld_elf_read_ubyte(bytes, 0, &ei_mag0_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	itpld_elf_ubyte_t ei_mag1_val;
	stat = itpld_elf_read_ubyte(bytes, 1, &ei_mag1_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	itpld_elf_ubyte_t ei_mag2_val;
	stat = itpld_elf_read_ubyte(bytes, 2, &ei_mag2_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	itpld_elf_ubyte_t ei_mag3_val;
	stat = itpld_elf_read_ubyte(bytes, 3, &ei_mag3_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	if (ei_mag0_val != ITPLD_EI_MAG0_VAL || ei_mag1_val != ITPLD_EI_MAG1_VAL || ei_mag2_val != ITPLD_EI_MAG2_VAL ||
	    ei_mag3_val != ITPLD_EI_MAG3_VAL)
		return ITPLD_STATUS_INVAL_FMT;

	out->e_ident[ITPLD_EI_MAG0] = ei_mag0_val;
	out->e_ident[ITPLD_EI_MAG1] = ei_mag1_val;
	out->e_ident[ITPLD_EI_MAG2] = ei_mag2_val;
	out->e_ident[ITPLD_EI_MAG3] = ei_mag3_val;

	return ITPLD_STATUS_OK;
}

static itpld_status_t
itpld_elf_decode_ehdr_ei_class_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ubyte_t ei_class_val;
	stat = itpld_elf_read_ubyte(bytes, 4, &ei_class_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ei_class_val) {
	case ITPLD_ELFCLASS64:
		out->e_ident[ITPLD_EI_CLASS] = ei_class_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ei_data_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ubyte_t ei_data_val;
	stat = itpld_elf_read_ubyte(bytes, 5, &ei_data_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ei_data_val) {
	case ITPLD_ELFDATA2LSB:
		out->e_ident[ITPLD_EI_DATA] = ei_data_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ei_version_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ubyte_t ei_version_val;
	stat = itpld_elf_read_ubyte(bytes, 6, &ei_version_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ei_version_val) {
	case ITPLD_EV_CURRENT:
		out->e_ident[ITPLD_EI_VERSION] = ei_version_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ei_osabi_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ubyte_t ei_osabi_val;
	stat = itpld_elf_read_ubyte(bytes, 7, &ei_osabi_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ei_osabi_val) {
	case ITPLD_ELFOSABI_NONE:
		out->e_ident[ITPLD_EI_OSABI] = ei_osabi_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ei_abiversion_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ubyte_t ei_abiversion_val;
	stat = itpld_elf_read_ubyte(bytes, 8, &ei_abiversion_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ei_abiversion_val) {
	case 0:
		out->e_ident[ITPLD_EI_ABIVERSION] = ei_abiversion_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ei_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	stat = itpld_elf_decode_ehdr_ei_magic_(bytes, out);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ei_class_(bytes, out);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ei_data_(bytes, out);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ei_version_(bytes, out);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ei_osabi_(bytes, out);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ei_abiversion_(bytes, out);
	if (stat != ITPLD_STATUS_OK) return stat;

	return ITPLD_STATUS_OK;
}

static itpld_status_t
itpld_elf_decode_ehdr_et_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf64_half_t et_val;
	stat = itpld_elf_read_utwobyte(bytes, 16, &et_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (et_val) {
	case ITPLD_ET_REL:
		out->e_type = et_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_em_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t	   stat = ITPLD_STATUS_OK;
	itpld_elf64_half_t em_val;
	stat = itpld_elf_read_utwobyte(bytes, 18, &em_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (em_val) {
	case ITPLD_EM_X86_64:
		out->e_machine = em_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ev_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ufourbyte_t ev_val;
	stat = itpld_elf_read_ufourbyte(bytes, 20, &ev_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ev_val) {
	case ITPLD_EV_CURRENT:
		out->e_version = ev_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_eentry_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ueightbyte_t eentry_val;
	stat = itpld_elf_read_ueightbyte(bytes, 24, &eentry_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	out->e_entry = eentry_val;

	return ITPLD_STATUS_OK;
}

static itpld_status_t
itpld_elf_decode_ehdr_ephoff_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ueightbyte_t ephoff_val;
	stat = itpld_elf_read_ueightbyte(bytes, 32, &ephoff_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	out->e_phoff = ephoff_val;

	return ITPLD_STATUS_OK;
}

static itpld_status_t
itpld_elf_decode_ehdr_eshoff_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ueightbyte_t eshoff_val;
	stat = itpld_elf_read_ueightbyte(bytes, 40, &eshoff_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	out->e_shoff = eshoff_val;

	return ITPLD_STATUS_OK;
}

static itpld_status_t
itpld_elf_decode_ehdr_ef_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_ufourbyte_t ef_val;
	stat = itpld_elf_read_ufourbyte(bytes, 48, &ef_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ef_val) {
	case ITPLD_EF_X86_64_NONE:
		out->e_flags = ef_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_eehsize_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_utwobyte_t eehsize_val;
	stat = itpld_elf_read_utwobyte(bytes, 52, &eehsize_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (eehsize_val) {
	case ITPLD_ELF64_EHDR_SIZE:
		out->e_ehsize = eehsize_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ephentsize_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_utwobyte_t ephentsize_val;
	stat = itpld_elf_read_utwobyte(bytes, 54, &ephentsize_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (ephentsize_val) {
	case 0:
	case ITPLD_ELF64_PHDR_SIZE:
		out->e_phentsize = ephentsize_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_ephnum_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_utwobyte_t ephnum_val;
	stat = itpld_elf_read_utwobyte(bytes, 56, &ephnum_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	out->e_phnum = ephnum_val;

	return ITPLD_STATUS_OK;
}

static itpld_status_t
itpld_elf_decode_ehdr_eshentsize_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_utwobyte_t eshentsize_val;
	stat = itpld_elf_read_utwobyte(bytes, 58, &eshentsize_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	switch (eshentsize_val) {
	case 0:
	case ITPLD_ELF64_SHDR_SIZE:
		out->e_shentsize = eshentsize_val;
		return ITPLD_STATUS_OK;
	default:
		return ITPLD_STATUS_INVAL_FMT;
	}
}

static itpld_status_t
itpld_elf_decode_ehdr_eshnum_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_utwobyte_t eshnum_val;
	stat = itpld_elf_read_utwobyte(bytes, 60, &eshnum_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	out->e_shnum = eshnum_val;

	return ITPLD_STATUS_OK;
}

static itpld_status_t
itpld_elf_decode_ehdr_eshstrndx_(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf_utwobyte_t eshstrndx_val;
	stat = itpld_elf_read_utwobyte(bytes, 62, &eshstrndx_val);
	if (stat != ITPLD_STATUS_OK) return stat;

	out->e_shstrndx = eshstrndx_val;

	return ITPLD_STATUS_OK;
}

itpld_status_t
itpld_elf_decode_ehdr(itpld_elf_bytes_t bytes, itpld_elf64_ehdr_t * out)
{
	if (out == NULL) return ITPLD_STATUS_INVAL_ARG;
	if (bytes.data == NULL || bytes.bytescnt < ITPLD_ELF64_EHDR_SIZE) return ITPLD_STATUS_INVAL_FMT;

	itpld_status_t stat = ITPLD_STATUS_OK;

	itpld_elf64_ehdr_t decoded = { 0 };

	stat = itpld_elf_decode_ehdr_ei_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_et_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_em_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ev_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_eentry_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ephoff_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_eshoff_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ef_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_eehsize_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ephentsize_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_ephnum_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_eshentsize_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_eshnum_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	stat = itpld_elf_decode_ehdr_eshstrndx_(bytes, &decoded);
	if (stat != ITPLD_STATUS_OK) return stat;

	*out = decoded;
	return ITPLD_STATUS_OK;
}

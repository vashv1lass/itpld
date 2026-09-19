#ifndef ITPLD_COMMON_STATUS_H_
#define ITPLD_COMMON_STATUS_H_

typedef enum itpld_status {
	ITPLD_STATUS_OK,
	ITPLD_STATUS_INVAL_ARG,
	ITPLD_STATUS_OUT_OF_MEM,
	ITPLD_STATUS_IO_ERROR,
	ITPLD_STATUS_INVAL_FMT,
	/* this status may be removed in the near future due to the difficulty of using it along with INVAL_FMT */
	ITPLD_STATUS_UNSUPPORTED,
	ITPLD_STATUS_OVERFLOW
} itpld_status_t;

#endif /* ITPLD_COMMON_STATUS_H_ */

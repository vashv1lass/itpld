#ifndef ITPLD_COMMON_UTILS_H_
#define ITPLD_COMMON_UTILS_H_

/* number of elements in an actual array. do not pass a pointer. */
#define ITPLD_STATIC_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif /* ITPLD_COMMON_UTILS_H_ */

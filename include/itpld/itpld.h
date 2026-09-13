#ifndef ITPLD_ITPLD_H_
#define ITPLD_ITPLD_H_

/*
 * does not take ownership of argv
 * returns process exit code
 * reports errors to stderr, messages to stdout
 * argc == 0 or argv == NULL is perceived as an error,
 * argv must point to an array of at least argc elements
 * every argv[i] must point to a null-terminated string
 *
 * the caller must provide valid argv strings; malformed argv elements are not valid
 */
int
itpld_run(int argc, char ** argv);

#endif /* ITPLD_ITPLD_H_ */

#ifndef ITPLD_ITPLD_H_
#define ITPLD_ITPLD_H_

/*
 * does not take ownership of argv
 * returns process exit code
 * reports errors to stderr, messages to stdout
 */
int
itpld_run(int argc, char ** argv);

#endif /* ITPLD_ITPLD_H_ */

#ifndef __GETOPT_H__
#define __GETOPT_H__

extern char * optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

int getopt(int argc, char * const argv[], const char * optstring);

#endif /* __GETOPT_H__ */

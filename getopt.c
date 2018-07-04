#include "getopt.h"

#include <stdio.h>
#include <string.h>

#define BADCH (int)'?'
#define BADARG (int)':'
#define EMSG ""

char * optarg;
int optind = 1;
int optopt;
int opterr;
int optreset;

int getopt(int argc, char * const argv[], const char * optstring)
{
    static char * place = EMSG; /* option letter processing */
    const char * oli;           /* option letter list index */
    if (optreset || !*place)
    { /* update scanning pointer */
        optreset = 0;
        if (optind >= argc || *(place = argv[optind]) != '-')
        {
            place = EMSG;
            return (-1);
        }
        if (place[1] && *++place == '-')
        { /* found "--" */
            ++optind;
            place = EMSG;
            return (-1);
        }
    } /* option letter okay? */
    if ((optopt = (int)*place++) == (int)':' ||
        !(oli = strchr(optstring, optopt)))
    {
        /*
         * if the user didn't specify '-' as an option,
         * assume it means -1.
         */
        if (optopt == (int)'-') {
            return (-1);
        }
        if (!*place) {
            ++optind;
        }
        if (opterr && *optstring != ':') {
            (void)printf("illegal option -- %c\n", optopt);
        }
        return (BADCH);
    }
    if (*++oli != ':')
    { /* don't need argument */
        optarg = NULL;
        if (!*place)
            ++optind;
    }
    else
    {               /* need an argument */
        if (*place) /* no white space */
            optarg = place;
        else if (argc <= ++optind)
        { /* no arg */
            place = EMSG;
            if (*optstring == ':')
                return (BADARG);
            if (opterr)
                (void)printf("option requires an argument -- %c\n", optopt);
            return (BADCH);
        }
        else /* white space */
            optarg = argv[optind];
        place = EMSG;
        ++optind;
    }
    return (optopt);
}

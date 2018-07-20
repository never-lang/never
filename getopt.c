/*
MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "getopt.h"
#include <stdio.h>
#include <string.h>

char * optarg;
int optind = 1;
int optopt;
int opterr;
int optreset;

int getopt(int argc, char * const argv[], const char * optstring)
{
    static char * place = "";
    const char * oli;
    if (optreset || !*place)
    {
        optreset = 0;
        if (optind >= argc || *(place = argv[optind]) != '-')
        {
            place = "";
            return (-1);
        }
        if (place[1] && *++place == '-')
        {
            ++optind;
            place = "";
            return (-1);
        }
    }
    if ((optopt = (int)*place++) == (int)':' ||
        !(oli = strchr(optstring, optopt)))
    {
        if (optopt == (int)'-')
        {
            return (-1);
        }
        if (!*place)
        {
            ++optind;
        }
        if (opterr && *optstring != ':')
        {
            (void)printf("illegal option -- %c\n", optopt);
        }
        return (int)'?';
    }
    if (*++oli != ':')
    {
        optarg = NULL;
        if (!*place)
            ++optind;
    }
    else
    {
        if (*place)
            optarg = place;
        else if (argc <= ++optind)
        {
            place = "";
            if (*optstring == ':')
                return (int)':';
            if (opterr)
                (void)printf("option requires an argument -- %c\n", optopt);
            return (int)'?';
        }
        else
            optarg = argv[optind];
        place = "";
        ++optind;
    }
    return (optopt);
}

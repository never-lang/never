/**
 * Copyright 2018 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <string.h>
#include "nev.h"

const char * usage = "usage: %s <file name | -e \"one line of program\">\n";

static void print_result(object * result)
{
    if (result->type == OBJECT_INT)
    {
         printf("result is %d\n", result->int_value);
    }
    else if (result->type == OBJECT_FLOAT)
    {
        printf("result is %.2f\n", result->float_value);
    }
    else
    {
        printf("unknown result type\n");
    }
}

int main(int argc, char * argv[])
{
    int ret = -1;
    object result = { 0 };

    if (argc < 2)
    {
        printf("%s: no input files\n", argv[0]);
        printf(usage, argv[0]);
        return 0;
    }

    if (strncmp("-e", argv[1], 2) == 0)
    {
        ret = parse_str_and_exec(argv[2], argc - 3, argv + 3, &result);
    }
    else
    {
        ret = parse_file_and_exec(argv[1], argc - 2, argv + 2, &result);
    }

    if (ret == 0)
    {
        print_result(&result);
    }

    return ret;
}



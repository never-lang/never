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

#include "getopt.h"
#include "nev.h"
#include <stdio.h>
#include <string.h>

static void print_usage(const char * exe)
{
    printf("usage: %s [-m memory size (default: %u)] [-s stack size (default: "
           "%u)] -f file name | -e \"one line of program\"\n",
           exe, DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);
}

static int get_result(object * result)
{
    int ret = 0;

    if (result->type == OBJECT_INT)
    {
        ret = result->int_value;
    }
    else if (result->type == OBJECT_FLOAT)
    {
        ret = (int)result->float_value;
    }
    else
    {
        fprintf(stderr, "unknown result type %d\n", result->type);
    }

    return ret;
}

int never(const char * src)
{
    int ret = 0;

    program * prog = program_new();

    ret = nev_compile_str(src, prog);
    if (ret == 0)
    {
        object result = { 0 };
        
        vm * machine = vm_new(DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);

        ret = nev_prepare(prog, "main");
        if (ret == 0)
        {
            ret = nev_execute(prog, machine, &result);
            if (ret == 0)
            {
                ret = get_result(&result);
            }
        }

        vm_delete(machine);
    }

#ifdef NO_FFI
    printf("\r\n");
    fflush(stdout);
#endif

    program_delete(prog);

    return ret;
}

int main(int argc, char * argv[])
{
    const char * exe = argv[0];
    const char * arg = NULL;
    int fflag = 0, eflag = 0;
    unsigned int vm_mem_size = 0, vm_stack_size = 0;

    while (getopt(argc, argv, "f:e:m:s:") != -1)
    {
        switch (optopt)
        {
        case 'f':
            arg = optarg;
            fflag = 1;
            break;
        case 'e':
            arg = optarg;
            eflag = 1;
            break;
        case 'm':
            sscanf(optarg, "%u", &vm_mem_size);
            break;
        case 's':
            sscanf(optarg, "%u", &vm_stack_size);
            break;
        case '?':
        default:
            print_usage(exe);
            return -1;
        }
    }
    argc -= optind;
    argv += optind;

    if (vm_mem_size < 1)
    {
        vm_mem_size = DEFAULT_VM_MEM_SIZE;
    }

    if (vm_stack_size < 1)
    {
        vm_stack_size = DEFAULT_VM_STACK_SIZE;
    }

    if (eflag)
    {
        int ret;
        object result = { 0 };

        ret = nev_compile_str_and_exec(arg, argc, argv, &result, vm_mem_size,
                                       vm_stack_size);

        if (ret == 0)
        {
            return get_result(&result);
        }
        else
        {
            return 1;
        }
    }

    if (fflag)
    {
        int ret;
        object result = { 0 };

        ret = nev_compile_file_and_exec(arg, argc, argv, &result, vm_mem_size,
                                        vm_stack_size);

        if (ret == 0)
        {
            return get_result(&result);
        }
        else
        {
            return 1;
        }
    }

    printf("%s: no input files\n", exe);
    print_usage(exe);

    return 1;
}



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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "never.h"
#include "scanner.h"
#include "parser.h"
#include "typecheck.h"
#include "optimize.h"
#include "tailrec.h"
#include "gencode.h"
#include "bytecode.h"
#include "vm.h"
#include "utils.h"
#include "libmath.h"

#define VM_MEM_SIZE 5000
#define VM_STACK_SIZE 200

extern FILE * yyin;
extern int parse_result;

int parse(bytecode ** code_arr, unsigned int * code_size)
{
    int ret = 0;
    never * nev = NULL;

    parse_result = 0;
    yyparse(&nev);
    if ((ret = parse_result) == 0)
    {
        libmath_add_funcs(nev->funcs);

        ret = never_sem_check(nev);
        if (ret == 0)
        {
            ret = never_optimize(nev);
            if (ret == 0)
            {
                ret = never_gencode(nev);
                never_tailrec(nev);
                if (ret == 0)
                {
                    bytecode_list * code;

                    code = bytecode_new();

                    never_emit(nev, code);

                    bytecode_func_addr(code);

                    /* print_functions(nev); */
                    /* bytecode_print(code); */

                    bytecode_to_array(code, code_arr, code_size);

                    bytecode_delete(code);
                }
            }
        }
    }

    if (nev != NULL)
    {
        never_delete(nev);
    }

    return ret;
}

int execute(bytecode * code_arr, unsigned int code_size)
{
    int ret = 0;
    int result = 0;
    vm * machine = NULL;

    if (code_arr == NULL)
    {
        return 0;
    }

    /* bytecode_array_print(code_arr, code_size); */

    machine = vm_new(VM_MEM_SIZE, VM_STACK_SIZE);

    ret = vm_execute(machine, code_arr, code_size, &result);
    if (ret == 0)
    {
        printf("result is %d\n", result);
    }

    vm_delete(machine);
    bytecode_array_delete(code_arr);

    return 0;
}

int parse_file_and_exec(const char * file_name)
{
    int ret = 0;
    bytecode * code_arr = NULL;
    unsigned int code_size = 0;

    set_utils_file_name(file_name);
    
    yyin = fopen(file_name, "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "cannot open %s. %s\n", file_name, strerror(errno));
        exit(1);
    }

    ret = parse(&code_arr, &code_size);
    if (ret == 0)
    {
        ret = execute(code_arr, code_size);
    }

    fclose(yyin);
    yylex_destroy();

    return ret;
}

int parse_and_exec(const char * src)
{
    int ret = 0;
    unsigned int code_size = 0;
    bytecode * code_arr = NULL;

    scan_string(src);

    ret = parse(&code_arr, &code_size);
    if (ret == 0)
    {
        ret = execute(code_arr, code_size);
    }

    yylex_destroy();

    return ret;
}


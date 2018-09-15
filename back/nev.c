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
#include "bytecode.h"
#include "gencode.h"
#include "libmath.h"
#include "never.h"
#include "optimize.h"
#include "parser.h"
#include "program.h"
#include "scanner.h"
#include "tailrec.h"
#include "typecheck.h"
#include "utils.h"
#include "vm.h"
#include "module.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARSE_STR 1
#define PARSE_FILE 2

extern FILE * yyin;
extern int parse_result;

int never_func_main_params(never * nev, object ** params,
                           unsigned int * param_count)
{
    object * object_param = NULL;
    symtab_entry * entry = NULL;

    *params = NULL;
    *param_count = 0;

    entry = symtab_lookup(nev->stab, "main", SYMTAB_FLAT);
    if (entry != NULL && entry->type == SYMTAB_FUNC &&
        entry->func_value != NULL)
    {
        func * func_value = entry->func_value;
        if (func_value->decl->params != NULL)
        {
            *param_count = func_value->decl->params->count;
            *params = object_param = malloc(sizeof(object) * (*param_count));
            memset(*params, 0, sizeof(object) * (*param_count));

            param_list_node * node = func_value->decl->params->tail;
            while (node != NULL)
            {
                param * value = node->value;
                if (value->type == PARAM_INT)
                {
                    object_param->type = OBJECT_INT;
                }
                else if (value->type == PARAM_FLOAT)
                {
                    object_param->type = OBJECT_FLOAT;
                }
                object_param++;
                node = node->next;
            }
        }
    }

    return 0;
}

int nev_compile_prog(program * prog)
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
                    never_emit(nev, prog->module_value);

                    print_functions(nev);
                    module_print(prog->module_value);
                    
                    never_func_main_params(nev, &prog->params, &prog->param_count);
                    module_close(prog->module_value);
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

int nev_compile(const char * input, program * prog, int type)
{
    int ret;

    if (type == PARSE_STR)
    {
        scan_string(input);
    }
    else if (type == PARSE_FILE)
    {
        set_utils_file_name(input);

        yyin = fopen(input, "r");
        if (yyin == NULL)
        {
            fprintf(stderr, "cannot open %s. %s\n", input, strerror(errno));
            exit(1);
        }
    }

    ret = nev_compile_prog(prog);

    if (type == PARSE_FILE)
    {
        fclose(yyin);
    }

    yylex_destroy();

    return ret;
}

int nev_compile_str(const char * str, program * prog)
{
    return nev_compile(str, prog, PARSE_STR);
}

int nev_compile_file(const char * file_name, program * prog)
{
    return nev_compile(file_name, prog, PARSE_FILE);
}

int nev_execute(program * prog, object * result, unsigned int vm_mem_size,
                unsigned int vm_stack_size)
{
    int ret = 0;
    vm * machine = NULL;

    if (prog->module_value->code_arr == NULL)
    {
        return 0;
    }

    /* bytecode_array_print(code_arr, code_size); */

    machine = vm_new(vm_mem_size, vm_stack_size);

    ret = vm_execute(machine, prog, result);

    vm_delete(machine);

    return ret;
}

int argc_to_program(program * prog, unsigned int argc, char * argv[])
{
    unsigned int i;

    if (prog->param_count > argc)
    {
        fprintf(stderr, "too few parameters, expected %d got %d\n",
                prog->param_count, argc);
        return 1;
    }

    for (i = 0; i < prog->param_count; i++)
    {
        if (prog->params[i].type == OBJECT_INT)
        {
            prog->params[i].int_value = atoi(argv[i]);
        }
        else if (prog->params[i].type == OBJECT_FLOAT)
        {
            prog->params[i].float_value = atof(argv[i]);
        }
    }

    return 0;
}

int nev_compile_and_exec(const char * input, int argc, char * argv[],
                         object * result, int type, unsigned int vm_mem_size,
                         unsigned int vm_stack_size)
{
    int ret = 0;
    program * prog = program_new();

    ret = nev_compile(input, prog, type);
    if (ret == 0)
    {
        ret = argc_to_program(prog, argc, argv);
        if (ret == 0)
        {
            ret = nev_execute(prog, result, vm_mem_size, vm_stack_size);
        }
    }

    program_delete(prog);

    return ret;
}

int nev_compile_file_and_exec(const char * file_name, int argc, char * argv[],
                              object * result, unsigned int vm_mem_size,
                              unsigned int vm_stack_size)
{
    return nev_compile_and_exec(file_name, argc, argv, result, PARSE_FILE,
                                vm_mem_size, vm_stack_size);
}

int nev_compile_str_and_exec(const char * src, unsigned int argc, char * argv[],
                             object * result, unsigned int vm_mem_size,
                             unsigned int vm_stack_size)
{
    return nev_compile_and_exec(src, argc, argv, result, PARSE_STR, vm_mem_size,
                                vm_stack_size);
}

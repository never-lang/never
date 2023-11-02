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
#include "emit.h"
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
#include "seq.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PARSE_STR 1
#define PARSE_FILE 2

extern FILE * yyin;
extern int parse_result;

static module_decl * module_decl_new_stdlib()
{
    seq_list * stdlib_funcs = seq_list_new();
    use_list * stdlib_uses = use_list_new();
    never * stdlib_never = never_new(stdlib_uses, NULL, stdlib_funcs);
    module_decl * module_stdlib = module_decl_new(strdup("stdlib"), stdlib_never);

    libmath_add_funcs(stdlib_funcs);

    return module_stdlib;
}

static module_decl * module_decl_new_modules()
{
    use_list * global_uses = use_list_new();
    never * global_never = never_new(global_uses, NULL, NULL);
    global_never->stab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);
    module_decl * module_modules = module_decl_new(strdup("modules"), global_never);

    return module_modules;
}

static int nev_compile_prog_modules(const char * input, program * prog, module_decl * module_main)
{
    int ret = 0;
    int typecheck_res = TYPECHECK_SUCC;
    module_decl * module_stdlib = module_decl_new_stdlib();
    module_decl * module_modules = module_decl_new_modules();

    module_decl_check_type(module_modules, NULL, module_stdlib, &typecheck_res);
    if (typecheck_res != 0)
    {
        fprintf(stderr, "cannot typecheck module global\n");
        assert(0);
    }

    if (module_main->id == NULL)
    {
        module_main->id = strdup(input);
    }

    main_check_type(module_modules, module_stdlib, module_main, &typecheck_res);
    if (typecheck_res == 0)
    {
        ret = module_decl_optimize(module_main);
        if (ret == 0)
        {
            ret = module_decl_tailrec(module_main);
            if (ret == 0)
            {
                ret = main_emit(module_modules, module_main, prog->module_value);
                if (ret == 0)
                {
                    module_close(prog->module_value);
                }
                /* module_print(prog->module_value); */
            }
        }
    }
    else
    {
        ret = typecheck_res;
    }

    module_decl_delete(module_modules);
    module_decl_delete(module_stdlib);

    return ret;
}

static int nev_compile_prog(const char * input, program * prog)
{
    int ret = 0;
    module_decl * module_main = NULL;

    set_line_no(1);
    parse_result = 0;

    set_msg_buffer(&prog->msg_count, &prog->msg_array_size, &prog->msg_array);

    yyparse(&module_main);
    if ((ret = parse_result) == 0)
    {
        ret = nev_compile_prog_modules(input, prog, module_main);
    }

    if (module_main != NULL)
    {
        module_decl_delete(module_main);
    }

    return ret;
}

static int nev_compile(const char * input, program * prog, int type)
{
    int ret = 0;

    if (type == PARSE_STR)
    {
        ret = scan_string(input);
        if (ret == 0)
        {
            ret = nev_compile_prog(get_utils_file_name(), prog);
        }
    }
    else if (type == PARSE_FILE)
    {
        ret = scan_file(input);
        if (ret == 0)
        {
            ret = nev_compile_prog(input, prog);
        }
    }

    scanner_destroy();

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

int nev_execute(program * prog, vm * machine, object * result)
{
    if (prog->module_value->code_arr == NULL)
    {
        return 1;
    }

    if (machine->initialized == 0)
    {
        machine->ip = 0;
        machine->initialized = 1;
    }
    else
    {
        machine->ip = prog->module_value->code_entry;
    }

    /*printf("machine->ip %u\n", machine->ip);
    bytecode_array_print(prog->module_value->code_arr,
                        prog->module_value->code_size);*/

    return vm_execute(machine, prog, result);
}

int nev_prepare(program * prog, const char * entry_name)
{
    functab_entry * entry = functab_lookup(prog->module_value->functab_value, entry_name);
    if (entry == NULL)
    {
        fprintf(stderr, "cannot find entry %s\n", entry_name);
        return 1;
    }

    prog->params_count = entry->params_count;
    prog->params = entry->params;
    prog->entry_addr = entry->func_addr;

    return 0;
}

int nev_prepare_argc_argv(program * prog, const char * entry_name, unsigned int argc, char * argv[])
{
    unsigned int i;

    functab_entry * entry = functab_lookup(prog->module_value->functab_value, entry_name);
    if (entry == NULL)
    {
        fprintf(stderr, "cannot find entry %s\n", entry_name);
        return 1;
    }

    prog->params_count = entry->params_count;
    prog->params = entry->params;
    prog->entry_addr = entry->func_addr;

    if (entry->entry_type == FUNC_ENTRY_TYPE_PARAM_LIST &&
        argc < prog->params_count)
    {
        fprintf(stderr, "too few parameters, expected %d got %d\n",
                prog->params_count, argc);
        return 1;
    }

    for (i = 0; i < prog->params_count; i++)
    {
        if (prog->params[i].type == OBJECT_INT)
        {
            prog->params[i].int_value = atoi(argv[i]);
        }
        else if (prog->params[i].type == OBJECT_FLOAT)
        {
            prog->params[i].float_value = atof(argv[i]);
        }
        else if (prog->params[i].type == OBJECT_STRING_REF)
        {
            prog->params[i].string_value = argv[i];
        }
        else if (prog->params[i].type == OBJECT_STRING_ARR)
        {
            prog->params[i].string_arr_value = object_str_arr_new(argc, argv);
        }
        else
        {
            assert(0);
        }
    }

    return 0;
}

int nev_compile_and_exec(const char * input,
                         int argc,
                         char * argv[],
                         object * result,
                         int type,
                         unsigned int vm_mem_size,
                         unsigned int vm_stack_size)
{
    int ret = 0;
    vm * machine = NULL;
    program * prog = program_new();

    ret = nev_compile(input, prog, type);
    if (ret == 0)
    {
        ret = nev_prepare_argc_argv(prog, "main", argc, argv);
        if (ret == 0)
        {
            machine = vm_new(vm_mem_size, vm_stack_size);

            ret = nev_execute(prog, machine, result);

            vm_delete(machine);
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

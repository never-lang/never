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
#include "module.h"
#include <stdlib.h>

module * module_new()
{
    module * value = (module *)malloc(sizeof(module));
    
    value->strtab_value = strtab_new(32);
    value->strtab_array = NULL;
    value->strtab_size = 0;
    
    value->code_arr = NULL;
    value->code_size = 0;
    value->code = bytecode_new();

    value->exctab_value = exception_tab_new(32);
    
    return value;
}

void module_delete(module * value)
{
    if (value->strtab_value != NULL)
    {
        strtab_delete(value->strtab_value);
    }
    if (value->strtab_array != NULL)
    {
        strtab_array_delete(value->strtab_array, value->strtab_size);
    }
    if (value->code != NULL)
    {
        bytecode_delete(value->code);
    }
    if (value->code_arr != NULL)
    {
        bytecode_array_delete(value->code_arr);
    }
    if (value->exctab_value != NULL)
    {
        exception_tab_delete(value->exctab_value);
    }
    free(value);
}

void module_close(module * value)
{
    if (value->strtab_value != NULL)
    {
        strtab_to_array(value->strtab_value, &value->strtab_array, &value->strtab_size);
    }
    if (value->code != NULL)
    {
        bytecode_func_addr(value->code);
        bytecode_to_array(value->code, &value->code_arr, &value->code_size);
    }
}

void module_print(module * value)
{
    if (value->strtab_array != NULL)
    {
        strtab_array_print(value->strtab_array, value->strtab_size);
    }
    if (value->code != NULL)
    {
        bytecode_list_print(value->code);
    }
    if (value->exctab_value != NULL)
    {
        exception_tab_print(value->exctab_value);
    }
}



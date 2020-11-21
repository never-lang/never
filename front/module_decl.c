/** 
 * Copyright 2020 Slawomir Maludzinski
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
#include "module_decl.h"
#include "never.h"
#include <stdlib.h>
#include <stdio.h>

module_decl * module_decl_new(char * id, never * nev)
{
    module_decl * value = (module_decl *)malloc(sizeof(module_decl));

    value->type = MODULE_DECL_TYPE_MOD;
    value->is_main = 0;
    value->is_checked = 0;
    value->id = id;
    value->nev = nev;
    value->line_no = 0;

    return value;
}

module_decl * module_decl_new_ref(char * id, never * nev)
{
    module_decl * value = (module_decl *)malloc(sizeof(module_decl));

    value->type = MODULE_DECL_TYPE_REF;
    value->is_main = 0;
    value->is_checked = 0;
    value->id = id;
    value->nev = nev;
    value->line_no = 0;

    return value;
}

void module_decl_delete(module_decl * value)
{
    switch (value->type)
    {
        case MODULE_DECL_TYPE_MOD:
            if (value->id != NULL)
            {
                free(value->id);
            }
            if (value->nev != NULL)
            {
                never_delete(value->nev);
            }
        break;
        case MODULE_DECL_TYPE_REF:
        break;
    }
    free(value);
}

void module_decl_add_module_ref(module_decl * value, module_decl * other)
{
    module_decl * module_ref = module_decl_new_ref(other->id, other->nev);
    use * use_value = use_new(NULL, module_ref);

    use_list_add_end(value->nev->uses, use_value);
}

void module_decl_print(module_decl * value)
{
    if (value->id != NULL)
    {
        printf("module %s\n", value->id);
    }
}

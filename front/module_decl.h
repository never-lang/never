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
#ifndef __MODULE_DECL_H__
#define __MODULE_DECL_H__

typedef enum module_decl_type
{
    MODULE_DECL_TYPE_MOD = 1,
    MODULE_DECL_TYPE_REF = 2
} module_decl_type;

typedef struct never never;

typedef struct module_decl
{
    module_decl_type type;
    char is_main;
    char is_active;
    char * id;
    never * nev;
    unsigned int line_no;
} module_decl;

module_decl * module_decl_new(char * id, never * nev);
module_decl * module_decl_new_ref(char * id, never * nev);
void module_decl_delete(module_decl * value);

void module_decl_add_module_ref(module_decl * value, module_decl * other);

void module_decl_print(module_decl * value);

#endif /* __MODULE_DECL_H__ */

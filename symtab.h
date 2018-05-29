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
#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "var.h"
#include "func.h"

enum
{
    SYMTAB_FLAT = 0,
    SYMTAB_NESTED = 1
};

enum
{
    SYMTAB_VAR = 1,
    SYMTAB_FUNC = 2
};

typedef struct symtab_entry
{
    int type;
    const char * id;
    union
    {
        var * var_value;
        func * func_value;
    };
    unsigned int syn_level;
} symtab_entry;

typedef struct symtab
{
    unsigned int size;
    unsigned int count;
    struct symtab * parent;
    symtab_entry * entries;
} symtab;

symtab_entry * symtab_entry_new(unsigned int size);
void symtab_entry_delete(symtab_entry * entries);
void symtab_entry_add_var_func(symtab_entry * entries, unsigned int size,
                               int type, const char * id, void * var_func_value);
symtab_entry * symtab_entry_lookup_var_func(symtab_entry * entries, unsigned int size, const char * id);
void symtab_entry_resize(symtab_entry * entries, int size, symtab_entry * entries_new, int size_new);

void symtab_entry_print(symtab_entry * entry);

symtab * symtab_new(unsigned int size, symtab * parent);
void symtab_delete(symtab * tab);

void symtab_add_var(symtab * tab, var * var_value);
void symtab_add_func(symtab * tab, func * func_value);

symtab_entry * symtab_lookup(symtab * tab, const char * id, char nested);

void symtab_set_syn_level(symtab * tab, unsigned int syn_level);
void symtab_func_mark_zero(symtab * tab);

void symtab_resize(symtab * tab);

void symtab_print(symtab * tab);

#endif /* __SYMTAB_H__ */




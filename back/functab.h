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
#ifndef __FUNCTAB_H__
#define __FUNCTAB_H__

typedef struct func func;
typedef struct object object;

typedef enum functab_type
{
    FUNCTAB_FUNC = 0,
    FUNCTAB_ADDR = 1
} functab_type;

typedef struct functab_entry
{
    functab_type type;
    func * func_value;
    char * id;
    unsigned int func_addr;
    int entry_type;
    object * params;
    unsigned int params_count;
} functab_entry;

typedef struct functab
{
    unsigned int size;
    unsigned int count;
    struct functab_entry * entries;
} functab;

functab_entry * functab_entry_new(unsigned int size);
void functab_entry_delete(functab_entry * entries, unsigned int size);

void functab_entry_add_func(functab_entry * entries, unsigned int size,
                            func * func_value, int entry_type,
                            object * params, unsigned int params_count);
functab_entry * functab_entry_lookup(functab_entry * entries, unsigned int size,
                                     const char * id);
void functab_entry_resize(functab_entry * entries, unsigned int size,
                          functab_entry * entries_new, unsigned int size_new);

functab * functab_new(unsigned int size);
void functab_delete(functab * tab);

void functab_resize(functab * tab);
void functab_close(functab * tab);
void functab_add_func(functab * tab, func * func_value, int entry_type,
                      object * params, unsigned int params_count);
functab_entry * functab_lookup(functab * tab, const char * id);

#endif /* __FUNCTAB_H__ */

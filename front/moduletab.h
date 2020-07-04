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
#ifndef __MODULE_TAB_H__
#define __MODULE_TAB_H__

typedef struct moduletab_entry
{
    char * id;
} moduletab_entry;

typedef struct moduletab
{
    unsigned int size;
    unsigned int count;
    moduletab_entry * entries;
} moduletab;

moduletab_entry * moduletab_entry_new(unsigned int size);
void moduletab_entry_delete(moduletab_entry * entries, unsigned int size);

void moduletab_entry_add_module(moduletab_entry * entries, unsigned int size, char * id);
moduletab_entry * moduletab_entry_lookup_module(moduletab_entry * entries, unsigned int size,
                                                const char * id);
void moduletab_entry_resize(moduletab_entry * entries, unsigned int size,
                            moduletab_entry * entries_new, unsigned int size_new);

moduletab * moduletab_new(unsigned int size);
void moduletab_delete(moduletab * tab);

void moduletab_resize(moduletab * tab);
void moduletab_add_module(moduletab * tab, char * id);
moduletab_entry * moduletab_lookup_module(moduletab * tab, const char * id);

#endif /* __MODULE_TAB_H__ */

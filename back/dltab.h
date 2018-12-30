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
#ifndef __DLTAB_H__
#define __DLTAB_H__

typedef struct dltab_entry
{
    char * name;
    void * lib;
} dltab_entry;

typedef struct dltab
{
    unsigned int size;
    unsigned int count;
    dltab_entry * entries;
} dltab;

dltab_entry * dltab_entry_new(unsigned int size);
void dltab_entry_delete(dltab_entry * entries, unsigned int size);

void dltab_entry_add_lib(dltab_entry * entries, unsigned int size, char * name, void * lib);
void * dltab_entry_lookup_lib(dltab_entry * entries, unsigned int size, char * name);
void dltab_entry_resize(dltab_entry * entries, unsigned int size,
                        dltab_entry * entries_new, unsigned int size_new);

dltab * dltab_new(unsigned int size);
void dltab_delete(dltab * tab);

void dltab_resize(dltab * tab);

void dltab_add_lib(dltab * tab, char * name, void * lib);
void * dltab_lookup_lib(dltab * tab, char * name);

#endif /* __DLTAB_H__ */


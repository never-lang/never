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
#ifndef __INTTAB_H__
#define __INTTAB_H__

typedef struct enumtype enumtype;
typedef struct enumerator enumerator;

typedef enum inttab_entry_type
{
    INTTAB_ENTRY_EMPTY = 0,
    INTTAB_ENTRY_USED
} inttab_entry_type;

typedef struct inttab_entry
{
    inttab_entry_type type;
    int value;
    enumerator * enumerator_value;
} inttab_entry;

typedef struct inttab
{
    unsigned int size;
    unsigned int count;
    inttab_entry * entries;
} inttab;

inttab_entry * inttab_entry_new(unsigned int size);
void inttab_entry_delete(inttab_entry * entries);

void inttab_entry_add_enumerator(inttab_entry * entries, unsigned int size, int value, enumerator * enumerator_value);
inttab_entry * inttab_entry_lookup(inttab_entry * entries, unsigned int size, int value);

void inttab_entry_resize(inttab_entry * entries, unsigned int size,
                         inttab_entry * entries_new, unsigned int size_new);

inttab * inttab_new(unsigned int size);
void inttab_delete(inttab * tab);

void inttab_add_enumerator(inttab * tab, int value, enumerator * enumerator_value);
inttab_entry * inttab_lookup(inttab * tab, int value);

void inttab_resize(inttab * tab);

#endif /* __INTTAB_H__ */


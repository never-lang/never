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
#ifndef __STRTAB_H__
#define __STRTAB_H__

typedef struct strtab_entry
{
    char * string;
    unsigned int order;
} strtab_entry;

typedef struct strtab
{
    unsigned int size;
    unsigned int count;
    strtab_entry * entries;
} strtab;

strtab_entry * strtab_entry_new(unsigned int size);
void strtab_entry_delete(strtab_entry * entries, unsigned int size);

unsigned int strtab_entry_add_string(strtab_entry * entries, unsigned int size,
                                     char * string, unsigned int order);
unsigned int strtab_entry_lookup_string(strtab_entry * entries, unsigned int size,
                                        char * string);
void strtab_entry_resize(strtab_entry * entries, unsigned int size,
                         strtab_entry * entries_new, unsigned int size_new);

strtab * strtab_new(unsigned int size);
void strtab_delete(strtab * tab);

void strtab_resize(strtab * tab);
unsigned int strtab_add_string(strtab * tab, char * string);
unsigned int strtab_lookup_string(strtab * tab, char * string);

void strtab_to_array(strtab * tab, char *** strings, unsigned int * size);
void strtab_array_delete(char ** strings, unsigned int size);
void strtab_array_print(char ** strings, unsigned int size);

#endif /* __STRTAB_H__ */



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
#ifndef __EXCTAB_H__
#define __EXCTAB_H__

typedef struct exctab_entry {
    unsigned int block_addr;
    unsigned int handler_addr;
} exctab_entry;

typedef struct exctab {
    unsigned int size;
    unsigned int count;
    exctab_entry * tab;
} exctab;

int exctab_between(exctab_entry * first, exctab_entry * second, unsigned int except_ip);
exctab_entry * exctab_search(exctab_entry * tab, int tab_len, unsigned int except_ip);

exctab * exception_tab_new(unsigned int size);
void exception_tab_delete(exctab * value);

void exception_tab_insert(exctab * value, unsigned int block_addr,
                          unsigned int handler_addr);
unsigned int exception_tab_search(exctab * value, unsigned int except_ip);

void exception_tab_print(exctab * value);

#endif /* __EXCTAB_H__ */



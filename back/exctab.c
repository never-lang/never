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
#include "exctab.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

int exctab_between(exctab_entry * first, exctab_entry * second, unsigned int except_ip)
{
    if (except_ip < first->block_addr)
    {
        return -1;
    }
    else if (except_ip >= second->block_addr)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

exctab_entry * exctab_search(exctab_entry * tab, int tab_len, unsigned int except_ip)
{
    int from = 0;
    int to = tab_len - 1;
    exctab_entry * found = NULL;

    if (tab == NULL || tab_len == 0) return NULL;

    while (from <= to)
    {
        int middle = from + (to - from) / 2;
        int cmp = exctab_between(tab + middle, tab + middle + 1, except_ip);

        if (cmp < 0)
        {
            to = middle - 1;
        }
        else if (cmp > 0)
        {
            from = middle + 1;
        }
        else
        {
            found = tab + middle;
            break;
        }
    }

    return found;
}

exctab * exception_tab_new(unsigned int size)
{
    exctab * value = (exctab *)malloc(sizeof(exctab));
    
    value->size = size;
    value->count = 0;
    value->tab = (exctab_entry *)malloc(size * sizeof(exctab_entry));
    value->tab[0].block_addr = UINT_MAX;
    value->tab[0].handler_addr = UINT_MAX;
    
    return value;
}

void exception_tab_delete(exctab * value)
{
    if (value->tab != NULL)
    {
        free(value->tab);
    }
    free(value);
}

void exception_tab_insert(exctab * value, unsigned int block_addr,
                          unsigned int handler_addr)
{
    unsigned int index = value->count;
    
    value->tab[index].block_addr = block_addr;
    value->tab[index].handler_addr = handler_addr;
    
    value->tab[index + 1].block_addr = UINT_MAX;
    value->tab[index + 1].handler_addr = UINT_MAX;
    
    value->count++;
    if (value->count >= value->size - 1)
    {
        value->size *= 2;
        value->tab = (exctab_entry *)realloc(value->tab, value->size * sizeof(exctab_entry));
    }
}

unsigned int exception_tab_search(exctab * value, unsigned int except_ip)
{
    exctab_entry * res = NULL;

    res = exctab_search(value->tab, value->count, except_ip);
    assert(res != NULL);

    return res->handler_addr;
}

void exception_tab_print(exctab * value)
{
    unsigned int i = 0;
    
    for (i = 0; i < value->count + 1; i++)
    {
        printf("block_addr %u handler_addr %u\n", value->tab[i].block_addr,
                                                  value->tab[i].handler_addr);
    }
}



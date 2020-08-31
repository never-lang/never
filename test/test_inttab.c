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
#include "inttab.h"
#include <stdlib.h>
#include <assert.h>

void test_one()
{
    inttab * tab = inttab_new(16);

    inttab_delete(tab);
}

void test_two()
{
    int value = 0;
    inttab * tab = inttab_new(8);

    for (value = 0; value < 32; value++)
    {
        inttab_add_int(tab, value);
    }

    for (value = 0; value < 32; value++)
    {
        inttab_entry * entry = inttab_lookup(tab, value);
        assert(entry != NULL && entry->value == value);
    }

    inttab_delete(tab);
}

void test_three()
{
    int value = 0;
    inttab * tab = inttab_new(8);

    for (value = -16; value < 16; value++)
    {
        inttab_add_int(tab, value);
    }

    for (value = 16; value < 16; value++)
    {
        inttab_entry * entry = inttab_lookup(tab, value);
        assert(entry != NULL && entry->value == value);
    }

    inttab_delete(tab);
}

void test_four()
{
    int value = 0;
    inttab * tab = inttab_new(8);

    for (value = -16; value < 16; value++)
    {
        inttab_add_int(tab, value);
    }

    for (value = 16; value < 16; value++)
    {
        inttab_entry * entry = inttab_lookup(tab, value);
        assert(entry != NULL && entry->value == value);
    }

    for (value = -32; value < -16; value++)
    {
        inttab_entry * entry = inttab_lookup(tab, value);
        assert(entry == NULL);
    }

    for (value = 16; value < 32; value++)
    {
        inttab_entry * entry = inttab_lookup(tab, value);
        assert(entry == NULL);
    }

    inttab_delete(tab);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    test_four();

    return 0;
}

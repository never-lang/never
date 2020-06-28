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
#include "moduletab.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_one()
{
    moduletab * tab = moduletab_new(4);

    moduletab_delete(tab);
}

void test_two()
{
    moduletab * tab = moduletab_new(4);

    moduletab_add_module(tab, strdup("one"));
    moduletab_add_module(tab, strdup("two"));
    moduletab_add_module(tab, strdup("three"));
    moduletab_add_module(tab, strdup("four"));
    moduletab_add_module(tab, strdup("five"));
    moduletab_add_module(tab, strdup("six"));

    moduletab_delete(tab);
}

void test_three()
{
    moduletab * tab = moduletab_new(4);

    moduletab_add_module(tab, strdup("one"));
    moduletab_add_module(tab, strdup("two"));
    moduletab_add_module(tab, strdup("three"));
    moduletab_add_module(tab, strdup("four"));
    moduletab_add_module(tab, strdup("five"));
    moduletab_add_module(tab, strdup("six"));

    assert(moduletab_lookup_module(tab, "one") != NULL);
    assert(moduletab_lookup_module(tab, "two") != NULL);
    assert(moduletab_lookup_module(tab, "three") != NULL);
    assert(moduletab_lookup_module(tab, "four") != NULL);
    assert(moduletab_lookup_module(tab, "five") != NULL);
    assert(moduletab_lookup_module(tab, "six") != NULL);
    assert(moduletab_lookup_module(tab, "seven") == NULL);
    assert(moduletab_lookup_module(tab, "eight") == NULL);
    assert(moduletab_lookup_module(tab, "nine") == NULL);
    assert(moduletab_lookup_module(tab, "ten") == NULL);

    moduletab_delete(tab);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();

    return 0;
}

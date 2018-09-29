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
#include "strtab.h"
#include <assert.h>

void test_one()
{
    strtab * tab = strtab_new(10);
    strtab_delete(tab);
}

void test_two()
{
    strtab * tab = strtab_new(4);
    
    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "one") == 1);

    assert(tab->size == 4);
    assert(tab->count == 2);
    
    strtab_delete(tab);
}

void test_three()
{
    strtab * tab = strtab_new(4);
    
    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "two") == 2);
    assert(strtab_add_string(tab, "three") == 3);
    assert(strtab_add_string(tab, "four") == 4);
    assert(strtab_add_string(tab, "five") == 5);
    assert(strtab_add_string(tab, "six") == 6);

    strtab_delete(tab);
}

void test_four()
{
    strtab * tab = strtab_new(4);

    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "two") == 2);
    assert(strtab_add_string(tab, "three") == 3);
    assert(strtab_add_string(tab, "three") == 3);
    assert(strtab_add_string(tab, "two") == 2);
    assert(strtab_add_string(tab, "four") == 4);
    assert(strtab_add_string(tab, "five") == 5);

    strtab_delete(tab);
}

void test_five()
{
    strtab * tab = strtab_new(4);

    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "two") == 2);
    assert(strtab_add_string(tab, "three") == 3);
    assert(strtab_add_string(tab, "three") == 3);
    assert(strtab_add_string(tab, "two") == 2);
    assert(strtab_add_string(tab, "four") == 4);

    assert(strtab_lookup_string(tab, "one") == 1);
    assert(strtab_lookup_string(tab, "two") == 2);
    assert(strtab_lookup_string(tab, "three") == 3);
    assert(strtab_lookup_string(tab, "four") == 4);
    assert(strtab_lookup_string(tab, "five") == 0);

    strtab_delete(tab);
}

void test_six()
{
    strtab * tab = strtab_new(4);

    assert(strtab_add_string(tab, "one") == 1);
    assert(strtab_add_string(tab, "two") == 2);
    assert(strtab_add_string(tab, "three") == 3);
    assert(strtab_add_string(tab, "three") == 3);
    assert(strtab_add_string(tab, "two") == 2);
    assert(strtab_add_string(tab, "four") == 4);

    assert(strtab_lookup_string(tab, "one") == 1);
    assert(strtab_lookup_string(tab, "two") == 2);
    assert(strtab_lookup_string(tab, "three") == 3);
    assert(strtab_lookup_string(tab, "four") == 4);
    assert(strtab_lookup_string(tab, "five") == 0);

    assert(strtab_add_string(tab, "five") == 5);
    assert(strtab_add_string(tab, "five") == 5);
    assert(strtab_lookup_string(tab, "five") == 5);

    assert(strtab_add_string(tab, "four") == 4);
    assert(strtab_add_string(tab, "one") == 1);

    strtab_delete(tab);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    test_four();
    test_five();
    test_six();

    return 0;
}


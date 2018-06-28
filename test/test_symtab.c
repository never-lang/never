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
#include "symtab.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_one()
{
    symtab * tab = symtab_new(32, NULL);

    symtab_delete(tab);
}

void test_two()
{
    symtab_entry * entry;
    symtab * tab = symtab_new(32, NULL);
    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);

    symtab_add_func(tab, func_one);
    symtab_add_func(tab, func_three);

    entry = symtab_lookup(tab, "func_one", SYMTAB_FLAT);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab, "func_two", SYMTAB_FLAT);
    assert(entry == NULL);

    entry = symtab_lookup(tab, "func_three", SYMTAB_FLAT);
    assert(entry->func_value == func_three);

    func_delete(func_one);
    func_delete(func_three);
    symtab_delete(tab);
}

void test_three()
{
    symtab_entry * entry;
    symtab * tab = symtab_new(4, NULL);
    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_two = func_new(strdup("func_two"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
    func * func_four = func_new(strdup("func_four"), NULL, NULL, NULL);

    symtab_add_func(tab, func_one);
    symtab_add_func(tab, func_two);
    symtab_add_func(tab, func_three);
    symtab_add_func(tab, func_four);

    entry = symtab_lookup(tab, "func_one", SYMTAB_FLAT);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab, "func_two", SYMTAB_FLAT);
    assert(entry->func_value == func_two);

    entry = symtab_lookup(tab, "func_three", SYMTAB_FLAT);
    assert(entry->func_value == func_three);

    func_delete(func_one);
    func_delete(func_two);
    func_delete(func_three);
    func_delete(func_four);

    symtab_delete(tab);
}

void test_four()
{
    symtab_entry * entry;
    symtab * tab_one = symtab_new(32, NULL);
    symtab * tab_two = symtab_new(32, tab_one);

    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_two = func_new(strdup("func_two"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
    func * func_four = func_new(strdup("func_four"), NULL, NULL, NULL);
    func * func_five = func_new(strdup("func_five"), NULL, NULL, NULL);
    func * func_six = func_new(strdup("func_six"), NULL, NULL, NULL);

    symtab_add_func(tab_one, func_one);
    symtab_add_func(tab_one, func_two);
    symtab_add_func(tab_two, func_three);
    symtab_add_func(tab_two, func_four);
    symtab_add_func(tab_two, func_five);
    symtab_add_func(tab_two, func_six);

    entry = symtab_lookup(tab_two, "func_one", SYMTAB_NESTED);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab_two, "func_two", SYMTAB_NESTED);
    assert(entry->func_value == func_two);

    entry = symtab_lookup(tab_two, "func_three", SYMTAB_NESTED);
    assert(entry->func_value == func_three);

    entry = symtab_lookup(tab_two, "func_four", SYMTAB_NESTED);
    assert(entry->func_value == func_four);

    entry = symtab_lookup(tab_two, "func_five", SYMTAB_NESTED);
    assert(entry->func_value == func_five);

    entry = symtab_lookup(tab_two, "func_six", SYMTAB_NESTED);
    assert(entry->func_value == func_six);

    entry = symtab_lookup(tab_two, "func_seven", SYMTAB_NESTED);
    assert(entry == NULL);

    func_delete(func_one);
    func_delete(func_two);
    func_delete(func_three);
    func_delete(func_four);
    func_delete(func_five);
    func_delete(func_six);

    symtab_delete(tab_one);
    symtab_delete(tab_two);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    test_four();

    return 0;
}

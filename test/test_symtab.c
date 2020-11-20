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
    symtab * tab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);

    symtab_delete(tab);
}

void test_two()
{
    symtab_entry * entry = NULL;
    symtab * tab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);
    func_decl * decl_one = func_decl_new(strdup("func_one"), NULL, NULL);
    func * func_one = func_new(decl_one, NULL);
    func_decl * decl_three = func_decl_new(strdup("func_three"), NULL, NULL);
    func * func_three = func_new(decl_three, NULL);

    symtab_add_func(tab, func_one, 0);
    symtab_add_func(tab, func_three, 0);

    entry = symtab_lookup(tab, "func_one", SYMTAB_LOOKUP_FUNC);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab, "func_two", SYMTAB_LOOKUP_FUNC);
    assert(entry == NULL);

    entry = symtab_lookup(tab, "func_three", SYMTAB_LOOKUP_FUNC);
    assert(entry->func_value == func_three);

    func_delete(func_one);
    func_delete(func_three);
    symtab_delete(tab);
}

void test_three()
{
    symtab_entry * entry = NULL;
    symtab * tab = symtab_new(4, SYMTAB_TYPE_FUNC, NULL);
    func_decl * decl_one = func_decl_new(strdup("func_one"), NULL, NULL); 
    func * func_one = func_new(decl_one, NULL);
    func_decl * decl_two = func_decl_new(strdup("func_two"), NULL, NULL);
    func * func_two = func_new(decl_two, NULL);
    func_decl * decl_three = func_decl_new(strdup("func_three"), NULL, NULL);
    func * func_three = func_new(decl_three, NULL);
    func_decl * decl_four = func_decl_new(strdup("func_four"), NULL, NULL);
    func * func_four = func_new(decl_four, NULL);
    func_decl * decl_five = func_decl_new(strdup("func_five"), NULL, NULL);
    func * func_five = func_new(decl_five, NULL);
    func_decl * decl_six = func_decl_new(strdup("func_six"), NULL, NULL);
    func * func_six = func_new(decl_six, NULL);
    func_decl * decl_seven = func_decl_new(strdup("func_seven"), NULL, NULL);
    func * func_seven = func_new(decl_seven, NULL);

    symtab_add_func(tab, func_one, 0);
    symtab_add_func(tab, func_two, 0);
    symtab_add_func(tab, func_three, 0);
    symtab_add_func(tab, func_four, 0);
    symtab_add_func(tab, func_five, 0);
    symtab_add_func(tab, func_six, 0);
    symtab_add_func(tab, func_seven, 0);

    entry = symtab_lookup(tab, "func_one", SYMTAB_LOOKUP_FUNC);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab, "func_two", SYMTAB_LOOKUP_FUNC);
    assert(entry->func_value == func_two);

    entry = symtab_lookup(tab, "func_three", SYMTAB_LOOKUP_FUNC);
    assert(entry->func_value == func_three);

    func_delete(func_one);
    func_delete(func_two);
    func_delete(func_three);
    func_delete(func_four);
    func_delete(func_five);
    func_delete(func_six);
    func_delete(func_seven);

    symtab_delete(tab);
}

void test_four()
{
    symtab_entry * entry = NULL;
    symtab * tab_one = symtab_new(2, SYMTAB_TYPE_FUNC, NULL);
    symtab * tab_two = symtab_new(2, SYMTAB_TYPE_FUNC, tab_one);

    func_decl * decl_one = func_decl_new(strdup("func_one"), NULL, NULL);
    func * func_one = func_new(decl_one, NULL);
    func_decl * decl_two = func_decl_new(strdup("func_two"), NULL, NULL);
    func * func_two = func_new(decl_two, NULL);
    func_decl * decl_three = func_decl_new(strdup("func_three"), NULL, NULL);
    func * func_three = func_new(decl_three, NULL);
    func_decl * decl_four = func_decl_new(strdup("func_four"), NULL, NULL);
    func * func_four = func_new(decl_four, NULL);
    func_decl * decl_five = func_decl_new(strdup("func_five"), NULL, NULL);
    func * func_five = func_new(decl_five, NULL);
    func_decl * decl_six = func_decl_new(strdup("func_six"), NULL, NULL);
    func * func_six = func_new(decl_six, NULL);

    symtab_add_func(tab_one, func_one, 0);
    symtab_add_func(tab_one, func_two, 0);
    symtab_add_func(tab_two, func_three, 0);
    symtab_add_func(tab_two, func_four, 0);
    symtab_add_func(tab_two, func_five, 0);
    symtab_add_func(tab_two, func_six, 0);

    entry = symtab_lookup(tab_two, "func_one", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab_two, "func_two", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_two);

    entry = symtab_lookup(tab_two, "func_three", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_three);

    entry = symtab_lookup(tab_two, "func_four", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_four);

    entry = symtab_lookup(tab_two, "func_five", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_five);

    entry = symtab_lookup(tab_two, "func_six", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_six);

    entry = symtab_lookup(tab_two, "func_seven", SYMTAB_LOOKUP_GLOBAL);
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

void test_five()
{
    symtab_entry * entry = NULL;
    symtab * tab_one = symtab_new(2, SYMTAB_TYPE_FUNC, NULL);

    func_decl * decl_one = func_decl_new(strdup("func_one"), NULL, NULL);
    func * func_one = func_new(decl_one, NULL);
    func_decl * decl_two = func_decl_new(strdup("func_two"), NULL, NULL);
    func * func_two = func_new(decl_two, NULL);

    /* add */
    symtab_add_func(tab_one, func_one, 0);
    symtab_add_func(tab_one, func_two, 0);

    entry = symtab_lookup(tab_one, "func_one", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab_one, "func_two", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_two);

    /* remove */
    symtab_remove_func(tab_one, func_one, 0);
    symtab_remove_func(tab_one, func_two, 0);

    entry = symtab_lookup(tab_one, "func_one", SYMTAB_LOOKUP_GLOBAL);
    assert(entry == NULL);

    entry = symtab_lookup(tab_one, "func_two", SYMTAB_LOOKUP_GLOBAL);
    assert(entry == NULL);

    /* add again */
    symtab_add_func(tab_one, func_one, 0);
    symtab_add_func(tab_one, func_two, 0);

    entry = symtab_lookup(tab_one, "func_one", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_one);

    entry = symtab_lookup(tab_one, "func_two", SYMTAB_LOOKUP_GLOBAL);
    assert(entry->func_value == func_two);

    symtab_delete(tab_one);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    test_four();
    test_five();

    return 0;
}

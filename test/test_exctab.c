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
#include <stdlib.h>
#include <assert.h>

void test_zero()
{
    int tab_len = 1;
    exctab_entry * res = NULL;
    exctab_entry tab[] = { { 10, 11 },
                           { 10000, 10000 } };

    res = exctab_search(tab, tab_len, 0);
    assert(res == NULL);

    res = exctab_search(tab, tab_len, 10);
    assert(res != NULL);
    assert(res->handler_addr == 11);

    res = exctab_search(tab, tab_len, 15);
    assert(res != NULL);
    assert(res->handler_addr == 11);

    res = exctab_search(tab, tab_len, 20);
    assert(res != NULL);
    assert(res->handler_addr == 11);

    res = exctab_search(tab, tab_len, 25);
    assert(res != NULL);
    assert(res->handler_addr == 11);
}

void test_one()
{
    int tab_len = 5;
    exctab_entry * res = NULL;
    exctab_entry tab[] = { { 0, 11 },
                           { 20, 41 },
                           { 50, 101 },
                           { 51, 201 },
                           { 90, 301 },
                           { 1000, 1000 } };
                     
    res = exctab_search(tab, tab_len, 0);
    assert(res != NULL);
    assert(res->handler_addr == 11);

    res = exctab_search(tab, tab_len, 10);
    assert(res != NULL);
    assert(res->handler_addr == 11);

    res = exctab_search(tab, tab_len, 15);
    assert(res != NULL);
    assert(res->handler_addr == 11);

    res = exctab_search(tab, tab_len, 20);
    assert(res != NULL);
    assert(res->handler_addr == 41);

    res = exctab_search(tab, tab_len, 35);
    assert(res != NULL);
    assert(res->handler_addr == 41);

    res = exctab_search(tab, tab_len, 45);
    assert(res != NULL);
    assert(res->handler_addr == 41);

    res = exctab_search(tab, tab_len, 50);
    assert(res != NULL);
    assert(res->handler_addr == 101);

    res = exctab_search(tab, tab_len, 51);
    assert(res != NULL);
    assert(res->handler_addr == 201);

    res = exctab_search(tab, tab_len, 85);
    assert(res != NULL);
    assert(res->handler_addr == 201);

    res = exctab_search(tab, tab_len, 100);
    assert(res != NULL);
    assert(res->handler_addr == 301);
}

void test_three()
{
    exctab * tab = exception_tab_new(2);
    exception_tab_delete(tab);
}

void test_four()
{
    exctab * tab = exception_tab_new(2);

    exception_tab_insert(tab, 0, 11);
    exception_tab_insert(tab, 20, 41);
    exception_tab_insert(tab, 50, 101);
    exception_tab_insert(tab, 51, 201);
    exception_tab_insert(tab, 90, 301);

    assert(exception_tab_search(tab, 0) == 11);
    assert(exception_tab_search(tab, 10) == 11);
    assert(exception_tab_search(tab, 15) == 11);
    assert(exception_tab_search(tab, 20) == 41);
    assert(exception_tab_search(tab, 35) == 41);
    assert(exception_tab_search(tab, 45) == 41);
    assert(exception_tab_search(tab, 50) == 101);
    assert(exception_tab_search(tab, 51) == 201);
    assert(exception_tab_search(tab, 85) == 201);
    assert(exception_tab_search(tab, 100) == 301);

    exception_tab_delete(tab);
}

int main(int argc, char * argv[])
{
    test_zero();
    test_one();
    test_three();
    test_four();

    return 0;
}


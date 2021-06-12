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
#include "dlcache.h"
#include <stdlib.h>
#include <assert.h>

void test_one()
{
    dlcache * cache = dlcache_new(4);
    
    dlcache_delete(cache);
}

void test_two()
{
    dlcache * cache = dlcache_new(2);
    
    dlcache_add_dl(cache, "libone.so", NULL);
    dlcache_add_dl(cache, "libtwo.so", NULL);
    dlcache_add_dl(cache, "libthree.so", NULL);
    dlcache_add_dl(cache, "libfour.so", NULL);
    dlcache_add_dl(cache, "libfive.so", NULL);
    dlcache_add_dl(cache, "libsix.so", NULL);
    dlcache_add_dl(cache, "libseven.so", NULL);
    dlcache_add_dl(cache, "libeight.so", NULL);

    assert(cache->count == 9);
    
    assert(dlcache_lookup(cache, "host") != NULL);
    assert(dlcache_lookup(cache, "libone.so") != NULL);
    assert(dlcache_lookup(cache, "libtwo.so") != NULL);
    assert(dlcache_lookup(cache, "libthree.so") != NULL);
    assert(dlcache_lookup(cache, "libfour.so") != NULL);
    assert(dlcache_lookup(cache, "libfive.so") != NULL);
    assert(dlcache_lookup(cache, "libsix.so") != NULL);
    assert(dlcache_lookup(cache, "libseven.so") != NULL);
    assert(dlcache_lookup(cache, "libeight.so") != NULL);

    assert(dlcache_lookup(cache, "libnine.so") == NULL);
    assert(dlcache_lookup(cache, "libten.so") == NULL);

    dlcache_add_dl(cache, "libnine.so", NULL);
    dlcache_add_dl(cache, "libten.so", NULL);

    assert(cache->count == 11);

    assert(dlcache_lookup(cache, "host") != NULL);
    assert(dlcache_lookup(cache, "libone.so") != NULL);
    assert(dlcache_lookup(cache, "libtwo.so") != NULL);
    assert(dlcache_lookup(cache, "libnine.so") != NULL);
    assert(dlcache_lookup(cache, "libten.so") != NULL);
    
    dlcache_delete(cache);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();

    return 0;
}


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


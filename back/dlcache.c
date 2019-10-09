#include "dlcache.h"
#include "fficall.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <dlfcn.h>

dlcache_entry * dlcache_entry_new(unsigned int size)
{
    dlcache_entry * entry = (dlcache_entry *)malloc(sizeof(dlcache_entry) * size);
    memset(entry, 0, sizeof(dlcache_entry) * size);
    
    return entry;
}

void dlcache_entry_delete(dlcache_entry * entries)
{
    free(entries);
}

void dlcache_entry_add_dl(
    dlcache_entry * entries, 
    unsigned int size,
    const char * dl_name,
    void * handle)
{
    unsigned int times = 0;
    unsigned int index = 0;
    
    index = hash_string(dl_name) % size;
    while (entries[index].dl_name != NULL)
    {
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
            return;
        }
    }
    entries[index].dl_name = dl_name;
    entries[index].handle = handle;
}
    
dlcache_entry * dlcache_entry_lookup(
    dlcache_entry * entries,
    unsigned int size,
    const char * dl_name)
{
    unsigned int times = 0;
    unsigned int index = 0;
    
    index = hash_string(dl_name) % size;
    while (entries[index].dl_name != NULL)
    {
        if (strcmp(entries[index].dl_name, dl_name) == 0)
        {
            return &entries[index];
        }
    
        index = (index + 1) % size;
        if (times++ > size)
        {
            return NULL;
        }
    }
    
    return NULL;
}

void dlcache_entry_resize(dlcache_entry * entries, unsigned int size,
                          dlcache_entry * entries_new, unsigned int size_new)
{
    unsigned int i = 0;
    
    for (i = 0; i < size; i++)
    {
        if (entries[i].dl_name != NULL)
        {
            dlcache_entry_add_dl(entries_new, size_new, entries[i].dl_name,
                                 entries[i].handle);
        }
    }
}

void dlcache_entry_print(dlcache_entry * entry)
{
    if (entry != NULL)
    {
        printf("%s", entry->dl_name);
    }
}

dlcache * dlcache_new(unsigned int size)
{
    dlcache * cache = (dlcache *)malloc(sizeof(dlcache));
    dlcache_entry * entries = dlcache_entry_new(size);
    void * host_handle = NULL;
    
    cache->size = size;
    cache->count = 0;
    cache->entries = entries;

    host_handle = ffi_decl_get_handle("host");
    dlcache_add_dl(cache, "host", host_handle);
    
    return cache;
}

void dlcache_delete(dlcache * cache)
{
    if (cache->entries != NULL)
    {
        unsigned int i = 0;
        
        for (i = 0; i < cache->size; i++)
        {
            if (cache->entries[i].handle != NULL)
            {
                dlclose(cache->entries[i].handle);
            }
        }
    
        dlcache_entry_delete(cache->entries);
    }
    free(cache);
}

void dlcache_add_dl(dlcache * cache, const char * dl_name, void * handle)
{
    if (dl_name == NULL)
    {
        return;
    }
    
    dlcache_entry_add_dl(cache->entries, cache->size, dl_name, handle);

    cache->count++;
    dlcache_resize(cache);
}

dlcache_entry * dlcache_lookup(dlcache * cache, const char * dl_name)
{
    dlcache_entry * entry = NULL;

    entry = dlcache_entry_lookup(cache->entries, cache->size, dl_name);

    return entry;
}

void * dlcache_get_handle(dlcache * cache, const char * dl_name)
{
    void * handle = NULL;

    dlcache_entry * entry = dlcache_lookup(cache, dl_name);
    if (entry != NULL)
    {
        handle = entry->handle;
    }
    else
    {
        handle = ffi_decl_get_handle(dl_name);
        if (handle == NULL)
        {
            fprintf(stderr, "cannot open library %s\n", dl_name);
            return NULL;
        }
        dlcache_add_dl(cache, dl_name, handle);
    }
    
    return handle;
}

void dlcache_resize(dlcache * cache)
{
    if (cache->count > cache->size * 3 / 4)
    {
        unsigned int size_new = cache->size * 2;
        dlcache_entry * entries_new = dlcache_entry_new(size_new);
        
        dlcache_entry_resize(cache->entries, cache->size, entries_new, size_new);
        dlcache_entry_delete(cache->entries);
        
        cache->size = size_new;
        cache->entries = entries_new;
    }
}

void dlcache_print(dlcache * cache)
{
    unsigned int i = 0;
    
    for (i = 0 ; i < cache->size; i++)
    {
        dlcache_entry_print(&cache->entries[i]);
    }
}




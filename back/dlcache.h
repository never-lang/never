#ifndef __DLCACHE_H__
#define __DLCACHE_H__

typedef struct dlcache_entry
{
    const char * dl_name;
    void * handle;
} dlcache_entry;

typedef struct dlcache
{
    unsigned int size;
    unsigned int count;
    dlcache_entry * entries;
} dlcache;

dlcache_entry * dlcache_entry_new(unsigned int size);
void dlcache_entry_delete(dlcache_entry * entries);

void dlcache_entry_add_dl(dlcache_entry * entries, unsigned int size,
                          const char * dl_name, void * handle);
dlcache_entry * dlcache_entry_lookup(dlcache_entry * entries, unsigned int size,
                                     const char * dl_name);
void dlcache_entry_resize(dlcache_entry * entries, unsigned int size,
                          dlcache_entry * entries_new, unsigned int size_new);

void dlcache_entry_print(dlcache_entry * entry);

dlcache * dlcache_new(unsigned int size);
void dlcache_delete(dlcache * cache);

void dlcache_open_dl(dlcache * cache, const char * dl_name);
dlcache_entry * dlcache_lookup(dlcache * cache, const char * dl_name);

void dlcache_resize(dlcache * cache);

void dlcache_print(dlcache * cache);

#endif /* __DLCACHE_H__ */



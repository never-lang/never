#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "symtab.h"

#define HASH_MAGIC 5381

unsigned int hash(const char * str)
{
    char c;
    unsigned int val = HASH_MAGIC;

    while ((c = *str++) != 0)
    {
        val = ((val << 5) + val) + c;
    }

    return val;
}

symtab_entry * symtab_entry_new(unsigned int size)
{
    symtab_entry * entries = (symtab_entry *)malloc(size * sizeof(symtab_entry));
    memset(entries, 0, size * sizeof(symtab_entry));
    
    return entries;
}

void symtab_entry_delete(symtab_entry * entries)
{
    free(entries);
}

void symtab_entry_add_arg_func(symtab_entry * entries, unsigned int size,
                               int type, const char * id, void * arg_func_value)
{
    unsigned int times = 0;
    unsigned int index = hash(id) % size;
    
    while (entries[index].type != 0)
    {
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
            return;
        }
    }
    entries[index].type = type;
    entries[index].id = id;
    entries[index].arg_func_value = arg_func_value;
}

symtab_entry * symtab_entry_lookup_arg_func(symtab_entry * entries, unsigned int size, 
                                            int type, const char * id)
{
    unsigned int times = 0;
    unsigned int index = hash(id) % size;

    while (entries[index].type != 0)
    {
        if (strcmp(entries[index].id, id) == 0)
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

void symtab_entry_resize(symtab_entry * entries, int size, symtab_entry * entries_new, int size_new)
{
    int i = 0;
    
    for (i = 0; i < size; i++)
    {
        symtab_entry_add_arg_func(entries_new, size_new,
                                  entries[i].type, entries[i].id, entries[i].arg_func_value);
    }
}

symtab * symtab_new(unsigned int size, symtab * parent)
{
    symtab * tab = (symtab *)malloc(sizeof(symtab));
    
    tab->size = size;
    tab->count = 0;
    tab->parent = parent;
    tab->entries = symtab_entry_new(size);
    
    return tab;
}

void symtab_delete(symtab * tab)
{
    if (tab->entries)
    {
        symtab_entry_delete(tab->entries);
    }
    free(tab);    
}

void symtab_resize(symtab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        symtab_entry * entries_new = symtab_entry_new(size_new);
        
        symtab_entry_resize(tab->entries, tab->size, entries_new, size_new);
        
        symtab_entry_delete(tab->entries);
        
        tab->size = size_new;
        tab->entries = entries_new;
    }
}

void symtab_add_arg(symtab * tab, arg * arg_value)
{
    if (arg_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_arg_func(tab->entries, tab->size, SYMTAB_ARG, arg_value->id, arg_value);
    tab->count++;
    symtab_resize(tab);    
}

void symtab_add_func(symtab * tab, func * func_value)
{
    if (func_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_arg_func(tab->entries, tab->size, SYMTAB_FUNC, func_value->id, func_value);
    tab->count++;
    symtab_resize(tab);
}

symtab_entry * symtab_lookup_type(symtab * tab, int type, const char * id)
{
    symtab_entry * entry = symtab_entry_lookup_arg_func(tab->entries, tab->size, type, id);
    if (entry == NULL && tab->parent != NULL)
    {
        return symtab_lookup_type(tab->parent, type, id);
    }
    else
    {
        return entry;
    }
}

symtab_entry * symtab_lookup_arg(symtab * tab, const char * id)
{
    return symtab_lookup_type(tab, SYMTAB_ARG, id);
}

symtab_entry * symtab_lookup_func(symtab * tab, const char * id)
{
    return symtab_lookup_type(tab, SYMTAB_FUNC, id);
}

void symtab_print(symtab * tab)
{
    int i;
    
    printf("[%p, %p]\n", tab, tab->parent);
        
    for (i = 0; i < tab->size; i++)
    {
        if (tab->entries[i].type != 0)
        {
            printf("[%d][%s]\n", tab->entries[i].type, tab->entries[i].id);
        }
    }
}



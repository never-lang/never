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
#include "hash.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

symtab_entry * symtab_entry_new(unsigned int size)
{
    symtab_entry * entries =
        (symtab_entry *)malloc(size * sizeof(symtab_entry));
    memset(entries, 0, size * sizeof(symtab_entry));

    return entries;
}

void symtab_entry_delete(symtab_entry * entries) { free(entries); }

void symtab_entry_add_object(symtab_entry * entries, unsigned int size,
                             int type, const char * id, void * object_value,
                             unsigned int syn_level)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(id) % size;
    while (entries[index].type != 0 &&
           entries[index].type != SYMTAB_REMOVED)
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
    entries[index].object_value = object_value;
    entries[index].syn_level = syn_level;
}

void symtab_entry_remove_object(symtab_entry * entries, unsigned int size,
                                int type, const char * id, void * object_value,
                                unsigned int syn_level)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(id) % size;
    while (entries[index].type != 0)
    {
        if (entries[index].type != SYMTAB_REMOVED &&
            strcmp(entries[index].id, id) == 0)
        {
            entries[index].type = SYMTAB_REMOVED;
            return;
        }

        index = (index + 1) % size;
        if (times++ > size)
        {
            return;
        }
    }
}

symtab_entry * symtab_entry_lookup_object(symtab_entry * entries,
                                          unsigned int size, const char * id)
{
    unsigned int times = 0;
    unsigned int index = 0;
    
    index = hash_string(id) % size;
    while (entries[index].type != 0)
    {
        if (entries[index].type != SYMTAB_REMOVED &&
            strcmp(entries[index].id, id) == 0)
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

void symtab_entry_resize(symtab_entry * entries, int size,
                         symtab_entry * entries_new, int size_new)
{
    int i = 0;

    for (i = 0; i < size; i++)
    {
        if (entries[i].id != NULL &&
            entries[i].type != SYMTAB_REMOVED)
        {
            symtab_entry_add_object(entries_new, size_new, entries[i].type,
                                    entries[i].id, entries[i].object_value,
                                    entries[i].syn_level);
        }
    }
}

void symtab_entry_print(symtab_entry * entry)
{
    if (entry->type == SYMTAB_PARAM)
    {
        param * param_value = entry->param_value;
        if (param_value)
        {
            if (param_value->type == PARAM_INT)
            {
                printf("[PI][%s][%d]\n", param_value->id, entry->syn_level);
            }
            else if (param_value->type == PARAM_FLOAT)
            {
                printf("[PI][%s][%d]\n", param_value->id, entry->syn_level);
            }
            else if (param_value->type == PARAM_FUNC)
            {
                printf("[PF][%s][%d]\n", param_value->id, entry->syn_level);
            }
            else if (param_value->type == PARAM_RECORD)
            {
                printf("[PR][%s][%d]\n", param_value->id, entry->syn_level);
            }
            else if (param_value->type == PARAM_ENUMTYPE)
            {
                printf("[PE][%s][%d]\n", param_value->id, entry->syn_level);
            }
        }
        else
        {
            printf("[PU][%s][%d]\n", entry->id, entry->syn_level);
        }
    }
    else if (entry->type == SYMTAB_BIND)
    {
        printf("[B][%s][%d]\n", entry->id, entry->syn_level);
    }
    else if (entry->type == SYMTAB_MATCHBIND)
    {
        printf("[M][%s][%d]\n", entry->id, entry->syn_level);
    }
    else if (entry->type == SYMTAB_QUALIFIER)
    {
        printf("[Q][%s][%d]\n", entry->id, entry->syn_level);
    }
    else if (entry->type == SYMTAB_RECORD)
    {
        printf("[R][%s][%d]\n", entry->id, entry->syn_level);
    }
    else if (entry->type == SYMTAB_ENUMERATOR)
    {
        printf("[I][%s][%d]\n", entry->id, entry->syn_level);
    }
    else if (entry->type == SYMTAB_ENUMTYPE)
    {
        printf("[E][%s][%d]\n", entry->id, entry->syn_level);
    }
    else if (entry->type == SYMTAB_FUNC)
    {
        printf("[F][%s][%d]\n", entry->id, entry->syn_level);
    }
    else
    {
        assert(0);
    }
}

char * symtab_entry_type_str(symtab_entry_type type)
{
    switch (type)
    {
        case SYMTAB_PARAM: return "param";
        case SYMTAB_BIND: return "bind";
        case SYMTAB_MATCHBIND: return "match bind";
        case SYMTAB_QUALIFIER: return "qualifier";
        case SYMTAB_FORIN: return "for in";
        case SYMTAB_ENUMERATOR: return "enumerator";
        case SYMTAB_ENUMTYPE: return "enum";
        case SYMTAB_RECORD: return "record";
        case SYMTAB_FUNC: return "func";
        case SYMTAB_MODULE_DECL: return "module decl";
        case SYMTAB_REMOVED: return "symtab entry removed";
    }
    return "unknown";
}

symtab * symtab_new(unsigned int size, symtab_type type, symtab * parent)
{
    symtab * tab = (symtab *)malloc(sizeof(symtab));
    
    tab->type = type;
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

void symtab_add_param(symtab * tab, param * param_value, unsigned int syn_level)
{
    if (param_value->id == NULL)
    {
        return;
    }
    
    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_PARAM, param_value->id,
                            param_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_bind(symtab * tab, bind * bind_value, unsigned int syn_level)
{
    if (bind_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_BIND, bind_value->id,
                            bind_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_matchbind(symtab * tab, matchbind * matchbind_value, unsigned int syn_level)
{
    if (matchbind_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_MATCHBIND, matchbind_value->id,
                            matchbind_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_qualifier(symtab * tab, qualifier * qualifier_value, unsigned int syn_level)
{
    if (qualifier_value->id == NULL)
    {
        return;
    }
 
    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_QUALIFIER, qualifier_value->id,
                            qualifier_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_forin(symtab * tab, forin * forin_value, unsigned int syn_level)
{
    if (forin_value->iter->id.id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_FORIN, forin_value->iter->id.id,
                            forin_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_enumerator(symtab * tab, enumerator * enumerator_value, unsigned int syn_level)
{
    if (enumerator_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_ENUMERATOR, enumerator_value->id,
                            enumerator_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_enumtype(symtab * tab, enumtype * enumtype_value, unsigned int syn_level)
{
    if (enumtype_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_ENUMTYPE, enumtype_value->id,
                            enumtype_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_record(symtab * tab, record * record_value, unsigned int syn_level)
{
    if (record_value->id == NULL)
    {
        return;
    }
    
    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_RECORD, record_value->id,
                            record_value, syn_level);
                            
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_func(symtab * tab, func * func_value, unsigned int syn_level)
{
    if (func_value->decl->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_FUNC,
                            func_value->decl->id, func_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_add_module_decl(symtab * tab, module_decl * module_decl_value, unsigned int syn_level)
{
    if (module_decl_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_MODULE_DECL,
                            module_decl_value->id, module_decl_value, syn_level);
    tab->count++;
    symtab_resize(tab);
}

void symtab_remove_func(symtab * tab, func * func_value, unsigned int syn_level)
{
    if (func_value->decl->id == NULL)
    {
        return;
    }

    tab->count--;
    symtab_entry_remove_object(tab->entries, tab->size, SYMTAB_FUNC,
                               func_value->decl->id, func_value, syn_level);
}

void symtab_for_all(symtab * tab, void (*symtab_exe) (symtab_entry * entry))
{
    unsigned int i = 0;

    for (i = 0; i < tab->size; i++)
    {
        if (tab->entries[i].type != 0)
        {
            symtab_exe(&tab->entries[i]);
        }
    }
}

#if 0
static void symtab_exe_module_decl_set_active(symtab_entry * entry)
{
    if (entry->type == SYMTAB_MODULE_DECL)
    {
        if (entry->module_decl_value)
        {
            entry->module_decl_value->is_active = 1;
        }
    }
}
#endif

#if 0
static void symtab_exe_module_decl_set_inactive(symtab_entry * entry)
{
    if (entry->type == SYMTAB_MODULE_DECL)
    {
        if (entry->module_decl_value)
        {
            entry->module_decl_value->is_active = 0;
        }
    }
}
#endif

#if 0
void symtab_module_decl_set_active(symtab * tab, char is_active)
{
    if (is_active)
    {
        symtab_for_all(tab, symtab_exe_module_decl_set_active);
    }
    else
    {
        symtab_for_all(tab, symtab_exe_module_decl_set_inactive);        
    }
}
#endif

symtab_entry * symtab_lookup(symtab * tab, const char * id, symtab_lookup_op lookup)
{
    symtab_entry * entry = NULL;

    entry = symtab_entry_lookup_object(tab->entries, tab->size, id);
    if (((lookup == SYMTAB_LOOKUP_GLOBAL) ||
         ((lookup == SYMTAB_LOOKUP_FUNC) && (tab->type == SYMTAB_TYPE_BLOCK))) &&
        (entry == NULL) && 
        (tab->parent != NULL))
    {
        return symtab_lookup(tab->parent, id, lookup);
    }
    else
    {
        return entry;
    }
}

void symtab_print(symtab * tab)
{
    unsigned int i;

    printf("[%p, %p]\n", tab, tab->parent);

    for (i = 0; i < tab->size; i++)
    {
        symtab_entry_print(tab->entries + i);
    }
}

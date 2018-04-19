#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include "arg.h"
#include "func.h"

enum
{
    SYMTAB_FLAT = 0,
    SYMTAB_NESTED = 1
};

enum
{
    SYMTAB_ARG = 1,
    SYMTAB_FUNC = 2
};

typedef struct symtab_entry
{
    int type;
    const char * id;
    void * arg_func_value;
    int attrib;   
} symtab_entry;

typedef struct symtab
{
    unsigned int size;
    unsigned int count;
    struct symtab * parent;
    symtab_entry * entries;
} symtab;

symtab_entry * symtab_entry_new(unsigned int size);
void symtab_entry_delete(symtab_entry * entries);
void symtab_entry_add_arg_func(symtab_entry * entries, unsigned int size,
                               int type, const char * id, void * arg_func_value);
symtab_entry * symtab_entry_lookup_arg_func(symtab_entry * entries, unsigned int size, const char * id);
void symtab_entry_resize(symtab_entry * entries, int size, symtab_entry * entries_new, int size_new);

symtab * symtab_new(unsigned int size, symtab * parent);
void symtab_delete(symtab * tab);

void symtab_add_arg(symtab * tab, arg * arg_value);
void symtab_add_func(symtab * tab, func * func_value);

symtab_entry * symtab_lookup(symtab * tab, const char * id, char nested);

void symtab_resize(symtab * tab);

void symtab_print(symtab * tab);

#endif /* __SYMTAB_H__ */




#ifndef __FREEVAR_H__
#define __FREEVAR_H__

#include "var.h"

enum
{
    FREEVAR_UNKNOWN = 0,
    FREEVAR_LOCAL = 1,
    FREEVAR_GLOBAL = 2
};

typedef struct freevar
{
    int type;
    char * id;
    int index;
    union
    {
        struct var * local_value;
        struct freevar * global_value;
    };
} freevar;

typedef struct freevar_list_node
{
    freevar * value;
    struct freevar_list_node * prev;
    struct freevar_list_node * next;
} freevar_list_node;

typedef struct freevar_list
{
    int count;
    freevar_list_node * head;
    freevar_list_node * tail;
} freevar_list;

freevar * freevar_new(char * id, int index);
void freevar_delete(freevar * value);

freevar_list_node * freevar_list_node_new(freevar * value);
void freevar_list_node_delete(freevar_list_node * node);

freevar_list * freevar_list_new();
void freevar_list_delete(freevar_list * list);

freevar * freevar_list_add(freevar_list * list, char * id);

void freevar_list_add_beg(freevar_list * list, freevar * value);
void freevar_list_add_end(freevar_list * list, freevar * value);

void freevar_print(freevar * value);
void freevar_list_print(freevar_list * list);

char * freevar_type_str(int type);

#endif /* __FREEVAR_H__ */



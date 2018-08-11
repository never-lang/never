#ifndef __BIND_H__
#define __BIND_H__

#include "expr.h"

typedef enum bind_type
{
    BIND_UNKNOWN = 0,
    BIND_LET = 1,
    BIND_VAR = 2
} bind_type;

typedef struct bind
{
    bind_type type;
    char * id;
    expr * expr_value;
    int index;
    unsigned int line_no;
} bind;

typedef struct bind_list_node
{
    bind * value;
    struct bind_list_node * prev;
    struct bind_list_node * next;
} bind_list_node;

typedef struct bind_list
{
    unsigned int count;
    bind_list_node * head;
    bind_list_node * tail;
} bind_list;

bind * bind_new_let(char * id, expr * expr_value);
bind * bind_new_var(char * id, expr * expr_value);

void bind_delete(bind * value);

bind_list_node * bind_list_node_new(bind * value);
void bind_list_node_delete(bind_list_node * node);

bind_list * bind_list_new();
void bind_list_delete(bind_list * list);

void bind_list_add_beg(bind_list * list, bind * value);
void bind_list_add_end(bind_list * list, bind * value);

#endif /* __BIND_H__ */


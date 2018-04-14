#ifndef __ARG_H__
#define __ARG_H__

enum
{
    ARG_INT = 1,
    ARG_FUNC = 2
};

typedef struct arg
{
    int type;
    char * id;
    struct arg_list * args;
    struct arg * ret;
} arg;

typedef struct arg_list_node
{
    arg * value;
    struct arg_list_node * prev;
    struct arg_list_node * next;
} arg_list_node;

typedef struct arg_list
{
    arg_list_node * head;
    arg_list_node * tail;
} arg_list;

arg * arg_new_int(char * id);
arg * arg_new_func(char * id, arg_list * args, arg * ret);
void arg_delete(arg * value);

arg_list_node * arg_list_node_new(arg * value);
void arg_list_node_delete(arg_list_node * node);

arg_list * arg_list_new();
void arg_list_delete(arg_list * list);

void arg_list_add_beg(arg_list * list, arg * value);
void arg_list_add_end(arg_list * list, arg * value);

#endif /* __ARG_H__ */


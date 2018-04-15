#ifndef __FUNC_H__
#define __FUNC_H__

#include "expr.h"
#include "arg.h"
#include "symtab.h"

struct func_body;
struct func_list;

typedef struct func
{
    char * id;
    arg_list * args;
    arg * ret;
    struct func_body * body;
    struct symtab * stab;
} func;

typedef struct func_body
{
    struct func_list * funcs;
    struct expr * ret;   
} func_body;

typedef struct func_list_node
{
    func * value;
    struct func_list_node * prev;
    struct func_list_node * next;
} func_list_node;

typedef struct func_list
{
    func_list_node * head;
    func_list_node * tail;
} func_list;

func * func_new(char * id, arg_list * args, arg * ret, func_body * body);
void func_delete(func * value);

func_body * func_body_new(func_list * funcs, expr * ret);
void func_body_delete(func_body * body);

func_list_node * func_list_node_new(func * value);
void func_list_node_delete(func_list_node * node);

func_list * func_list_new();
void func_list_delete(func_list * list);

void func_list_add_beg(func_list * list, func * value);
void func_list_add_end(func_list * list, func * value);

#endif /* __FUNC_H__ */


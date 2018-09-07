#ifndef __WEAKFUNC_H__
#define __WEAKFUNC_H__

#include "func.h"

typedef struct func_list_weak_node
{
    func * value;
    struct func_list_weak_node * next;
    struct func_list_weak_node * prev;
} func_list_weak_node;

typedef struct func_list_weak
{
    int count;
    struct func_list_weak_node * tail;
    struct func_list_weak_node * head;
} func_list_weak;

func_list_weak_node * func_list_weak_node_new(func * value);
void func_list_weak_node_delete(func_list_weak_node * node);

func_list_weak * func_list_weak_new();
void func_list_weak_delete(func_list_weak * list);

void func_list_weak_add(func_list_weak * list, func * value);
func * func_list_weak_pop(func_list_weak * list);

#endif /* __WEAKFUNC_H__ */

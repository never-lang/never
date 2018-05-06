#include <stdlib.h>
#include <stdio.h>
#include "bytecode.h"

bytecode * bytecode_new()
{
    bytecode * value = (bytecode *)malloc(sizeof(bytecode));
    
    value->type = 0;
    
    return value;
}

void bytecode_delete(bytecode * value)
{
    free(value);
}

bytecode_node * bytecode_node_new(bytecode * value)
{
    bytecode_node * node = (bytecode_node *)malloc(sizeof(bytecode_node));
    
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void bytecode_node_delete(bytecode_node * node)
{
    bytecode_delete(node->value);
    free(node);
}

bytecode_list * bytecode_list_new()
{
    bytecode_list * list = (bytecode_list *)malloc(sizeof(bytecode_list));
    
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void bytecode_list_delete(bytecode_list * list)
{
    bytecode_node * node = list->tail;
    while (node != NULL)
    {
        bytecode_node * tmp = node->next;
        bytecode_node_delete(node);
        node = tmp;
    }
    free(list);
}



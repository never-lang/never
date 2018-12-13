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
#include "enums.h"
#include "enumerator.h"
#include "symtab.h"
#include <stdlib.h>
#include <assert.h>

enumtype * enumtype_new(char * id, enumerator_list * enums)
{
    enumtype * value = (enumtype *)malloc(sizeof(enumtype));
    
    value->id = id;
    value->enums = enums;
    value->stab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);
    
    return value;
}

void enumtype_delete(enumtype * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    if (value->enums != NULL)
    {
        enumerator_list_delete(value->enums);
    }
    if (value->stab != NULL)
    {
        symtab_delete(value->stab);
    }

    free(value);
}

enumerator * enumtype_find_enumerator(enumtype * enumtype_value, char * id)
{
    enumerator * ret = NULL;
    symtab_entry * entry = NULL;

    if (enumtype_value->stab != NULL && id != NULL)
    {    
        entry = symtab_lookup(enumtype_value->stab, id, SYMTAB_LOOKUP_LOCAL);
    }

    if (entry != NULL && entry->type == SYMTAB_ENUMERATOR)
    {
        ret = entry->enumerator_value;
    }
    else if (entry != NULL && entry->type != SYMTAB_ENUMERATOR)
    {
        assert(0);
    }

    return ret;
}

enumtype_list_node * enumtype_list_node_new(enumtype * value)
{
    enumtype_list_node * node =
        (enumtype_list_node *)malloc(sizeof(enumtype_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void enumtype_list_node_delete(enumtype_list_node * node)
{
    if (node->value)
    {
        enumtype_delete(node->value);
    }
    free(node);
}

enumtype_list * enumtype_list_new()
{
    enumtype_list * list = (enumtype_list *)malloc(sizeof(enumtype_list));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void enumtype_list_delete(enumtype_list * list)
{
    enumtype_list_node * node = list->tail;

    while (node != NULL)
    {
        enumtype_list_node * tmp = node->next;
        enumtype_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void enumtype_list_add_beg(enumtype_list * list, enumtype * value)
{
    enumtype_list_node * node = enumtype_list_node_new(value);

    if (list->head == NULL && list->tail == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->prev = node;
        node->next = list->tail;
        list->tail = node;
    }
}

void enumtype_list_add_end(enumtype_list * list, enumtype * value)
{
    enumtype_list_node * node = enumtype_list_node_new(value);

    if (list->head == NULL && list->tail == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->head->next = node;
        node->prev = list->head;
        list->head = node;
    }
}



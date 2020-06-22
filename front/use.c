/** 
 * Copyright 2020 Slawomir Maludzinski
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
#include "use.h"
#include <stdio.h>
#include <stdlib.h>

use * use_new(char * id, module_decl * decl)
{
    use * value = (use *)malloc(sizeof(use));

    value->id = id;
    value->decl = decl;

    return value;
}

void use_delete(use * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    if (value->decl != NULL)
    {
        free(value->decl);
    }
    free(value);
}

void use_print(use * value)
{
    if (value->id != NULL)
    {
        printf("use %s\n", value->id);
    }
}

use_list_node * use_list_node_new(use * value)
{
    use_list_node * node = (use_list_node *)malloc(sizeof(use_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void use_list_node_delete(use_list_node * node)
{
    if (node->value)
    {
        use_delete(node->value);
    }
    free(node);
}

use_list * use_list_new()
{
    use_list * list = (use_list *)malloc(sizeof(use_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void use_list_delete(use_list * list)
{
    use_list_node * node = list->tail;

    while (node != NULL)
    {
        use_list_node * tmp = node->next;
        use_list_node_delete(node);
        node = tmp;
    }
    free(node);
}

void use_list_add_beg(use_list * list, use * value)
{
    use_list_node * node = use_list_node_new(value);

    list->count++;
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

void use_list_add_end(use_list * list, use * value)
{
    use_list_node * node = use_list_node_new(value);

    list->count++;
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

void use_list_print(use_list * list)
{
    use_list_node * node = list->tail;

    while (node != NULL)
    {
        if (node->value != NULL)
        {
            use_print(node->value);
        }
        node = node->next;
    }
}

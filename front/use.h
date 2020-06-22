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
#ifndef __USE_H__
#define __USE_H__

typedef struct module_decl module_decl;

typedef struct use
{
    char * id;
    module_decl * decl;
} use;

typedef struct use_list_node
{
    use * value;
    struct use_list_node * prev;
    struct use_list_node * next;
} use_list_node;

typedef struct use_list
{
    unsigned int count;
    use_list_node * head;
    use_list_node * tail;
} use_list;

use * use_new(char * id, module_decl * decl);
void use_delete(use * value);
void use_print(use * value);

use_list_node * use_list_node_new(use * value);
void use_list_node_delete(use_list_node * node);

use_list * use_list_new();
void use_list_delete(use_list * list);

void use_list_add_beg(use_list * list, use * value);
void use_list_add_end(use_list * list, use * value);

void use_list_print(use_list * list);

#endif /* __USE_H__ */

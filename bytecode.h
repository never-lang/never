#ifndef __BYTECODE_H__
#define __BYTECODE_H__

typedef struct bytecode
{
    int type;
} bytecode;

typedef struct bytecode_node
{
    bytecode * value;
    struct bytecode_node * prev;
    struct bytecode_node * next;
} bytecode_node;

typedef struct bytecode_list
{
    bytecode_node * head;
    bytecode_node * tail;
} bytecode_list;

bytecode * bytecode_new();
void bytecode_delete(bytecode * value);

bytecode_node * bytecode_node_new(bytecode * value);
void bytecode_node_delete(bytecode_node * node);

bytecode_list * bytecode_list_new();
void bytecode_list_delete(bytecode_list * list);

#endif /* __BYTECODE_H__ */


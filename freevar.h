#ifndef __FREEVAR_H__
#define __FREEVAR_H__

typedef struct btree
{
    int value;
    struct btree * left;
    struct btree * right;
} btree;

btree * btree_new(int value);
void btree_destroy(btree ** node);

char btree_search(btree * node, int value);
void btree_insert(btree ** node, int value);
btree * find_next(btree ** node);
void btree_delete(btree ** node, int value);

int btree_max(btree * node);
void btree_print(btree * node);

#endif /* __FREEVAR_H__ */



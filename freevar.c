#include <stdlib.h>
#include "freevar.h"

btree * btree_new(int value)
{
    btree * node = (btree *)malloc(sizeof(btree));
    
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

char btree_search(btree * node, int value)
{
    if (node == NULL)
    {
        return 0;
    }
    if (node->value == value)
    {
        return 1;
    }
    if (value < node->value)
    {
        return btree_search(node->left, value);
    }
    else
    {
        return btree_search(node->right, value);
    }
}

void btree_insert(btree ** node, int value)
{
    if (*node == NULL)
    {
        *node = btree_new(value);
        return;
    }
    if (value < (*node)->value)
    {
        btree_insert(&(*node)->left, value);
    }
    else
    {
        btree_insert(&(*node)->right, value);
    }
}

btree * find_next(btree ** node)
{
    if ((*node)->left != NULL)
    {
        return find_next(&(*node)->left);
    }
    btree * tmp = (*node);
    *node = (*node)->right;
    return tmp;
}

void btree_delete(btree ** node, int value)
{
    if (*node == NULL)
    {
        return;
    }
    if ((*node)->value == value)
    {
        if ((*node)->left == NULL && (*node)->right == NULL)
        {
            free(*node);
            *node = NULL;
        }
        else if ((*node)->right != NULL && (*node)->left == NULL)
        {
            btree * curr = *node;
            *node = (*node)->right;

            free(curr);
        }
        else if ((*node)->left != NULL && (*node)->right == NULL)
        {
            btree * curr = *node;
            *node = (*node)->left;

            free(curr);
        }
        else
        {
            btree * curr = *node;
            btree * next = find_next(&(*node)->right);

            next->left = (*node)->left;
            next->right = (*node)->right;
            *node = next;

            free(curr);
        }
    }
    else if (value < (*node)->value)
    {
        btree_delete(&(*node)->left, value);
    }
    else
    {
        btree_delete(&(*node)->right, value);
    }
}

int btree_max(btree * node)
{
    if (node->right != NULL)
    {
        return btree_max(node->right);
    }
    return node->value;
}

void btree_destroy(btree ** node)
{
    if (*node == NULL) return;
    
    if ((*node)->left == NULL && (*node)->right == NULL)
    {
        free(*node);
        *node = NULL;
        return;
    }
    
    if ((*node)->left != NULL)
    {
        btree_destroy(&(*node)->left);
    }
    if ((*node)->right != NULL)
    {
        btree_destroy(&(*node)->right);
    }
    
    free(*node);
    *node = NULL;
}

void btree_print(btree * node)
{
    if (node == NULL) return;
    
    printf("(");
    if (node->left != NULL)
    {
        btree_print(node->left);
    }
    printf(")");
    
    printf("(");
    if (node->right != NULL)
    {
        btree_print(node->right);
    }
    printf(")\n");
}






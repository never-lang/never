#include <stdio.h>
#include <assert.h>
#include "gencode.h"
#include "symtab.h"
#include "freevar.h"

/**
 * free variables
 */
int expr_id_gencode(unsigned int syn_level, func * func_value, expr * value, int * result)
{
    symtab_entry * entry = NULL;
    
    entry = symtab_lookup(func_value->stab, value->id, SYMTAB_NESTED);
    if (entry != NULL && entry->var_func_value)
    {
        if (entry->type == SYMTAB_FUNC)
        {
            func * sup_func_value = (func *)entry->var_func_value;
            if (sup_func_value)
            {
                if (syn_level == entry->syn_level || entry->syn_level == 0)
                {
                    value->id_type_value = ID_TYPE_FUNC;
                    value->id_func_value = sup_func_value;
                }
                else
                {
                    freevar * freevar_value = NULL;
                    if (func_value->freevars == NULL)
                    {
                        func_value->freevars = freevar_list_new();
                    }
                    
                    freevar_value = freevar_list_add(func_value->freevars, value->id);
                
                    value->id_type_value = ID_TYPE_GLOBAL;
                    value->id_freevar_value = freevar_value;
                }
            }
        }
        else if (entry->type == SYMTAB_VAR)
        {
            var * var_value = (var *)entry->var_func_value;
            if (var_value->type == VAR_INT || var_value->type == VAR_FUNC)
            {
                if (syn_level == entry->syn_level)
                {
                    value->id_type_value = ID_TYPE_LOCAL;
                    value->id_var_value = var_value;
                }
                else
                {
                    freevar * freevar_value = NULL;
                    if (func_value->freevars == NULL)
                    {
                        func_value->freevars = freevar_list_new();
                    }
                    
                    freevar_value = freevar_list_add(func_value->freevars, value->id);
                
                    value->id_type_value = ID_TYPE_GLOBAL;
                    value->id_freevar_value = freevar_value;
                }
            }
        }
    }
    else
    {                    
        *result = 1;
        printf("cannot find variable %s, at this stage it is very bad\n", value->id);
        assert(0);
    }
    
    return 0;
}

int expr_gencode(unsigned int syn_level, func * func_value, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
            /* printf("gencode EXPR_INT %d\n", value->int_value); */
        break;
        case EXPR_ID:
            expr_id_gencode(syn_level, func_value, value, result);
        break;
        case EXPR_NEG:
            expr_gencode(syn_level, func_value, value->left, result);
        break;
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_LTE:
        case EXPR_GTE:
        case EXPR_EQ:
            expr_gencode(syn_level, func_value, value->left, result);
            expr_gencode(syn_level, func_value, value->right, result);
        break;
        break;
        case EXPR_SUP:
            expr_gencode(syn_level, func_value, value->left, result);
        break;
        case EXPR_COND:
            expr_gencode(syn_level, func_value, value->left, result);
            expr_gencode(syn_level, func_value, value->middle, result);
            expr_gencode(syn_level, func_value, value->right, result);
        break;
        case EXPR_CALL:
            expr_gencode(syn_level, func_value, value->func_expr, result);
            if (value->vars)
            {
                expr_list_gencode(syn_level, func_value, value->vars, result);
            }
        break;
        case EXPR_FUNC:
            if (value->func_value)
            {
                func_gencode(syn_level + 1, value->func_value, result);
            }
        break;
    }
    return 0;
}

int expr_list_gencode(unsigned int syn_level, func * func_value, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            expr_gencode(syn_level, func_value, value, result);
        }
        node = node->next;
    }
    return 0;
}

int func_enum_vars(func * func_value)
{
    int index = 0;
    var_list_node * node = NULL;
    
    node = func_value->vars->tail;
    while (node != NULL)
    {
        var * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
        }
        node = node->next;
    }
    return 0;
}

/**
 * free variables
 */
int func_gencode_freevars_freevar(func * func_value, freevar * freevar_value, int * result)
{
    /** search in symtab */
    /** if found then mark as local */
    /** otherwise mark as global and set index */
    symtab_entry * entry = NULL;

    entry = symtab_lookup(func_value->stab, freevar_value->id, SYMTAB_FLAT);
    if (entry != NULL && entry->var_func_value)
    {
        if (entry->type == SYMTAB_FUNC)
        {
            freevar_value->type = FREEVAR_FUNC;
            freevar_value->func_value = entry->var_func_value;
        }
        else if (entry->type == SYMTAB_VAR)
        {
            freevar_value->type = FREEVAR_LOCAL;
            freevar_value->local_value = entry->var_func_value;
        }
    }
    else
    {                    
        freevar * freevar_sup_value = NULL;
        if (func_value->freevars == NULL)
        {
            func_value->freevars = freevar_list_new();
        }
                    
        freevar_sup_value = freevar_list_add(func_value->freevars, freevar_value->id);
        
        freevar_value->type = FREEVAR_GLOBAL;
        freevar_value->global_value = freevar_sup_value;
    }
    
    return 0;
}

int func_gencode_freevars_expr(func * func_value, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
            /* not possible */
        break;
        case EXPR_ID:
            /* not possible */
        break;
        case EXPR_NEG:
            func_gencode_freevars_expr(func_value, value->left, result);
        break;
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_LTE:
        case EXPR_GTE:
        case EXPR_EQ:
            func_gencode_freevars_expr(func_value, value->left, result);
            func_gencode_freevars_expr(func_value, value->right, result);
        break;
        break;
        case EXPR_SUP:
            func_gencode_freevars_expr(func_value, value->left, result);
        break;
        case EXPR_COND:
            func_gencode_freevars_expr(func_value, value->left, result);
            func_gencode_freevars_expr(func_value, value->middle, result);
            func_gencode_freevars_expr(func_value, value->right, result);
        break;
        case EXPR_CALL:
            func_gencode_freevars_expr(func_value, value->func_expr, result);
            if (value->vars)
            {
                func_gencode_freevars_expr_list(func_value, value->vars, result);
            }
        break;
        case EXPR_FUNC:
            if (value->func_value)
            {
                func_gencode_freevars_func(func_value, value->func_value, result);
            }
        break;
    }

    return 0;
}

int func_gencode_freevars_expr_list(func * func_value, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            func_gencode_freevars_expr(func_value, value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars_func(func * func_value, func * subfunc_value, int * result)
{
    if (subfunc_value->freevars)
    {
        freevar_list_node * node = subfunc_value->freevars->tail;
        while (node != NULL)
        {
            freevar * freevar_value = node->value;
            if (freevar_value != NULL)
            {
                func_gencode_freevars_freevar(func_value, freevar_value, result);
            }        
            node = node->next;
        }
    }
    
    return 0;
}

int func_gencode_freevars_func_list(func * func_value, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    
    while (node != NULL)
    {
        func * subfunc_value = node->value;
        if (subfunc_value != NULL)
        {
            func_gencode_freevars_func(func_value, subfunc_value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars(func * func_value, int * result)
{
    if (func_value->body && func_value->body->funcs)
    {
        func_gencode_freevars_func_list(func_value, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        func_gencode_freevars_expr(func_value, func_value->body->ret, result);
    }

    return 0;
}

int func_gencode(unsigned int syn_level, func * func_value, int * result)
{
    if (func_value->stab != NULL)
    {
        symtab_set_syn_level(func_value->stab, syn_level);
    }
    if (func_value->vars != NULL)
    {
        func_enum_vars(func_value);
    }

    if (func_value->body && func_value->body->funcs)
    {
        func_list_gencode(syn_level + 1, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_gencode(syn_level, func_value, func_value->body->ret, result);
    }
    
    /** set subfunction local/global indexes **/
    func_gencode_freevars(func_value, result);
        
    return 0;
}

int func_list_gencode(unsigned int syn_level, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            func_gencode(syn_level, func_value, result);
        }
        node = node->next;
    }
    return 0;
}

int never_gencode(never * nev)
{
    int gencode_res = 0;
    unsigned int syn_level = 0;

    if (nev->funcs)
    {
        if (nev->stab != NULL)
        {
            symtab_set_syn_level(nev->stab, syn_level);
        }
        func_list_gencode(syn_level + 1, nev->funcs, &gencode_res);
    }
    
    return gencode_res;
}

/**
 * emit code
 */
int expr_int_emit(expr * value, int * result)
{
    printf("emit int %d\n", value->int_value);
    return 0;
} 

int expr_id_func_freevar_emit(freevar * value, int * result)
{
    switch (value->type)
    {
        case FREEVAR_UNKNOWN:
            printf("unknown freevar during emit\n");
            assert(0);
        break;
        case FREEVAR_LOCAL:
        case FREEVAR_GLOBAL:
            freevar_print(value);
        break;
        case FREEVAR_FUNC:
            expr_id_func_freevar_list_emit(value->func_value, result);
            freevar_print(value);
        break;
    }

    return 0;
}

int expr_id_func_freevar_list_emit(func * func_value, int * result)
{
    freevar_list_node * node;
    if (func_value->freevars == NULL)
    {
        return 0;
    }
    
    node = func_value->freevars->tail;
    while (node != NULL)
    {
        freevar * value = node->value;
        if (value != NULL)
        {
            expr_id_func_freevar_emit(value, result);
        }
        node = node->next;
    }
    printf("global vec %d\n", func_value->freevars->count);

    return 0;
}

int expr_id_func_emit(expr * value, int * result)
{
    if (value->id_func_value->id)
    {
        printf("emit id func %s\n", value->id_func_value->id);
    }
    else
    {
        printf("emit id func (nil)\n");
    }
    if (value->id_func_value)
    {
        expr_id_func_freevar_list_emit(value->id_func_value, result);
    }
    return 0;
}
 
int expr_id_emit(expr * value, int * result)
{
    switch (value->id_type_value)
    {
        case ID_TYPE_UNKNOWN:
            printf("not recognized id, at this stage it is very bad\n");
            assert(0);
        break;
        case ID_TYPE_LOCAL:
            printf("emit id local\n");
            var_print(value->id_var_value);
        break;
        case ID_TYPE_GLOBAL:
            printf("emit id global\n");
            freevar_print(value->id_freevar_value);
        break;
        case ID_TYPE_FUNC:
            expr_id_func_emit(value, result);
        break;
    }
    return 0;
} 
 
int expr_emit(expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
            expr_int_emit(value, result); 
        break;
        case EXPR_ID:
            expr_id_emit(value, result);
        break;
        case EXPR_NEG:
            expr_emit(value->left, result);
            printf("op neg\n");
        break;
        case EXPR_ADD:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op add\n");
        break;
        case EXPR_SUB:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op sub\n");
        break;
        case EXPR_MUL:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op mul\n");
        break;
        case EXPR_DIV:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op div\n");
        break;
        case EXPR_LT:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op lt\n");
        break;
        case EXPR_GT:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op gt\n");
        break;
        case EXPR_LTE:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op lte\n");
        break;
        case EXPR_GTE:
            expr_emit(value->right, result);
            expr_emit(value->left, result);
            printf("op gte\n");
        break;
        case EXPR_EQ:
            expr_emit(value->right, result);
            expr_emit(value->right, result);
            printf("op eq\n");
        break;
        break;
        case EXPR_SUP:
            expr_emit(value->left, result);
        break;
        case EXPR_COND:
            expr_emit(value->left, result);
            printf("jumpz labelA\n");
            expr_emit(value->middle, result);
            printf("jump labelB\n");
            printf("labelA:\n");
            expr_emit(value->right, result);
            printf("labelB:\n");
        break;
        case EXPR_CALL:
            printf("mark\n");
            if (value->vars)
            {
                expr_list_emit(value->vars, result);
            }
            expr_emit(value->func_expr, result);
            printf("call func\n");
        break;
        case EXPR_FUNC:
            if (value->func_value)
            {
                func_emit(value->func_value, result);
            }
        break;
    }
    return 0;
}

int expr_list_emit(expr_list * list, int * result)
{
    expr_list_node * node = list->head;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_emit(value, result);
        }
        node = node->prev;
    }

    return 0;
}

int func_emit(func * func_value, int * result)
{
    if (func_value->id != NULL)
    {
        printf("\nemit func def %s\n", func_value->id);
    }
    else
    {
        printf("\nemit func def (nil)\n");
    }

    if (func_value->body && func_value->body->ret)
    {
        expr_emit(func_value->body->ret, result);
        if (func_value->vars == NULL)
        {
            printf("emit ret 0 from %s\n", func_value->id);
        }
        else
        {
            printf("emit ret %d from %s\n", func_value->vars->count, func_value->id);
        }
    }
    if (func_value->body && func_value->body->funcs)
    {
        func_list_emit(func_value->body->funcs, result);
    }
        
    return 0;
}

int func_list_emit(func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_emit(func_value, result);
        }
        node = node->next;
    }
    return 0;
}

int never_emit(never * nev)
{
    int gencode_res = 0;

    if (nev->funcs)
    {
        func_list_emit(nev->funcs, &gencode_res);
    }
    
    return gencode_res;
}





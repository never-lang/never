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
int expr_int_emit(expr * value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };
    
    bc.type = BYTECODE_INT;
    bc.integer.value = value->int_value;

    bytecode_add(code, &bc);

    return 0;
} 

int expr_id_func_freevar_emit(freevar * value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    switch (value->type)
    {
        case FREEVAR_UNKNOWN:
            printf("unknown freevar during emit\n");
            assert(0);
        break;
        case FREEVAR_LOCAL:
            bc.type = BYTECODE_ID_LOCAL;
            bc.id_local.stack_level = stack_level;
            bc.id_local.index = value->local_value->index;

            bytecode_add(code, &bc);
        break;
        case FREEVAR_GLOBAL:
            bc.type = BYTECODE_ID_GLOBAL;
            bc.id_global.index = value->global_value->index;

            bytecode_add(code, &bc);
        break;
        case FREEVAR_FUNC:
            if (value != NULL)
            {
                expr_id_func_freevar_list_emit(value->func_value, stack_level, code, result);
            }
        break;
    }

    return 0;
}

int expr_id_func_freevar_list_emit(func * func_value, int stack_level, bytecode_list * code, int * result)
{
    int count = 0;
    bytecode bc = { 0 };
    
    if (func_value != NULL && func_value->freevars != NULL)
    {
        int e = 0;
        count = func_value->freevars->count;
        freevar_list_node * node = func_value->freevars->tail;
        while (node != NULL)
        {
            freevar * value = node->value;
            if (value != NULL)
            {
                expr_id_func_freevar_emit(value, stack_level + e++, code, result);
            }
            node = node->next;
        }
    }
    
    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = count;
    bytecode_add(code, &bc);

    return 0;
}

int expr_id_func_emit(func * func_value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    expr_id_func_freevar_list_emit(func_value, stack_level, code, result);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(code, &bc);

    return 0;
}
 
int expr_id_emit(expr * value, int stack_level, bytecode_list * code, int * result)
{
    switch (value->id_type_value)
    {
        case ID_TYPE_UNKNOWN:
            printf("not recognized id, at this stage it is very bad\n");
            assert(0);
        break;
        case ID_TYPE_LOCAL:
        {
            bytecode bc = { 0 };
            
            bc.type = BYTECODE_ID_LOCAL;
            bc.id_local.stack_level = stack_level;
            bc.id_local.index = value->id_var_value->index;
        
            bytecode_add(code, &bc);
        }
        break;
        case ID_TYPE_GLOBAL:
        {
            bytecode bc = { 0 };
            
            bc.type = BYTECODE_ID_GLOBAL;
            bc.id_global.index = value->id_freevar_value->index;

            bytecode_add(code, &bc);        
        }
        break;
        case ID_TYPE_FUNC:
            if (value->id_func_value != NULL)
            {
                expr_id_func_emit(value->id_func_value, stack_level, code, result);
            }
        break;
    }
    return 0;
} 

int expr_cond_emit(expr * value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };
    bytecode * cond, * condz;
    bytecode * labelA, * labelB;

    expr_emit(value->left, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(code, &bc);

    expr_emit(value->middle, stack_level, code, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(code, &bc);    
    condz->jump.offset = labelA->addr - condz->addr;

    expr_emit(value->right, stack_level, code, result);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(code, &bc);
    cond->jump.offset = labelB->addr - cond->addr;

    return 0;
}

int expr_call_emit(expr * value, int stack_level, bytecode_list * code, int * result)
{
    int v = 0;
    bytecode bc = { 0 };
    bytecode * mark, * label;

    bc.type = BYTECODE_MARK;
    mark = bytecode_add(code, &bc);
    
    if (value->vars)
    {
         v = value->vars->count;
         expr_list_emit(value->vars, stack_level, code, result);
    }
    expr_emit(value->func_expr, stack_level + v, code, result);

    bc.type = BYTECODE_CALL;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    label = bytecode_add(code, &bc);
    mark->mark.addr = label->addr;

    return 0;
}

int expr_func_emit(func * func_value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };
    bytecode * jump, * label;
                
    bc.type = BYTECODE_JUMP;
    jump = bytecode_add(code, &bc);
            
    func_emit(func_value, stack_level, code, result);
                
    bc.type = BYTECODE_LABEL;
    label = bytecode_add(code, &bc);
    jump->jump.offset = label->addr - jump->addr;

    return 0;
}
 
int expr_emit(expr * value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    switch (value->type)
    {
        case EXPR_INT:
            expr_int_emit(value, stack_level, code, result); 
        break;
        case EXPR_ID:
            expr_id_emit(value, stack_level, code, result);
        break;
        case EXPR_NEG:
            expr_emit(value->left, stack_level, code, result);

            bc.type = BYTECODE_OP_NEG;
            bytecode_add(code, &bc);
        break;
        case EXPR_ADD:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_ADD;
            bytecode_add(code, &bc);
        break;
        case EXPR_SUB:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_SUB;
            bytecode_add(code, &bc);
        break;
        case EXPR_MUL:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_MUL;
            bytecode_add(code, &bc);
        break;
        case EXPR_DIV:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_LINE;
            bc.line.no = value->line_no;
            bytecode_add(code, &bc);

            bc.type = BYTECODE_OP_DIV;
            bytecode_add(code, &bc);
        break;
        case EXPR_LT:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_LT;
            bytecode_add(code, &bc);
        break;
        case EXPR_GT:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_GT;
            bytecode_add(code, &bc);
        break;
        case EXPR_LTE:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_LTE;
            bytecode_add(code, &bc);
        break;
        case EXPR_GTE:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->left, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_GTE;
            bytecode_add(code, &bc);
        break;
        case EXPR_EQ:
            expr_emit(value->right, stack_level, code, result);
            expr_emit(value->right, stack_level + 1, code, result);

            bc.type = BYTECODE_OP_EQ;
            bytecode_add(code, &bc);
        break;
        break;
        case EXPR_SUP:
            expr_emit(value->left, stack_level, code, result);
        break;
        case EXPR_COND:
            expr_cond_emit(value, stack_level, code, result);
        break;
        case EXPR_CALL:
            expr_call_emit(value, stack_level, code, result);
        break;
        case EXPR_FUNC:
            if (value->func_value)
            {
                expr_func_emit(value->func_value, stack_level, code, result);
                expr_id_func_emit(value->func_value, stack_level, code, result);
            }
        break;
    }
    return 0;
}

int expr_list_emit(expr_list * list, int stack_level, bytecode_list * code, int * result)
{
    int e = 0;
    expr_list_node * node = list->head;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_emit(value, stack_level + e++, code, result);
        }
        node = node->prev;
    }

    return 0;
}

int func_emit(func * func_value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    func_value->addr = code->addr;
    bc.type = BYTECODE_FUNC_DEF;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LINE;
    bc.line.no = func_value->line_no;
    bytecode_add(code, &bc);
    
    if (func_value->body && func_value->body->funcs)
    {
        bytecode * jump, * label;

        bc.type = BYTECODE_JUMP;
        jump = bytecode_add(code, &bc);

        func_list_emit(func_value->body->funcs, stack_level, code, result);

        bc.type = BYTECODE_LABEL;
        label = bytecode_add(code, &bc);
        jump->jump.offset = label->addr - jump->addr;
    }

    if (func_value->body && func_value->body->ret)
    {
        int vars = 0;
        expr_emit(func_value->body->ret, stack_level, code, result);

        if (func_value->vars != NULL)
        {
            vars = func_value->vars->count;
        }
        
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->body->ret->line_no;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_RET;
        bc.ret.count = vars;
        bytecode_add(code, &bc);
    }
        
    return 0;
}

int func_list_emit(func_list * list, int stack_level, bytecode_list * code, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_emit(func_value, stack_level, code, result);
        }
        node = node->next;
    }
    return 0;
}

int func_main_emit(never * nev, int stack_level, bytecode_list * code, int * result)
{
    symtab_entry * entry = NULL;
    entry = symtab_lookup(nev->stab, "main", SYMTAB_FLAT);
    if (entry != NULL && entry->var_func_value)
    {
        bytecode bc = { 0 };
        
        bc.type = BYTECODE_GLOBAL_VEC;
        bc.global_vec.count = 0;
        bytecode_add(code, &bc);
       
        bc.type = BYTECODE_ID_FUNC_FUNC;
        bc.id_func.func_value = entry->var_func_value;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_CALL;
        bytecode_add(code, &bc);
    }
    else
    {
        *result = GENCODE_FAIL;
        printf("no main function defined\n");
    }
    return 0;
}

int never_emit(never * nev, bytecode_list * code)
{
    int stack_level = 0;
    int gencode_res = 0;

    if (nev->funcs)
    {
        func_main_emit(nev, stack_level, code, &gencode_res);
        func_list_emit(nev->funcs, stack_level, code, &gencode_res);
    }
    
    return gencode_res;
}





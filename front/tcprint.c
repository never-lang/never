#include "tcprint.h"

/**
 * print functions
 */
int print_func_attr(expr * value, int depth)
{
    if (value->attr.record_value != NULL)
    {
        print_func_expr(value->attr.record_value, depth);
    }
    return 0;
}

int print_func_match_guard(match_guard * value, int depth)
{
    switch (value->type)
    {
    case MATCH_GUARD_ITEM:
        print_func_expr(value->guard_item.expr_value, depth);
    break;
    case MATCH_GUARD_ELSE:
        print_func_expr(value->guard_else.expr_value, depth);
    break;
    }

    return 0;
}

int print_func_match_guard_list(match_guard_list * list, int depth)
{
    match_guard_list_node * node = list->tail;
    
    while (node != NULL)
    {
        match_guard * value = node->value;
        if (value != NULL)
        {
            print_func_match_guard(value, depth);
        }
        node = node->next;
    }
    
    return 0;
}

int print_func_match_expr(expr * value, int depth)
{
    print_func_expr(value->match.expr_value, depth);
    print_func_match_guard_list(value->match.match_guards, depth);

    return 0;
}

int print_func_expr(expr * value, int depth)
{
    switch (value->type)
    {
    case EXPR_INT:
    case EXPR_FLOAT:
    case EXPR_CHAR:
    case EXPR_STRING:
        /* no symtabs possible */
        break;
    case EXPR_ID:
        /* no symtabs possible */
        break;
    case EXPR_NIL:
        /* no symtabs possible */
        break;
    case EXPR_NEG:
        print_func_expr(value->left, depth);
        break;
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    case EXPR_EQ:
    case EXPR_NEQ:
        print_func_expr(value->left, depth);
        print_func_expr(value->right, depth);
        break;
    case EXPR_AND:
    case EXPR_OR:
        print_func_expr(value->left, depth);
        print_func_expr(value->right, depth);
        break;
    case EXPR_NOT:
        print_func_expr(value->left, depth);
        break;
    case EXPR_SUP:
        print_func_expr(value->left, depth);
        break;
    case EXPR_COND:
        print_func_expr(value->left, depth);
        print_func_expr(value->middle, depth);
        print_func_expr(value->right, depth);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            print_func_array(value->array.array_value, depth);
        }
        break;
    case EXPR_ARRAY_DEREF:
        print_func_expr(value->array_deref.array_expr, depth);
        print_func_expr_list(value->array_deref.ref, depth);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        print_func_expr(value->call.func_expr, depth);
        if (value->call.params != NULL)
        {
            print_func_expr_list(value->call.params, depth);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value != NULL)
        {
            print_func(value->func_value, depth + 1);
        }
        break;
    case EXPR_SEQ:
        if (value->seq.list != NULL)
        {
            print_func_expr_list(value->seq.list, depth);
        }
        break;
    case EXPR_ASS:
        print_func_expr(value->left, depth);
        print_func_expr(value->right, depth);
        break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
        print_func_expr(value->whileloop.cond, depth);
        print_func_expr(value->whileloop.do_value, depth);
        break;
    case EXPR_FOR:
        print_func_expr(value->forloop.init, depth);
        print_func_expr(value->forloop.cond, depth);
        print_func_expr(value->forloop.incr, depth);
        print_func_expr(value->forloop.do_value, depth);
        break;
    case EXPR_MATCH:
        print_func_match_expr(value, depth);
        break;
    case EXPR_BUILD_IN:
        if (value->func_build_in.param != NULL)
        {
            print_func_expr_list(value->func_build_in.param, depth + 1);
        }
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        if (value->left != NULL)
        {
            print_func_expr(value->left, depth);
        }
        break;
    case EXPR_LISTCOMP:
        if (value->listcomp_value != NULL)
        {
            print_func_listcomp(value->listcomp_value, depth);
        }
        break;
    case EXPR_ATTR:
        print_func_attr(value, depth);
        break;
    }
    return 0;
}

int print_func_expr_list(expr_list * list, int depth)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            print_func_expr(value, depth);
        }
        node = node->next;
    }
    return 0;
}

int print_func_qualifier(qualifier * value, int depth)
{
    switch (value->type)
    {
        case QUALIFIER_UNKNOWN:
            assert(0);
        break;
        case QUALIFIER_GENERATOR:
            if (value->expr_value != NULL)
            {
                print_func_expr(value->expr_value, depth);
            }
        break;
        case QUALIFIER_FILTER:
            if (value->expr_value != NULL)
            {
                print_func_expr(value->expr_value, depth);
            }
        break;
    }

    return 0;
}

int print_func_qualifier_list(qualifier_list * list, int depth)
{
    qualifier_list_node * node = list->tail;

    while (node != NULL)
    {
        qualifier * qualifier_value = node->value;
        if (qualifier_value != NULL)
        {
            print_func_qualifier(qualifier_value, depth);
        }
        node = node->next;
    }

    return 0;
}

int print_func_listcomp(listcomp * value, int depth)
{
    if (value->list != NULL)
    {
        print_func_qualifier_list(value->list, depth);
    }
    if (value->expr_value != NULL)
    {
        print_func_expr(value->expr_value, depth);
    }

    return 0;
}

int print_func_array(array * value, int depth)
{
    if (value->elements != NULL)
    {
        print_func_expr_list(value->elements, depth);
    }
    if (value->dims != NULL)
    {
        print_func_expr_list(value->dims, depth);
    }

    return 0;
}

int print_func_bind(bind * value, int depth)
{
    if (value->expr_value)
    {
        print_func_expr(value->expr_value, depth);
    }

    return 0;
}

int print_func_bind_list(bind_list * list, int depth)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * value = node->value;
        if (value != NULL)
        {
            print_func_bind(value, depth);
        }

        node = node->next;
    }
    return 0;
}

int print_func_except(except * value, int depth)
{
    if (value->id != NULL)
    {
        printf("except %s\n", value->id);
    }
    if (value->expr_value != NULL)
    {
        print_func_expr(value->expr_value, depth);
    }

    return 0;
}

int print_func_except_list(except_list * list, int depth)
{
    except_list_node * node = list->tail;
    while (node != NULL)
    {
        except * value = node->value;
        if (value != NULL)
        {
            print_func_except(value, depth);
        }
        node = node->next;
    }
    return 0;
}

int print_func_ffi(func * value, int depth)
{
    if (value->decl->id != NULL)
    {
        printf("\nextern function (%d): %d %s@%u\n", depth, value->index,
               value->decl->id, value->addr);
    }

    return 0;
}

int print_func_native(func * value, int depth)
{
    if (value->decl->id != NULL)
    {
        printf("\nfunction (%d): %d %s@%u\n", depth, value->index,
               value->decl->id, value->addr);
    }
    if (value->stab)
    {
        symtab_print(value->stab);
    }
    if (value->freevars != NULL)
    {
        freevar_list_print(value->freevars);
    }
    if (value->body != NULL && value->body->binds != NULL)
    {
        print_func_bind_list(value->body->binds, depth);
    }
    if (value->body != NULL && value->body->funcs != NULL)
    {
        print_func_list(value->body->funcs, depth + 1);
    }
    if (value->body != NULL && value->body->ret != NULL)
    {
        print_func_expr(value->body->ret, depth);
    }
    if (value->except != NULL && value->except->list != NULL)
    {
        print_func_except_list(value->except->list, depth);
    }
    if (value->except != NULL && value->except->all != NULL)
    {
        print_func_except(value->except->all, depth);
    }

    return 0;
}

int print_func(func * value, int depth)
{
    switch (value->type)
    {
        case FUNC_TYPE_UNKNOWN:
            assert(0);
        break;
        case FUNC_TYPE_NATIVE:
            print_func_native(value, depth);
        break;
        case FUNC_TYPE_FFI:
            print_func_ffi(value, depth);
        break;
    }

    return 0;
}

int print_func_list(func_list * list, int depth)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * value = node->value;
        if (value != NULL)
        {
            print_func(value, depth);
        }

        node = node->next;
    }
    return 0;
}

int print_functions(never * nev)
{
    if (nev->funcs)
    {
        print_func_list(nev->funcs, 1);
    }
    return 0;
}



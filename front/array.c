#include <stdlib.h>
#include "array.h"

array * array_new(expr_list * elements, var * ret)
{
    array * value = (array *)malloc(sizeof(array));
    
    value->type = ARRAY_INIT;
    value->elements = elements;
    value->dims = NULL;
    value->ret = ret;
    value->line_no = 0;
    
    return value;
}

array * array_new_sub(expr_list * elements)
{
    array * value = (array *)malloc(sizeof(array));
    
    value->type = ARRAY_SUB;
    value->elements = elements;
    value->dims = NULL;
    value->ret = NULL;
    value->line_no = 0;
    
    return value;
}

array * array_new_dims(expr_list * dims, var * ret)
{
    array * value = (array *)malloc(sizeof(array));
    
    value->type = ARRAY_DIMS;
    value->elements = NULL;
    value->dims = dims;
    value->ret = ret;
    value->line_no = 0;
    
    return value;
}

void array_delete(array * value)
{
    if (value->elements != NULL)
    {
        expr_list_delete(value->elements);
    }
    if (value->dims != NULL)
    {
        expr_list_delete(value->dims);
    }
    if (value->ret != NULL)
    {
        var_delete(value->ret);
    }
    free(value);
}



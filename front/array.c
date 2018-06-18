#include <stdlib.h>
#include "array.h"

array * array_new(expr_list * elements, var * ret)
{
    array * value = (array *)malloc(sizeof(array));
    
    value->type = ARRAY_INIT;
    value->id = NULL;
    value->dims_count = 0;
    value->elements = elements;
    value->ret = ret;
    value->line_no = 0;
    
    return value;
}

array * array_new_sub(expr_list * elements)
{
    array * value = (array *)malloc(sizeof(array));
    
    value->type = ARRAY_SUB;
    value->id = NULL;
    value->dims_count = 0;
    value->elements = elements;
    value->ret = NULL;
    value->line_no = 0;
    
    return value;
}

array * array_new_dims(expr_list * dims, var * ret)
{
    array * value = (array *)malloc(sizeof(array));
    
    value->type = ARRAY_DIMS;
    value->id = NULL;
    value->elements = dims;
    value->dims_count = 0;
    value->ret = ret;
    value->line_no = 0;
    
    return value;
}

void array_delete(array * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    if (value->elements != NULL)
    {
        expr_list_delete(value->elements);
    }
    if (value->ret != NULL)
    {
        var_delete(value->ret);
    }
    free(value);
}



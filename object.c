#include <stdlib.h>
#include <stdio.h>
#include "object.h"

object * object_new()
{
    object * obj = (object *)malloc(sizeof(object));
    
    return obj;
}

void object_delete(object * obj)
{
    free(obj);
}


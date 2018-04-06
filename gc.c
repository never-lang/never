#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define GC_OBJ_ARRAY 0
#define GC_OBJ_MIXED 1

#define GC_OBJ_TYPE_VALUE 0
#define GC_OBJ_TYPE_PTR 1
#define GC_OBJ_TYPE_UNKNOWN 2

typedef struct gc_obj {
    unsigned int reach : 1;  /* mark-and-sweep */
    unsigned int type  : 1;  /* array, mixed pointers and values */ 
    unsigned int size  : 30; /* size of array */
} gc_obj;

typedef struct gc_obj_array {
    gc_obj header;
    char data[0];
} gc_obj_array;

typedef struct gc_obj_mixed {
    gc_obj header;
    char data[0];
} gc_obj_mixed;

unsigned int round32(unsigned int value)
{
    return value == 0 ? 0 : (((value - 1) >> 5) << 2) + 4;
}

gc_obj * gc_obj_new(unsigned int type, unsigned int size)
{
    gc_obj * ret;

    ret = (gc_obj *)malloc(sizeof(gc_obj) + sizeof(unsigned int) * size);

    ret->type = type;
    ret->size = size;
    
    return ret;
}

void gc_obj_free(gc_obj * obj)
{
    if (obj == NULL) return;    
    free(obj);
}

int gc_obj_get_int(gc_obj * obj, unsigned int index)
{
    return 0;
}

int gc_obj_get_type(gc_obj * obj, unsigned int index)
{
    if (obj->type == GC_OBJ_ARRAY)
    {
        if (obj->size > index)
        {
            return GC_OBJ_TYPE_VALUE;
        }
        else
        {
            return GC_OBJ_TYPE_UNKNOWN;
        }
    }
    else if (obj->type == GC_OBJ_MIXED)
    {
        if (obj->size > index)
        {
            char bit = ((gc_obj_mixed *)obj)->data[index / 4];
            return bit & (1 << (index % 4));
        }
        else
        {
            return GC_OBJ_TYPE_UNKNOWN;
        }
    }
    else
    {
        assert(0);
    }
}

void test_round32()
{
    assert(round32(0) == 0);
    assert(round32(1) == 4);
    assert(round32(2) == 4);
    assert(round32(32) == 4);
    assert(round32(33) == 8);
    assert(round32(34) == 8);
    assert(round32(64) == 8);
}

int main(int argc, char * argv[])
{
    printf("sizeof(gc_obj) == %ld\n", sizeof(gc_obj));

    test_round32();

    return 0;
}


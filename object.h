#ifndef __OBJECT_H__
#define __OBJECT_H__

typedef enum object_type
{
    OBJECT_INT = 1,
    OBJECT_VEC = 2,
    OBJECT_FUNC = 3
} object_type;

typedef struct object_vec
{
    unsigned int size;
    unsigned int * value;
} object_vec;

typedef struct object_func
{
    unsigned int vec;
    unsigned int addr;
} object_func;

typedef struct object
{
    object_type type;
    union
    {
        int int_value; /* OBJECT_INT */
        object_vec * vec_value; /* OBJECT_VEC */
        object_func * func_value; /* OBJECT_FUNC */
    };
} object;

object * object_new_int(int value);
object * object_new_vec(unsigned int size);
object * object_new_func(unsigned int vec, unsigned int addr);

void object_delete(object * obj);

void object_print(object * obj);

#endif /* __OBJECT_H__ */



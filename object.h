#ifndef __OBJECT_H__
#define __OBJECT_H__

typedef unsigned int ip_ptr;
typedef int mem_ptr;
typedef int stack_ptr;

typedef enum object_type
{
    OBJECT_UNKNOWN = 0,
    OBJECT_INT = 1,
    OBJECT_VEC = 2,
    OBJECT_FUNC = 3
} object_type;

typedef struct object_vec
{
    unsigned int size;
    mem_ptr * value;
} object_vec;

typedef struct object_func
{
    mem_ptr vec;
    ip_ptr addr;
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
object * object_new_func(mem_ptr vec, ip_ptr addr);

void object_delete(object * obj);

void object_print(object * obj);

#endif /* __OBJECT_H__ */



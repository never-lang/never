/**
 * Copyright 2018 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __OBJECT_H__
#define __OBJECT_H__

/**
 * Functions and data structures to allocate memory object needed by
 * virtual machine. Mostly used by garbage collector which allocates and
 * deallocates those objects on virtual machine requests.
 */

/**
 * @brief ip_ptr - pointer to code
 */
typedef unsigned int ip_ptr;
/**
 * @brief mem_ptr - reference to memory
 */
typedef unsigned int mem_ptr;
/**
 * @brief stack_ptr - reference to stack
 */
typedef int stack_ptr;
/**
 * @brief nil_ptr - reference to nothing
 */
extern unsigned int nil_ptr;

/**
 * @brief object identifiers
 */
typedef enum object_type
{
    OBJECT_UNKNOWN = 0,
    OBJECT_INT = 1,
    OBJECT_LONG = 2,
    OBJECT_FLOAT = 3,
    OBJECT_DOUBLE = 4,
    OBJECT_CHAR = 5,
    OBJECT_STRING = 6,
    OBJECT_STRING_REF = 7,
    OBJECT_STRING_ARR = 14,
    OBJECT_C_PTR = 8,
    OBJECT_ARRAY = 9,
    OBJECT_ARRAY_REF = 10,
    OBJECT_VEC = 11,
    OBJECT_VEC_REF = 12,
    OBJECT_FUNC = 13
} object_type;

/**
 * @brief Vector of memory pointers
 *
 * @param size - number of elements
 * @param value - array with memory pointers
 */
typedef struct object_vec
{
    unsigned int size;
    mem_ptr * value;
} object_vec;

/**
 * @brief Array dimension
 * 
 * @param elems - number of array elements in given dimension
 * @param mult - multiplier used to access elements of the array
 *               @see e.g. object_arr_dim_addr
 */
typedef struct object_arr_dim
{
    unsigned int elems;
    unsigned int mult;
} object_arr_dim;

/**
 * @brief Array
 * 
 * @param dims - number of dimensions
 * @param elems - number of all elements in the array
 * @param dv - pointer to array of dimensions
 * @param value - pointer to continous array of elements
 */
typedef struct object_arr
{
    unsigned int dims;
    unsigned int elems;
    object_arr_dim * dv;
    mem_ptr * value;
} object_arr;

/**
 * @brief Array of strings
 *        Used to pass string arguments to a never program
 */
typedef struct object_str_arr
{
    unsigned int argc;
    char ** argv;
} object_str_arr;

/**
 * @brief Function
 * 
 * @param vec - reference to vector of free variables in function
 * @param addr - pointer to code
 */
typedef struct object_func
{
    mem_ptr vec;
    ip_ptr addr;
} object_func;

/**
 * @brief Different objects
 * 
 */
typedef struct object
{
    object_type type;
    union {
        int int_value;            /* OBJECT_INT */
        long long long_value;     /* OBJECT_LONG */
        float float_value;        /* OBJECT_FLOAT */
        double double_value;      /* OBJECT_DOUBLE */
        char char_value;          /* OBJECT_CHAR */
        char * string_value;      /* OBJECT_STRING */
        mem_ptr string_ref_value; /* OBJECT_STRING_REF */
        object_str_arr * string_arr_value; /* OBJECT_STRING_ARR */
        void * c_ptr_value;       /* OBJECT_C_PTR */
        object_vec * vec_value;   /* OBJECT_VEC */
        mem_ptr vec_ref_value;    /* OBJECT_VEC_REF */
        object_arr * arr_value;   /* OBJECT_ARR */
        mem_ptr arr_ref_value;    /* OBJECT_ARR_REF */
        object_func * func_value; /* OBJECT_FUNC */
    };
} object;

/**
 * @brief Create objects which hold values of different types.
 * Returned object needs to be deleted using @see object_delete function.
 *
 * @param value - value to be stored in an object
 * @return - pointer to the object
 */
object * object_new_int(int value);
object * object_new_long(long long value);
object * object_new_float(float value);
object * object_new_double(double value);
object * object_new_char(float value);
object * object_new_c_ptr(void * value);

/**
 * @brief Create object which holds string.
 * object_new_string duplicats memory hold by a string
 * object_new_string_take stores pointer to a string. memory is not released
 * object_new_string_ref store reference to string
 * 
 * @param value 
 * @return - pointer to the object
 */
object * object_new_string(char * value);
object * object_new_string_take(char * value);
object * object_new_string_ref(mem_ptr str_value);

/**
 * @brief Create vector of memory pointers of given size.
 * 
 * @param size - number of memory pointers to be kept
 * @return - pointer to the object
 */
object * object_new_vec(unsigned int size);

/**
 * @brief Create object which references other vector
 *
 * @param vec_value - address of a vector
 * @return - pointer to the object
 */
object * object_new_vec_ref(mem_ptr vec_value);

/**
 * @brief Create an array with given number of dimensions
 *
 * @param dims - number of dimensions
 * @param dv - array dimension descriptor
 * @return - pointer to the object
 */
object * object_new_arr(unsigned int dims, object_arr_dim * dv);

/**
 * @brief Create a reference to an array
 *
 * @param arr_value - reference to an array
 * @return - pointer to the object
 */
object * object_new_arr_ref(mem_ptr arr_value);

/**
 * @brief Create a function object
 * 
 * @param vec - vector of free variables
 * @param addr - address of function code
 * @return - pointer to the object
 */
object * object_new_func(mem_ptr vec, ip_ptr addr);

/**
 * @brief Delete an object. The object is deleted based on its type
 *
 * @param obj - pointer to the object being deleted
 */
void object_delete(object * obj);

/**
 * @brief Delete an array
 * 
 * @param arr_value - pointer to the array
 */
void object_arr_delete(object_arr * arr_value);

/**
 * @brief Create an array of strings
 * 
 * @param argc - number of strings
 * @param argv - array of pointers to strings
 * @return - pointer to the object
 */
object_str_arr * object_str_arr_new(unsigned int argc, char ** argv);

/**
 * @brief Delete an array of strings
 *
 * @param value - pointer to an array of strings
 */
void object_str_arr_delete(object_str_arr * value);

/**
 * @brief Create array dimension descriptor
 *
 * @param dims - number of dimensions
 * @return - pointer to the object
 */
object_arr_dim * object_arr_dim_new(unsigned int dims);

/**
 * @brief Delete array dimension descriptor.
 *
 * @param dv - pointer to the array dimension descriptor
 */
void object_arr_dim_delete(object_arr_dim * dv);

/**
 * @brief Initialize array dimension descriptor
 * 
 * @param dims - number of dimensions
 * @param dv - pointer to array dimension descriptor
 * @param elems - number of elements in the array
 */
void object_arr_dim_mult(unsigned int dims, object_arr_dim * dv,
                         unsigned int * elems);

/**
 * @brief Return index of the array element in its continuous memory region.
 *
 * @param dims - number of dimensions
 * @param dv - pointer to the array dimension descriptor
 * @param addr - array with element address
 * @param oobounds - -1 no error, != -1 number of dimension where out of bounds error occurred
 * @return element index
 */
unsigned int object_arr_dim_addr(unsigned int dims, object_arr_dim * dv,
                                 object_arr_dim * addr, int * oobounds);

/**
 * @brief Copy array dimension descriptor
 *
 * @param dims - number of dimensions
 * @param value - array dimension descriptor to be copied
 * @return copy of array dimension descriptor
 */
object_arr_dim * object_arr_dim_copy(unsigned int dims, object_arr_dim * value);

/**
 * @brief Return copy of an array. Retruned array is not initalized
 *
 * @param value - pointer to an array
 * @return 
 */
object * object_arr_copy(object * value);

/**
 * @brief Append an element to a vector. Vector is reallocated
 *
 * @param arr_value - pointer to a vector
 * @param value - an element which is to be added
 */
void object_arr_append(object_arr * arr_value, mem_ptr value);

/**
 * @brief Check if two arrays can be added. Have the same number of dimensions
 * and each dimension is has the same size.
 * 
 * @param arr1 - first array
 * @param arr2 - second array
 * @return 0 - arrays cannot be added, 1 - arrays can be added
 */
char object_arr_can_add(object_arr * arr1, object_arr * arr2);

/**
 * @brief  Check if two arrays can be multipled. Arrays need to be two dimensinal
 * and number of columns of the first array must be equal to the number of rows
 * of the second array.
  *
 * @param arr1 - first array
 * @param arr2 - second array
 * @return 0 - array cannot be multiplied, 1 - array can be multiplied
 */
char object_arr_can_mult(object_arr * arr1, object_arr * arr2);

/**
 * @brief Print an array object to stdout
 *
 * @param value - pointer to an object
 */
void object_arr_print(object_arr * value);

/**
 * @brief Print an object to stdout
 *
 * @param obj - pointer to an object
 */
void object_print(object * obj);

#endif /* __OBJECT_H__ */


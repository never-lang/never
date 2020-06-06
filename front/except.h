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
#ifndef __EXCEPT_H__
#define __EXCEPT_H__

#include "vm.h"
#include "expr.h"

typedef enum except_type
{
    EXCEPT_UNKNOWN = 0,
    EXCEPT_ALL = 1,
    EXCEPT_ID = 2
} except_type;

#define EXCEPT_NO_UNKNOWN_NAME "unknown_exception"
#define EXCEPT_NO_DIVISION_NAME "division_by_zero"
#define EXCEPT_NO_ARR_SIZE_NAME "wrong_array_size"
#define EXCEPT_NO_INDEX_OOB_NAME "index_out_of_bounds"
#define EXCEPT_NO_INVALID_NAME "invalid_domain"
#define EXCEPT_NO_OVERFLOW_NAME "overflow"
#define EXCEPT_NO_UNDERFLOW_NAME "underflow"
#define EXCEPT_NO_INEXACT_NAME "inexact"
#define EXCEPT_NIL_POINTER_NAME "nil_pointer"
#define EXCEPT_FFI_FAIL_NAME "ffi_fail"

typedef struct except
{
    except_type type;
    int no;
    char * id;
    expr * expr_value;
    unsigned int line_no;
} except;

typedef struct except_list_node
{
    except * value;
    struct except_list_node * prev;
    struct except_list_node * next;
} except_list_node;

typedef struct except_list
{
    unsigned int count;
    except_list_node * head;
    except_list_node * tail;
} except_list;

except * except_new_all(expr_list * seq);
except * except_new_id(char * id, expr_list * seq);

void except_delete(except * value);

except_list_node * except_list_node_new(except * value);
void except_list_node_delete(except_list_node * node);

except_list * except_list_new();
void except_list_delete(except_list * list);

void except_list_add_beg(except_list * list, except * value);
void except_list_add_end(except_list * list, except * value);

char * except_to_str(except_no no);

#endif /* __EXCEPT_H__ */


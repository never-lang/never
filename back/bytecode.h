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
#ifndef __BYTECODE_H__
#define __BYTECODE_H__

#define SLICE_SIZE 2
#define SLICE_ARRAY_INDEX 0
#define SLICE_RANGE_INDEX 1

typedef enum bytecode_type
{
    BYTECODE_UNKNOWN = 0,

    BYTECODE_INT,
    BYTECODE_FLOAT,
    BYTECODE_CHAR,
    BYTECODE_STRING,

    BYTECODE_ID_LOCAL,
    BYTECODE_ID_DIM_LOCAL,
    BYTECODE_ID_DIM_SLICE,
    BYTECODE_ID_GLOBAL,
    BYTECODE_ID_FUNC_FUNC,
    BYTECODE_ID_FUNC_ADDR,

    BYTECODE_OP_NEG_INT,
    BYTECODE_OP_ADD_INT,
    BYTECODE_OP_SUB_INT,
    BYTECODE_OP_MUL_INT,
    BYTECODE_OP_DIV_INT,
    BYTECODE_OP_MOD_INT,

    BYTECODE_OP_NEG_FLOAT,
    BYTECODE_OP_ADD_FLOAT,
    BYTECODE_OP_SUB_FLOAT,
    BYTECODE_OP_MUL_FLOAT,
    BYTECODE_OP_DIV_FLOAT,

    BYTECODE_OP_ADD_STRING,
    BYTECODE_OP_ADD_INT_STRING,
    BYTECODE_OP_ADD_STRING_INT,
    BYTECODE_OP_ADD_FLOAT_STRING,
    BYTECODE_OP_ADD_STRING_FLOAT,

    BYTECODE_OP_LT_INT,
    BYTECODE_OP_GT_INT,
    BYTECODE_OP_LTE_INT,
    BYTECODE_OP_GTE_INT,
    BYTECODE_OP_EQ_INT,
    BYTECODE_OP_NEQ_INT,

    BYTECODE_OP_LT_FLOAT,
    BYTECODE_OP_GT_FLOAT,
    BYTECODE_OP_LTE_FLOAT,
    BYTECODE_OP_GTE_FLOAT,
    BYTECODE_OP_EQ_FLOAT,
    BYTECODE_OP_NEQ_FLOAT,

    BYTECODE_OP_LT_CHAR,
    BYTECODE_OP_GT_CHAR,
    BYTECODE_OP_LTE_CHAR,
    BYTECODE_OP_GTE_CHAR,
    BYTECODE_OP_EQ_CHAR,
    BYTECODE_OP_NEQ_CHAR,

    BYTECODE_OP_EQ_STRING,
    BYTECODE_OP_NEQ_STRING,

    BYTECODE_OP_EQ_NIL,
    BYTECODE_OP_EQ_STRING_NIL,
    BYTECODE_OP_EQ_ARRAY_NIL,
    BYTECODE_OP_EQ_RECORD_NIL,
    BYTECODE_OP_EQ_FUNC_NIL,

    BYTECODE_OP_EQ_NIL_STRING,
    BYTECODE_OP_EQ_NIL_ARRAY,
    BYTECODE_OP_EQ_NIL_RECORD,
    BYTECODE_OP_EQ_NIL_FUNC,

    BYTECODE_OP_NEQ_NIL,
    BYTECODE_OP_NEQ_STRING_NIL,
    BYTECODE_OP_NEQ_ARRAY_NIL,
    BYTECODE_OP_NEQ_RECORD_NIL,
    BYTECODE_OP_NEQ_FUNC_NIL,

    BYTECODE_OP_NEQ_NIL_STRING,
    BYTECODE_OP_NEQ_NIL_ARRAY,
    BYTECODE_OP_NEQ_NIL_RECORD,
    BYTECODE_OP_NEQ_NIL_FUNC,

    BYTECODE_OP_NOT_INT,
    
    BYTECODE_OP_INC_INT,
    BYTECODE_OP_DEC_INT,
    BYTECODE_OP_DUP_INT,

    BYTECODE_INT_TO_FLOAT,
    BYTECODE_FLOAT_TO_INT,
    
    BYTECODE_OP_NEG_ARR_INT,
    BYTECODE_OP_NEG_ARR_FLOAT,
    BYTECODE_OP_ADD_ARR_INT,
    BYTECODE_OP_ADD_ARR_FLOAT,
    BYTECODE_OP_SUB_ARR_INT,
    BYTECODE_OP_SUB_ARR_FLOAT,
    BYTECODE_OP_MUL_ARR_INT,
    BYTECODE_OP_MUL_ARR_FLOAT,
    BYTECODE_OP_MUL_ARR_ARR_INT,
    BYTECODE_OP_MUL_ARR_ARR_FLOAT,

    BYTECODE_OP_ASS_INT,
    BYTECODE_OP_ASS_FLOAT,
    BYTECODE_OP_ASS_CHAR,
    BYTECODE_OP_ASS_STRING,
    BYTECODE_OP_ASS_ARRAY,
    BYTECODE_OP_ASS_RECORD,
    BYTECODE_OP_ASS_FUNC,

    BYTECODE_OP_ASS_RECORD_NIL,

    BYTECODE_JUMPZ,
    BYTECODE_JUMP,
    BYTECODE_LABEL,

    BYTECODE_MK_ARRAY_INT,
    BYTECODE_MK_ARRAY_FLOAT,
    BYTECODE_MK_ARRAY_CHAR,
    BYTECODE_MK_ARRAY_STRING,
    BYTECODE_MK_ARRAY_ARRAY,
    BYTECODE_MK_ARRAY_RECORD,
    BYTECODE_MK_ARRAY_FUNC,
    BYTECODE_MK_INIT_ARRAY,
    BYTECODE_MK_RANGE,

    BYTECODE_SLICE_ARRAY,
    BYTECODE_SLICE_RANGE,
    BYTECODE_SLICE_SLICE,
    BYTECODE_SLICE_STRING,

    BYTECODE_ARRAY_DEREF,
    BYTECODE_ARRAYREF_DEREF,
    BYTECODE_RANGE_DEREF,
    BYTECODE_SLICE_DEREF,
    BYTECODE_STRING_DEREF,
    BYTECODE_ARRAY_APPEND,

    BYTECODE_RECORD,
    BYTECODE_VEC_DEREF,
    BYTECODE_VECREF_DEREF,
    BYTECODE_VECREF_VEC_DEREF,
    BYTECODE_NIL_RECORD_REF,

    BYTECODE_FUNC_DEF,
    BYTECODE_FUNC_OBJ,

    BYTECODE_FUNC_FFI,
    BYTECODE_FUNC_FFI_INT,
    BYTECODE_FUNC_FFI_FLOAT,
    BYTECODE_FUNC_FFI_CHAR,
    BYTECODE_FUNC_FFI_STRING,

    BYTECODE_DUP,
    BYTECODE_GLOBAL_VEC,
    BYTECODE_MARK,
    BYTECODE_CALL,
    BYTECODE_SLIDE,
    BYTECODE_CLEAR_STACK,
    BYTECODE_RET,
    BYTECODE_LINE,
    BYTECODE_BUILD_IN,
    BYTECODE_COPYGLOB,
    BYTECODE_ALLOC,
    BYTECODE_REWRITE,
    BYTECODE_PUSH_PARAM,
    BYTECODE_PUSH_EXCEPT,
    BYTECODE_RETHROW,

    BYTECODE_UNHANDLED_EXCEPTION,
    BYTECODE_HALT,
    BYTECODE_END
} bytecode_type;

typedef struct bytecode
{
    bytecode_type type;
    unsigned int addr;
    union {
        struct
        {
            int value; /* BYTECODE_INT */
        } integer;
        struct
        {
            float value; /* BYTECODE_FLOAT */
        } real;
        struct
        {
            char value; /* BYTECODE_CHAR */
        } chr;
        struct
        {
            unsigned int index; /* BYTECODE_STRING */
        } string;
        struct /* BYTECODE_ID_LOCAL BYTECODE_INC_INT */
        {
            int stack_level;
            int index;
        } id_local;
        struct /* BYTECODE_ID_GLOBAL */
        {
            int index;
        } id_global;
        struct
        {
            int stack_level;
            int index;
            int dim_index;
        } id_dim_local;
        struct
        {
            int stack_level;
            int index;
            int dim_index;
        } id_dim_slice;
        struct
        {
            int index;
            int dim_index;
        } id_dim_global;
        union /* BYTECODE_ID_FUNC_FUNC BYTECODE_ID_FUNC_ADDR */
        {
            struct func * func_value;
            unsigned int func_addr;
        } id_func;
        struct /* BYTECODE_JUMPZ BYTECODE_JUMP */
        {
            int offset;
        } jump;
        struct /* BYTECODE_GLOBAL_VEC */
        {
            unsigned int count;
        } global_vec;
        struct /* BYTECODE_MARK */
        {
            unsigned int addr;
        } mark;
        struct /* BYTECODE_SLIDE */
        {
            unsigned int q;
            unsigned int m;
        } slide;
        struct /* BYTECODE_CLEAR_STACK */
        {
            unsigned int count;
        } clear;
        struct /* BYTECODE_LINE */
        {
            unsigned int no;
        } line;
        struct
        {
            unsigned int id;
        } build_in;
        struct
        {
            unsigned int n;
        } alloc;
        struct
        {
            unsigned int j;
        } rewrite;
        struct
        {
            unsigned int dims;
        } mk_array; /* BYTECODE_MK_ARRAY, BYTECODE_MK_INIT_ARRAY */
        struct
        {
            unsigned int dims;
        } array_deref; /* BYTECODE_ARRAYREF_DEREF BYTECODE_RANGE_DEREF BYTECODE_SLICE_DEREF */
        struct
        {
            unsigned int dims;
        } mk_range;
        struct
        {
            unsigned int dims;
        } mk_slice;
        struct
        {
            unsigned int count;
        } record;
        struct
        {
            int stack_level;
            unsigned int index;
        } attr;
        struct
        {
            unsigned int count;
            unsigned int fname_index;
            unsigned int libname_index;
        } ffi;
    };
} bytecode;

typedef struct bytecode_list_node
{
    bytecode value;
    struct bytecode_list_node * prev;
    struct bytecode_list_node * next;
} bytecode_list_node;

typedef struct bytecode_list
{
    unsigned int addr;
    bytecode_list_node * head;
    bytecode_list_node * tail;
} bytecode_list;

typedef struct bytecode_op_str
{
    bytecode_type type;
    void (*print)(bytecode * code);
} bytecode_op_str;

void bytecode_print_unknown(bytecode * code);

void bytecode_print_int(bytecode * code);
void bytecode_print_float(bytecode * code);
void bytecode_print_char(bytecode * code);
void bytecode_print_string(bytecode * code);

void bytecode_print_id_local(bytecode * code);
void bytecode_print_id_dim_local(bytecode * code);
void bytecode_print_id_dim_slice(bytecode * code);
void bytecode_print_id_global(bytecode * code);
void bytecode_print_id_func_func(bytecode * code);
void bytecode_print_id_func_addr(bytecode * code);

void bytecode_print_op_neg_int(bytecode * code);
void bytecode_print_op_add_int(bytecode * code);
void bytecode_print_op_sub_int(bytecode * code);
void bytecode_print_op_mul_int(bytecode * code);
void bytecode_print_op_div_int(bytecode * code);
void bytecode_print_op_mod_int(bytecode * code);

void bytecode_print_op_neg_float(bytecode * code);
void bytecode_print_op_add_float(bytecode * code);
void bytecode_print_op_sub_float(bytecode * code);
void bytecode_print_op_mul_float(bytecode * code);
void bytecode_print_op_div_float(bytecode * code);

void bytecode_print_op_add_string(bytecode * code);
void bytecode_print_op_add_int_string(bytecode * code);
void bytecode_print_op_add_string_int(bytecode * code);
void bytecode_print_op_add_float_string(bytecode * code);
void bytecode_print_op_add_string_float(bytecode * code);

void bytecode_print_op_lt_int(bytecode * code);
void bytecode_print_op_gt_int(bytecode * code);
void bytecode_print_op_lte_int(bytecode * code);
void bytecode_print_op_gte_int(bytecode * code);
void bytecode_print_op_eq_int(bytecode * code);
void bytecode_print_op_neq_int(bytecode * code);

void bytecode_print_op_lt_float(bytecode * code);
void bytecode_print_op_gt_float(bytecode * code);
void bytecode_print_op_lte_float(bytecode * code);
void bytecode_print_op_gte_float(bytecode * code);
void bytecode_print_op_eq_float(bytecode * code);
void bytecode_print_op_neq_float(bytecode * code);

void bytecode_print_op_lt_char(bytecode * code);
void bytecode_print_op_gt_char(bytecode * code);
void bytecode_print_op_lte_char(bytecode * code);
void bytecode_print_op_gte_char(bytecode * code);
void bytecode_print_op_eq_char(bytecode * code);
void bytecode_print_op_neq_char(bytecode * code);

void bytecode_print_op_eq_string(bytecode * code);
void bytecode_print_op_neq_string(bytecode * code);

void bytecode_print_op_eq_nil(bytecode * code);

void bytecode_print_op_eq_string_nil(bytecode * code);
void bytecode_print_op_eq_array_nil(bytecode * code);
void bytecode_print_op_eq_record_nil(bytecode * code);
void bytecode_print_op_eq_func_nil(bytecode * code);

void bytecode_print_op_eq_nil_string(bytecode * code);
void bytecode_print_op_eq_nil_array(bytecode * code);
void bytecode_print_op_eq_nil_record(bytecode * code);
void bytecode_print_op_eq_nil_func(bytecode * code);

void bytecode_print_op_neq_nil(bytecode * code);

void bytecode_print_op_neq_string_nil(bytecode * code);
void bytecode_print_op_neq_array_nil(bytecode * code);
void bytecode_print_op_neq_record_nil(bytecode * code);
void bytecode_print_op_neq_func_nil(bytecode * code);

void bytecode_print_op_neq_nil_string(bytecode * code);
void bytecode_print_op_neq_nil_array(bytecode * code);
void bytecode_print_op_neq_nil_record(bytecode * code);
void bytecode_print_op_neq_nil_func(bytecode * code);

void bytecode_print_op_not_int(bytecode * code);

void bytecode_print_op_inc_int(bytecode * code);
void bytecode_print_op_dec_int(bytecode * code);
void bytecode_print_op_dup_int(bytecode * code);

void bytecode_print_int_to_float(bytecode * code);
void bytecode_print_float_to_int(bytecode * code);

void bytecode_print_op_neg_arr_int(bytecode * code);
void bytecode_print_op_neg_arr_float(bytecode * code);
void bytecode_print_op_add_arr_int(bytecode * code);
void bytecode_print_op_add_arr_float(bytecode * code);
void bytecode_print_op_sub_arr_int(bytecode * code);
void bytecode_print_op_sub_arr_float(bytecode * code);
void bytecode_print_op_mul_arr_int(bytecode * code);
void bytecode_print_op_mul_arr_float(bytecode * code);
void bytecode_print_op_mul_arr_arr_int(bytecode * code);
void bytecode_print_op_mul_arr_arr_float(bytecode * code);

void bytecode_print_op_ass_int(bytecode * code);
void bytecode_print_op_ass_float(bytecode * code);
void bytecode_print_op_ass_char(bytecode * code);
void bytecode_print_op_ass_string(bytecode * code);
void bytecode_print_op_ass_array(bytecode * code);
void bytecode_print_op_ass_func(bytecode * code);
void bytecode_print_op_ass_record(bytecode * code);

void bytecode_print_op_ass_string_nil(bytecode * code);
void bytecode_print_op_ass_array_nil(bytecode * code);
void bytecode_print_op_ass_func_nil(bytecode * code);
void bytecode_print_op_ass_record_nil(bytecode * code);

void bytecode_print_jumpz(bytecode * code);
void bytecode_print_jump(bytecode * code);
void bytecode_print_label(bytecode * code);

void bytecode_print_mk_array_int(bytecode * code);
void bytecode_print_mk_array_float(bytecode * code);
void bytecode_print_mk_array_char(bytecode * code);
void bytecode_print_mk_array_string(bytecode * code);
void bytecode_print_mk_array_array(bytecode * code);
void bytecode_print_mk_array_record(bytecode * code);
void bytecode_print_mk_array_func(bytecode * code);
void bytecode_print_mk_init_array(bytecode * code);
void bytecode_print_mk_range(bytecode * code);

void bytecode_print_slice_array(bytecode * code);
void bytecode_print_slice_range(bytecode * code);
void bytecode_print_slice_slice(bytecode * code);
void bytecode_print_slice_string(bytecode * code);

void bytecode_print_array_deref(bytecode * code);
void bytecode_print_arrayref_deref(bytecode * code);
void bytecode_print_range_deref(bytecode * code);
void bytecode_print_slice_deref(bytecode * code);
void bytecode_print_string_deref(bytecode * code);
void bytecode_print_array_append(bytecode * code);

void bytecode_print_record(bytecode * code);
void bytecode_print_vec_deref(bytecode * code);
void bytecode_print_vecref_deref(bytecode * code);
void bytecode_print_vecref_vec_deref(bytecode * code);
void bytecode_print_nil_record_ref(bytecode * code);

void bytecode_print_func_def(bytecode * code);
void bytecode_print_func_obj(bytecode * code);

void bytecode_print_func_ffi(bytecode * code);
void bytecode_print_func_ffi_int(bytecode * code);
void bytecode_print_func_ffi_float(bytecode * code);
void bytecode_print_func_ffi_char(bytecode * code);
void bytecode_print_func_ffi_string(bytecode * code);

void bytecode_print_dup(bytecode * code);
void bytecode_print_global_vec(bytecode * code);
void bytecode_print_mark(bytecode * code);
void bytecode_print_call(bytecode * code);
void bytecode_print_slide(bytecode * code);
void bytecode_print_clear_stack(bytecode * code);
void bytecode_print_ret(bytecode * code);
void bytecode_print_line(bytecode * code);
void bytecode_print_build_in(bytecode * code);
void bytecode_print_copyglob(bytecode * code);
void bytecode_print_alloc(bytecode * code);
void bytecode_print_rewrite(bytecode * code);
void bytecode_print_push_param(bytecode * code);
void bytecode_print_push_except(bytecode * code);
void bytecode_print_rethrow(bytecode * code);

void bytecode_print_unhandled_exception(bytecode * code);
void bytecode_print_halt(bytecode * code);

bytecode_list_node * bytecode_list_node_new(bytecode * value);
void bytecode_list_node_delete(bytecode_list_node * node);

bytecode_list * bytecode_new();
void bytecode_delete(bytecode_list * code);

bytecode * bytecode_add(bytecode_list * code, bytecode * value);
void bytecode_func_addr(bytecode_list * code);
void bytecode_print(bytecode * code);
void bytecode_list_print(bytecode_list * code);

void bytecode_to_array(bytecode_list * code, bytecode ** code_arr,
                       unsigned int * code_size);
void bytecode_array_delete(bytecode * code_arr);
void bytecode_array_print(bytecode * code_arr, unsigned int size);

#endif /* __BYTECODE_H__ */

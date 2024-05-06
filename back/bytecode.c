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
#include "bytecode.h"
#include "func.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bytecode_op_str bytecode_op[] = {
    { BYTECODE_UNKNOWN, bytecode_print_unknown },

    { BYTECODE_INT, bytecode_print_int },
    { BYTECODE_LONG, bytecode_print_long },
    { BYTECODE_FLOAT, bytecode_print_float },
    { BYTECODE_DOUBLE, bytecode_print_double },
    { BYTECODE_CHAR, bytecode_print_char },
    { BYTECODE_STRING, bytecode_print_string },
    { BYTECODE_C_NULL, bytecode_print_c_null },

    { BYTECODE_ID_TOP, bytecode_print_id_top },
    { BYTECODE_ID_LOCAL, bytecode_print_id_local },
    { BYTECODE_ID_DIM_LOCAL, bytecode_print_id_dim_local },
    { BYTECODE_ID_DIM_SLICE, bytecode_print_id_dim_slice },
    { BYTECODE_ID_GLOBAL, bytecode_print_id_global },
    { BYTECODE_ID_FUNC_FUNC, bytecode_print_id_func_func },
    { BYTECODE_ID_FUNC_ADDR, bytecode_print_id_func_addr },
    { BYTECODE_ID_FUNC_ENTRY, bytecode_print_id_func_entry },

    { BYTECODE_OP_NEG_INT, bytecode_print_op_neg_int },
    { BYTECODE_OP_ADD_INT, bytecode_print_op_add_int },
    { BYTECODE_OP_SUB_INT, bytecode_print_op_sub_int },
    { BYTECODE_OP_MUL_INT, bytecode_print_op_mul_int },
    { BYTECODE_OP_DIV_INT, bytecode_print_op_div_int },
    { BYTECODE_OP_MOD_INT, bytecode_print_op_mod_int },

    { BYTECODE_OP_NEG_LONG, bytecode_print_op_neg_long },
    { BYTECODE_OP_ADD_LONG, bytecode_print_op_add_long },
    { BYTECODE_OP_SUB_LONG, bytecode_print_op_sub_long },
    { BYTECODE_OP_MUL_LONG, bytecode_print_op_mul_long },
    { BYTECODE_OP_DIV_LONG, bytecode_print_op_div_long },
    { BYTECODE_OP_MOD_LONG, bytecode_print_op_mod_long },

    { BYTECODE_OP_NEG_FLOAT, bytecode_print_op_neg_float },
    { BYTECODE_OP_ADD_FLOAT, bytecode_print_op_add_float },
    { BYTECODE_OP_SUB_FLOAT, bytecode_print_op_sub_float },
    { BYTECODE_OP_MUL_FLOAT, bytecode_print_op_mul_float },
    { BYTECODE_OP_DIV_FLOAT, bytecode_print_op_div_float },

    { BYTECODE_OP_NEG_DOUBLE, bytecode_print_op_neg_double },
    { BYTECODE_OP_ADD_DOUBLE, bytecode_print_op_add_double },
    { BYTECODE_OP_SUB_DOUBLE, bytecode_print_op_sub_double },
    { BYTECODE_OP_MUL_DOUBLE, bytecode_print_op_mul_double },
    { BYTECODE_OP_DIV_DOUBLE, bytecode_print_op_div_double },

    { BYTECODE_OP_ADD_STRING, bytecode_print_op_add_string },
    { BYTECODE_OP_ADD_INT_STRING, bytecode_print_op_add_int_string },
    { BYTECODE_OP_ADD_STRING_INT, bytecode_print_op_add_string_int },
    { BYTECODE_OP_ADD_LONG_STRING, bytecode_print_op_add_long_string },
    { BYTECODE_OP_ADD_STRING_LONG, bytecode_print_op_add_string_long },
    { BYTECODE_OP_ADD_FLOAT_STRING, bytecode_print_op_add_float_string },
    { BYTECODE_OP_ADD_STRING_FLOAT, bytecode_print_op_add_string_float },
    { BYTECODE_OP_ADD_DOUBLE_STRING, bytecode_print_op_add_double_string },
    { BYTECODE_OP_ADD_STRING_DOUBLE, bytecode_print_op_add_string_double },
    { BYTECODE_OP_ADD_CHAR_STRING, bytecode_print_op_add_char_string },
    { BYTECODE_OP_ADD_STRING_CHAR, bytecode_print_op_add_string_char },

    { BYTECODE_OP_LT_INT, bytecode_print_op_lt_int },
    { BYTECODE_OP_GT_INT, bytecode_print_op_gt_int },
    { BYTECODE_OP_LTE_INT, bytecode_print_op_lte_int },
    { BYTECODE_OP_GTE_INT, bytecode_print_op_gte_int },
    { BYTECODE_OP_EQ_INT, bytecode_print_op_eq_int },
    { BYTECODE_OP_NEQ_INT, bytecode_print_op_neq_int },

    { BYTECODE_OP_LT_LONG, bytecode_print_op_lt_long },
    { BYTECODE_OP_GT_LONG, bytecode_print_op_gt_long },
    { BYTECODE_OP_LTE_LONG, bytecode_print_op_lte_long },
    { BYTECODE_OP_GTE_LONG, bytecode_print_op_gte_long },
    { BYTECODE_OP_EQ_LONG, bytecode_print_op_eq_long },
    { BYTECODE_OP_NEQ_LONG, bytecode_print_op_neq_long },

    { BYTECODE_OP_LT_FLOAT, bytecode_print_op_lt_float },
    { BYTECODE_OP_GT_FLOAT, bytecode_print_op_gt_float },
    { BYTECODE_OP_LTE_FLOAT, bytecode_print_op_lte_float },
    { BYTECODE_OP_GTE_FLOAT, bytecode_print_op_gte_float },
    { BYTECODE_OP_EQ_FLOAT, bytecode_print_op_eq_float },
    { BYTECODE_OP_NEQ_FLOAT, bytecode_print_op_neq_float },

    { BYTECODE_OP_LT_DOUBLE, bytecode_print_op_lt_double },
    { BYTECODE_OP_GT_DOUBLE, bytecode_print_op_gt_double },
    { BYTECODE_OP_LTE_DOUBLE, bytecode_print_op_lte_double },
    { BYTECODE_OP_GTE_DOUBLE, bytecode_print_op_gte_double },
    { BYTECODE_OP_EQ_DOUBLE, bytecode_print_op_eq_double },
    { BYTECODE_OP_NEQ_DOUBLE, bytecode_print_op_neq_double },

    { BYTECODE_OP_LT_CHAR, bytecode_print_op_lt_char },
    { BYTECODE_OP_GT_CHAR, bytecode_print_op_gt_char },
    { BYTECODE_OP_LTE_CHAR, bytecode_print_op_lte_char },
    { BYTECODE_OP_GTE_CHAR, bytecode_print_op_gte_char },
    { BYTECODE_OP_EQ_CHAR, bytecode_print_op_eq_char },
    { BYTECODE_OP_NEQ_CHAR, bytecode_print_op_neq_char },

    { BYTECODE_OP_EQ_STRING, bytecode_print_op_eq_string },
    { BYTECODE_OP_NEQ_STRING, bytecode_print_op_neq_string },

    { BYTECODE_OP_EQ_C_PTR, bytecode_print_op_eq_c_ptr },
    { BYTECODE_OP_NEQ_C_PTR, bytecode_print_op_neq_c_ptr },

    { BYTECODE_OP_EQ_NIL, bytecode_print_op_eq_nil },

    { BYTECODE_OP_EQ_STRING_NIL, bytecode_print_op_eq_string_nil },
    { BYTECODE_OP_EQ_ARRAY_NIL, bytecode_print_op_eq_array_nil },
    { BYTECODE_OP_EQ_RECORD_NIL, bytecode_print_op_eq_record_nil },
    { BYTECODE_OP_EQ_FUNC_NIL, bytecode_print_op_eq_func_nil },

    { BYTECODE_OP_EQ_NIL_STRING, bytecode_print_op_eq_nil_string },
    { BYTECODE_OP_EQ_NIL_ARRAY, bytecode_print_op_eq_nil_array },
    { BYTECODE_OP_EQ_NIL_RECORD, bytecode_print_op_eq_nil_record },
    { BYTECODE_OP_EQ_NIL_FUNC, bytecode_print_op_eq_nil_func },

    { BYTECODE_OP_NEQ_NIL, bytecode_print_op_neq_nil },

    { BYTECODE_OP_NEQ_STRING_NIL, bytecode_print_op_neq_string_nil },
    { BYTECODE_OP_NEQ_ARRAY_NIL, bytecode_print_op_neq_array_nil },
    { BYTECODE_OP_NEQ_RECORD_NIL, bytecode_print_op_neq_record_nil },
    { BYTECODE_OP_NEQ_FUNC_NIL, bytecode_print_op_neq_func_nil },

    { BYTECODE_OP_NEQ_NIL_STRING, bytecode_print_op_neq_nil_string },
    { BYTECODE_OP_NEQ_NIL_ARRAY, bytecode_print_op_neq_nil_array },
    { BYTECODE_OP_NEQ_NIL_RECORD, bytecode_print_op_neq_nil_record },
    { BYTECODE_OP_NEQ_NIL_FUNC, bytecode_print_op_neq_nil_func },

    { BYTECODE_OP_NOT_INT, bytecode_print_op_not_int },

    { BYTECODE_OP_BIN_NOT_INT, bytecode_print_op_bin_not_int },
    { BYTECODE_OP_BIN_AND_INT, bytecode_print_op_bin_and_int },
    { BYTECODE_OP_BIN_OR_INT, bytecode_print_op_bin_or_int },
    { BYTECODE_OP_BIN_XOR_INT, bytecode_print_op_bin_xor_int },
    { BYTECODE_OP_BIN_SHL_INT, bytecode_print_op_bin_shl_int },
    { BYTECODE_OP_BIN_SHR_INT, bytecode_print_op_bin_shr_int },

    { BYTECODE_OP_BIN_NOT_LONG, bytecode_print_op_bin_not_long },
    { BYTECODE_OP_BIN_AND_LONG, bytecode_print_op_bin_and_long },
    { BYTECODE_OP_BIN_OR_LONG, bytecode_print_op_bin_or_long },
    { BYTECODE_OP_BIN_XOR_LONG, bytecode_print_op_bin_xor_long },
    { BYTECODE_OP_BIN_SHL_LONG, bytecode_print_op_bin_shl_long },
    { BYTECODE_OP_BIN_SHR_LONG, bytecode_print_op_bin_shr_long },

    { BYTECODE_OP_INC_INT, bytecode_print_op_inc_int },
    { BYTECODE_OP_DEC_INT, bytecode_print_op_dec_int },
    { BYTECODE_OP_DUP_INT, bytecode_print_op_dup_int },

    { BYTECODE_INT_TO_LONG, bytecode_print_int_to_long },
    { BYTECODE_INT_TO_FLOAT, bytecode_print_int_to_float },
    { BYTECODE_INT_TO_DOUBLE, bytecode_print_int_to_double },
    { BYTECODE_LONG_TO_INT, bytecode_print_long_to_int },
    { BYTECODE_LONG_TO_FLOAT, bytecode_print_long_to_float },
    { BYTECODE_LONG_TO_DOUBLE, bytecode_print_long_to_double },
    { BYTECODE_FLOAT_TO_INT, bytecode_print_float_to_int },
    { BYTECODE_FLOAT_TO_LONG, bytecode_print_float_to_long },
    { BYTECODE_FLOAT_TO_DOUBLE, bytecode_print_float_to_double },
    { BYTECODE_DOUBLE_TO_INT, bytecode_print_double_to_int },
    { BYTECODE_DOUBLE_TO_LONG, bytecode_print_double_to_long },
    { BYTECODE_DOUBLE_TO_FLOAT, bytecode_print_double_to_float },
    { BYTECODE_ENUMTYPE_RECORD_TO_INT, bytecode_print_enumtype_record_to_int },

    { BYTECODE_OP_NEG_ARR_INT, bytecode_print_op_neg_arr_int },
    { BYTECODE_OP_NEG_ARR_LONG, bytecode_print_op_neg_arr_long },
    { BYTECODE_OP_NEG_ARR_FLOAT, bytecode_print_op_neg_arr_float },
    { BYTECODE_OP_NEG_ARR_DOUBLE, bytecode_print_op_neg_arr_double },
    { BYTECODE_OP_ADD_ARR_INT, bytecode_print_op_add_arr_int },
    { BYTECODE_OP_ADD_ARR_LONG, bytecode_print_op_add_arr_long },
    { BYTECODE_OP_ADD_ARR_FLOAT, bytecode_print_op_add_arr_float },
    { BYTECODE_OP_ADD_ARR_DOUBLE, bytecode_print_op_add_arr_double },
    { BYTECODE_OP_SUB_ARR_INT, bytecode_print_op_sub_arr_int },
    { BYTECODE_OP_SUB_ARR_LONG, bytecode_print_op_sub_arr_long },
    { BYTECODE_OP_SUB_ARR_FLOAT, bytecode_print_op_sub_arr_float },
    { BYTECODE_OP_SUB_ARR_DOUBLE, bytecode_print_op_sub_arr_double },
    { BYTECODE_OP_MUL_ARR_INT, bytecode_print_op_mul_arr_int },
    { BYTECODE_OP_MUL_ARR_LONG, bytecode_print_op_mul_arr_long },
    { BYTECODE_OP_MUL_ARR_FLOAT, bytecode_print_op_mul_arr_float },
    { BYTECODE_OP_MUL_ARR_DOUBLE, bytecode_print_op_mul_arr_double },
    { BYTECODE_OP_MUL_ARR_ARR_INT, bytecode_print_op_mul_arr_arr_int },
    { BYTECODE_OP_MUL_ARR_ARR_LONG, bytecode_print_op_mul_arr_arr_long },
    { BYTECODE_OP_MUL_ARR_ARR_FLOAT, bytecode_print_op_mul_arr_arr_float },
    { BYTECODE_OP_MUL_ARR_ARR_DOUBLE, bytecode_print_op_mul_arr_arr_double },

    { BYTECODE_OP_ASS_INT, bytecode_print_op_ass_int },
    { BYTECODE_OP_ASS_LONG, bytecode_print_op_ass_long },
    { BYTECODE_OP_ASS_FLOAT, bytecode_print_op_ass_float },
    { BYTECODE_OP_ASS_DOUBLE, bytecode_print_op_ass_double },
    { BYTECODE_OP_ASS_CHAR, bytecode_print_op_ass_char },
    { BYTECODE_OP_ASS_STRING, bytecode_print_op_ass_string },
    { BYTECODE_OP_ASS_C_PTR, bytecode_print_op_ass_c_ptr },
    { BYTECODE_OP_ASS_ARRAY, bytecode_print_op_ass_array },
    { BYTECODE_OP_ASS_RECORD, bytecode_print_op_ass_record },
    { BYTECODE_OP_ASS_FUNC, bytecode_print_op_ass_func },

    { BYTECODE_OP_ASS_RECORD_NIL, bytecode_print_op_ass_record_nil },

    { BYTECODE_JUMPZ, bytecode_print_jumpz },
    { BYTECODE_JUMP, bytecode_print_jump },
    { BYTECODE_LABEL, bytecode_print_label },

    { BYTECODE_MK_ARRAY_INT, bytecode_print_mk_array_int },
    { BYTECODE_MK_ARRAY_LONG, bytecode_print_mk_array_long },
    { BYTECODE_MK_ARRAY_FLOAT, bytecode_print_mk_array_float },
    { BYTECODE_MK_ARRAY_DOUBLE, bytecode_print_mk_array_double },
    { BYTECODE_MK_ARRAY_CHAR, bytecode_print_mk_array_char },
    { BYTECODE_MK_ARRAY_STRING, bytecode_print_mk_array_string },
    { BYTECODE_MK_ARRAY_ARRAY, bytecode_print_mk_array_array },
    { BYTECODE_MK_ARRAY_RECORD, bytecode_print_mk_array_record },
    { BYTECODE_MK_ARRAY_FUNC, bytecode_print_mk_array_func },
    { BYTECODE_MK_INIT_ARRAY, bytecode_print_mk_init_array },
    { BYTECODE_MK_RANGE, bytecode_print_mk_range },

    { BYTECODE_SLICE_ARRAY, bytecode_print_slice_array },
    { BYTECODE_SLICE_RANGE, bytecode_print_slice_range },
    { BYTECODE_SLICE_SLICE, bytecode_print_slice_slice },
    { BYTECODE_SLICE_STRING, bytecode_print_slice_string },

    { BYTECODE_ARRAY_DEREF, bytecode_print_array_deref },
    { BYTECODE_ARRAYREF_DEREF, bytecode_print_arrayref_deref },
    { BYTECODE_RANGE_DEREF, bytecode_print_range_deref },
    { BYTECODE_SLICE_DEREF, bytecode_print_slice_deref },
    { BYTECODE_STRING_DEREF, bytecode_print_string_deref },
    { BYTECODE_ARRAY_APPEND, bytecode_print_array_append },

    { BYTECODE_RECORD, bytecode_print_record },
    { BYTECODE_VEC_DEREF, bytecode_print_vec_deref },
    { BYTECODE_VECREF_DEREF, bytecode_print_vecref_deref },
    { BYTECODE_VECREF_VEC_DEREF, bytecode_print_vecref_vec_deref },
    { BYTECODE_VECREF_VEC_INDEX_DEREF, bytecode_print_vecref_vec_index_deref },
    { BYTECODE_RECORD_UNPACK, bytecode_print_record_unpack },
    { BYTECODE_NIL_RECORD_REF, bytecode_print_nil_record_ref },

    { BYTECODE_FUNC_DEF, bytecode_print_func_def },
    { BYTECODE_FUNC_OBJ, bytecode_print_func_obj },
    { BYTECODE_FUNC_FFI, bytecode_print_func_ffi },
    { BYTECODE_FUNC_FFI_BOOL, bytecode_print_func_ffi_bool },
    { BYTECODE_FUNC_FFI_INT, bytecode_print_func_ffi_int },
    { BYTECODE_FUNC_FFI_LONG, bytecode_print_func_ffi_long },
    { BYTECODE_FUNC_FFI_FLOAT, bytecode_print_func_ffi_float },
    { BYTECODE_FUNC_FFI_DOUBLE, bytecode_print_func_ffi_double },
    { BYTECODE_FUNC_FFI_CHAR, bytecode_print_func_ffi_char },
    { BYTECODE_FUNC_FFI_STRING, bytecode_print_func_ffi_string },
    { BYTECODE_FUNC_FFI_VOID, bytecode_print_func_ffi_void },
    { BYTECODE_FUNC_FFI_C_PTR, bytecode_print_func_ffi_c_ptr },
    { BYTECODE_FUNC_FFI_RECORD, bytecode_print_func_ffi_record },

    { BYTECODE_DUP, bytecode_print_dup },    
    { BYTECODE_GLOBAL_VEC, bytecode_print_global_vec },

    { BYTECODE_MARK, bytecode_print_mark },
    { BYTECODE_CALL, bytecode_print_call },
    { BYTECODE_SLIDE, bytecode_print_slide },
    { BYTECODE_CLEAR_STACK, bytecode_print_clear_stack },
    { BYTECODE_RET, bytecode_print_ret },
    { BYTECODE_LINE, bytecode_print_line },
    { BYTECODE_BUILD_IN, bytecode_print_build_in },
    { BYTECODE_COPYGLOB, bytecode_print_copyglob },
    { BYTECODE_ALLOC, bytecode_print_alloc },
    { BYTECODE_REWRITE, bytecode_print_rewrite },
    { BYTECODE_PUSH_PARAM, bytecode_print_push_param },
    { BYTECODE_PUSH_EXCEPT, bytecode_print_push_except },
    { BYTECODE_RETHROW, bytecode_print_rethrow },

    { BYTECODE_UNHANDLED_EXCEPTION, bytecode_print_unhandled_exception },
    { BYTECODE_HALT, bytecode_print_halt }
};

static void bytecode_op_test()
{
    unsigned int i = 0;
    for (i = 0; i < BYTECODE_END; i++)
    {
        assert(i == bytecode_op[i].type);
    }
}

void bytecode_print_unknown(bytecode * code)
{
    printf("%d: unknown\n", code->addr);
}

void bytecode_print_int(bytecode * code)
{
    printf("%d: int %d\n", code->addr, code->int_t.value);
}

void bytecode_print_long(bytecode * code)
{
    printf("%d: long %lld\n", code->addr, code->long_t.value);
}

void bytecode_print_float(bytecode * code)
{
    printf("%d: float %f\n", code->addr, code->float_t.value);
}

void bytecode_print_double(bytecode * code)
{
    printf("%d: double %lf\n", code->addr, code->double_t.value);
}

void bytecode_print_char(bytecode * code)
{
    printf("%d: char %c\n", code->addr, code->char_t.value);
}

void bytecode_print_string(bytecode * code)
{
    printf("%d: string %u\n", code->addr, code->string.index);
}

void bytecode_print_c_null(bytecode * code)
{
    printf("%d: c_null\n", code->addr);
}

void bytecode_print_id_top(bytecode * code)
{
    printf("%d: id top %d\n", code->addr, code->id_top.index);
}

void bytecode_print_id_local(bytecode * code)
{
    printf("%d: id local %d %d\n", code->addr, code->id_local.stack_level,
           code->id_local.index);
}

void bytecode_print_id_dim_local(bytecode * code)
{
    printf("%d: id dim local %d %d %d\n", code->addr,
           code->id_dim_local.stack_level, code->id_dim_local.index,
           code->id_dim_local.dim_index);
}

void bytecode_print_id_dim_slice(bytecode * code)
{
    printf("%d: id dim slice %d %d %d\n", code->addr,
           code->id_dim_slice.stack_level, code->id_dim_slice.index,
           code->id_dim_slice.dim_index);
}

void bytecode_print_id_global(bytecode * code)
{
    printf("%d: id global %d\n", code->addr, code->id_global.index);
}

void bytecode_print_id_func_func(bytecode * code)
{
    printf("%d: id func func %d\n", code->addr, code->id_func.func_value ? code->id_func.func_value->addr : 0);
}

void bytecode_print_id_func_addr(bytecode * code)
{
    printf("%d: id func addr %d\n", code->addr, code->id_func.func_addr);
}

void bytecode_print_id_func_entry(bytecode * code)
{
    printf("%d: id func entry\n", code->addr);    
}

void bytecode_print_op_neg_int(bytecode * code)
{
    printf("%d: op neg int\n", code->addr);
}

void bytecode_print_op_add_int(bytecode * code)
{
    printf("%d: op add int\n", code->addr);
}

void bytecode_print_op_sub_int(bytecode * code)
{
    printf("%d: op sub int\n", code->addr);
}

void bytecode_print_op_mul_int(bytecode * code)
{
    printf("%d: op mul int\n", code->addr);
}

void bytecode_print_op_div_int(bytecode * code)
{
    printf("%d: op div int\n", code->addr);
}

void bytecode_print_op_mod_int(bytecode * code)
{
    printf("%d: op mod int\n", code->addr);
}

void bytecode_print_op_neg_long(bytecode * code)
{
    printf("%d: op neg long\n", code->addr);
}

void bytecode_print_op_add_long(bytecode * code)
{
    printf("%d: op add long\n", code->addr);
}

void bytecode_print_op_sub_long(bytecode * code)
{
    printf("%d: op sub long\n", code->addr);
}

void bytecode_print_op_mul_long(bytecode * code)
{
    printf("%d: op mul long\n", code->addr);
}

void bytecode_print_op_div_long(bytecode * code)
{
    printf("%d: op div long\n", code->addr);
}

void bytecode_print_op_mod_long(bytecode * code)
{
    printf("%d: op mod long\n", code->addr);
}

void bytecode_print_op_neg_float(bytecode * code)
{
    printf("%d: op neg float\n", code->addr);
}

void bytecode_print_op_add_float(bytecode * code)
{
    printf("%d: op add float\n", code->addr);
}

void bytecode_print_op_sub_float(bytecode * code)
{
    printf("%d: op sub float\n", code->addr);
}

void bytecode_print_op_mul_float(bytecode * code)
{
    printf("%d: op mul float\n", code->addr);
}

void bytecode_print_op_div_float(bytecode * code)
{
    printf("%d: op div float\n", code->addr);
}

void bytecode_print_op_neg_double(bytecode * code)
{
    printf("%d: op neg double\n", code->addr);
}

void bytecode_print_op_add_double(bytecode * code)
{
    printf("%d: op add double\n", code->addr);
}

void bytecode_print_op_sub_double(bytecode * code)
{
    printf("%d: op sub double\n", code->addr);
}

void bytecode_print_op_mul_double(bytecode * code)
{
    printf("%d: op mul double\n", code->addr);
}

void bytecode_print_op_div_double(bytecode * code)
{
    printf("%d: op div double\n", code->addr);
}

void bytecode_print_op_add_string(bytecode * code)
{
    printf("%d: op add string\n", code->addr);
}

void bytecode_print_op_add_int_string(bytecode * code)
{
    printf("%d: op add int string\n", code->addr);
}

void bytecode_print_op_add_string_int(bytecode * code)
{
    printf("%d: op add string int\n", code->addr);
}

void bytecode_print_op_add_long_string(bytecode * code)
{
    printf("%d: op add long string\n", code->addr);
}

void bytecode_print_op_add_string_long(bytecode * code)
{
    printf("%d: op add string long\n", code->addr);
}

void bytecode_print_op_add_float_string(bytecode * code)
{
    printf("%d: op add float string\n", code->addr);
}

void bytecode_print_op_add_string_float(bytecode * code)
{
    printf("%d: op add string float\n", code->addr);
}

void bytecode_print_op_add_double_string(bytecode * code)
{
    printf("%d: op add double string\n", code->addr);
}

void bytecode_print_op_add_string_double(bytecode * code)
{
    printf("%d: op add string double\n", code->addr);
}

void bytecode_print_op_add_char_string(bytecode * code)
{
    printf("%d: op add char string\n", code->addr);
}

void bytecode_print_op_add_string_char(bytecode * code)
{
    printf("%d: op add string char\n", code->addr);
}

void bytecode_print_op_lt_int(bytecode * code)
{
    printf("%d: op lt int\n", code->addr);
}

void bytecode_print_op_gt_int(bytecode * code)
{
    printf("%d: op gt int\n", code->addr);
}

void bytecode_print_op_lte_int(bytecode * code)
{
    printf("%d: op lte int\n", code->addr);
}

void bytecode_print_op_gte_int(bytecode * code)
{
    printf("%d: op gte int\n", code->addr);
}

void bytecode_print_op_eq_int(bytecode * code)
{
    printf("%d: op eq int\n", code->addr);
}

void bytecode_print_op_neq_int(bytecode * code)
{
    printf("%d: op neq int\n", code->addr);
}

void bytecode_print_op_lt_long(bytecode * code)
{
    printf("%d: op lt long\n", code->addr);
}

void bytecode_print_op_gt_long(bytecode * code)
{
    printf("%d: op gt long\n", code->addr);
}

void bytecode_print_op_lte_long(bytecode * code)
{
    printf("%d: op lte long\n", code->addr);
}

void bytecode_print_op_gte_long(bytecode * code)
{
    printf("%d: op gte long\n", code->addr);
}

void bytecode_print_op_eq_long(bytecode * code)
{
    printf("%d: op eq long\n", code->addr);
}

void bytecode_print_op_neq_long(bytecode * code)
{
    printf("%d: op neq long\n", code->addr);
}

void bytecode_print_op_lt_float(bytecode * code)
{
    printf("%d: op lt float\n", code->addr);
}

void bytecode_print_op_gt_float(bytecode * code)
{
    printf("%d: op gt float\n", code->addr);
}

void bytecode_print_op_lte_float(bytecode * code)
{
    printf("%d: op lte float\n", code->addr);
}

void bytecode_print_op_gte_float(bytecode * code)
{
    printf("%d: op gte float\n", code->addr);
}

void bytecode_print_op_eq_float(bytecode * code)
{
    printf("%d: op eq float\n", code->addr);
}

void bytecode_print_op_neq_float(bytecode * code)
{
    printf("%d: op neq float\n", code->addr);
}

void bytecode_print_op_lt_double(bytecode * code)
{
    printf("%d: op lt double\n", code->addr);
}

void bytecode_print_op_gt_double(bytecode * code)
{
    printf("%d: op gt double\n", code->addr);
}

void bytecode_print_op_lte_double(bytecode * code)
{
    printf("%d: op lte double\n", code->addr);
}

void bytecode_print_op_gte_double(bytecode * code)
{
    printf("%d: op gte double\n", code->addr);
}

void bytecode_print_op_eq_double(bytecode * code)
{
    printf("%d: op eq double\n", code->addr);
}

void bytecode_print_op_neq_double(bytecode * code)
{
    printf("%d: op neq double\n", code->addr);
}

void bytecode_print_op_lt_char(bytecode * code)
{
    printf("%d: op lt char\n", code->addr);
}

void bytecode_print_op_gt_char(bytecode * code)
{
    printf("%d: op gt char\n", code->addr);
}

void bytecode_print_op_lte_char(bytecode * code)
{
    printf("%d: op lte char\n", code->addr);
}

void bytecode_print_op_gte_char(bytecode * code)
{
    printf("%d: op gte char\n", code->addr);
}

void bytecode_print_op_eq_char(bytecode * code)
{
    printf("%d: op eq char\n", code->addr);
}

void bytecode_print_op_neq_char(bytecode * code)
{
    printf("%d: op neq char\n", code->addr);
}

void bytecode_print_op_eq_string(bytecode * code)
{
    printf("%d: op eq string\n", code->addr);
}

void bytecode_print_op_neq_string(bytecode * code)
{
    printf("%d: op neq string\n", code->addr);
}

void bytecode_print_op_eq_c_ptr(bytecode * code)
{
    printf("%d: op eq c_ptr\n", code->addr);
}

void bytecode_print_op_neq_c_ptr(bytecode * code)
{
    printf("%d: op neq c_ptr\n", code->addr);
}

void bytecode_print_op_eq_nil(bytecode * code)
{
    printf("%d: op eq nil\n", code->addr);
}

void bytecode_print_op_eq_string_nil(bytecode * code)
{
    printf("%d: op eq string nil\n", code->addr);
}

void bytecode_print_op_eq_array_nil(bytecode * code)
{
    printf("%d: op eq array nil\n", code->addr);
}

void bytecode_print_op_eq_record_nil(bytecode * code)
{
    printf("%d: op eq record nil\n", code->addr);
}

void bytecode_print_op_eq_func_nil(bytecode * code)
{
    printf("%d: op eq func nil\n", code->addr);
}

void bytecode_print_op_eq_nil_string(bytecode * code)
{
    printf("%d: op eq nil string\n", code->addr);
}

void bytecode_print_op_eq_nil_array(bytecode * code)
{
    printf("%d: op eq nil array\n", code->addr);
}

void bytecode_print_op_eq_nil_record(bytecode * code)
{
    printf("%d: op eq nil record\n", code->addr);
}

void bytecode_print_op_eq_nil_func(bytecode * code)
{
    printf("%d: op eq nil func\n", code->addr);
}

void bytecode_print_op_neq_nil(bytecode * code)
{
    printf("%d: op neq nil\n", code->addr);
}

void bytecode_print_op_neq_string_nil(bytecode * code)
{
    printf("%d: op neq string nil\n", code->addr);
}

void bytecode_print_op_neq_array_nil(bytecode * code)
{
    printf("%d: op neq array nil\n", code->addr);
}

void bytecode_print_op_neq_record_nil(bytecode * code)
{
    printf("%d: op neq record nil\n", code->addr);
}

void bytecode_print_op_neq_func_nil(bytecode * code)
{
    printf("%d: op neq func nil\n", code->addr);
}

void bytecode_print_op_neq_nil_string(bytecode * code)
{
    printf("%d: op neq nil string\n", code->addr);
}

void bytecode_print_op_neq_nil_array(bytecode * code)
{
    printf("%d: op neq nil array\n", code->addr);
}

void bytecode_print_op_neq_nil_record(bytecode * code)
{
    printf("%d: op neq nil record\n", code->addr);
}

void bytecode_print_op_neq_nil_func(bytecode * code)
{
    printf("%d: op neq nil func\n", code->addr);
}

void bytecode_print_op_not_int(bytecode * code)
{
    printf("%d: op not\n", code->addr);
}

void bytecode_print_op_bin_not_int(bytecode * code)
{
    printf("%d: op bin not int\n", code->addr);
}

void bytecode_print_op_bin_and_int(bytecode * code)
{
    printf("%d: op bin and int\n", code->addr);
}

void bytecode_print_op_bin_or_int(bytecode * code)
{
    printf("%d: op bin or int\n", code->addr);
}

void bytecode_print_op_bin_xor_int(bytecode * code)
{
    printf("%d: op bin xor int\n", code->addr);
}

void bytecode_print_op_bin_shl_int(bytecode * code)
{
    printf("%d: op bin shl int\n", code->addr);
}

void bytecode_print_op_bin_shr_int(bytecode * code)
{
    printf("%d: op bin shr int\n", code->addr);
}

void bytecode_print_op_bin_not_long(bytecode * code)
{
    printf("%d: op bin not long\n", code->addr);
}

void bytecode_print_op_bin_and_long(bytecode * code)
{
    printf("%d: op bin and long\n", code->addr);
}

void bytecode_print_op_bin_or_long(bytecode * code)
{
    printf("%d: op bin or long\n", code->addr);
}

void bytecode_print_op_bin_xor_long(bytecode * code)
{
    printf("%d: op bin xor long\n", code->addr);
}

void bytecode_print_op_bin_shl_long(bytecode * code)
{
    printf("%d: op bin shl long\n", code->addr);
}

void bytecode_print_op_bin_shr_long(bytecode * code)
{
    printf("%d: op bin shr long\n", code->addr);
}

void bytecode_print_op_inc_int(bytecode * code)
{
    printf("%d: op inc int %d %d\n", code->addr, 
                                     code->id_local.stack_level,
                                     code->id_local.index);
}

void bytecode_print_op_dec_int(bytecode * code)
{
    printf("%d: op dec int %d %d\n", code->addr, 
                                     code->id_local.stack_level,
                                     code->id_local.index);
}

void bytecode_print_op_dup_int(bytecode * code)
{
    printf("%d: op dup int %d %d\n", code->addr, 
                                     code->id_local.stack_level,
                                     code->id_local.index);
}

void bytecode_print_int_to_long(bytecode * code)
{
    printf("%d: int to long\n", code->addr);
}

void bytecode_print_int_to_float(bytecode * code)
{
    printf("%d: int to float\n", code->addr);
}

void bytecode_print_int_to_double(bytecode * code)
{
    printf("%d: int to double\n", code->addr);
}

void bytecode_print_long_to_int(bytecode * code)
{
    printf("%d: long to int\n", code->addr);
}

void bytecode_print_long_to_float(bytecode * code)
{
    printf("%d: long to float\n", code->addr);
}

void bytecode_print_long_to_double(bytecode * code)
{
    printf("%d: long to double\n", code->addr);
}

void bytecode_print_float_to_int(bytecode * code)
{
    printf("%d: float to int\n", code->addr);
}

void bytecode_print_float_to_long(bytecode * code)
{
    printf("%d: float to long\n", code->addr);    
}

void bytecode_print_float_to_double(bytecode * code)
{
    printf("%d: float to double\n", code->addr);
}

void bytecode_print_double_to_int(bytecode * code)
{
    printf("%d: double to int\n", code->addr);
}

void bytecode_print_double_to_long(bytecode * code)
{
    printf("%d: double to long\n", code->addr);
}

void bytecode_print_double_to_float(bytecode * code)
{
    printf("%d: double to float\n", code->addr);
}

void bytecode_print_enumtype_record_to_int(bytecode * code)
{
    printf("%d: enumtype record to int\n", code->addr);
}

void bytecode_print_op_neg_arr_int(bytecode * code)
{
    printf("%d: op neg arr int\n", code->addr);
}

void bytecode_print_op_neg_arr_long(bytecode * code)
{
    printf("%d: op neg arr long\n", code->addr);
}

void bytecode_print_op_neg_arr_float(bytecode * code)
{
    printf("%d: op neg arr float\n", code->addr);
}

void bytecode_print_op_neg_arr_double(bytecode * code)
{
    printf("%d: op neg arr double\n", code->addr);
}

void bytecode_print_op_add_arr_int(bytecode * code)
{
    printf("%d: op add arr int\n", code->addr);
}

void bytecode_print_op_add_arr_long(bytecode * code)
{
    printf("%d: op add arr long\n", code->addr);
}

void bytecode_print_op_add_arr_float(bytecode * code)
{
    printf("%d: op add arr float\n", code->addr);
}

void bytecode_print_op_add_arr_double(bytecode * code)
{
    printf("%d: op add arr double\n", code->addr);
}

void bytecode_print_op_sub_arr_int(bytecode * code)
{
    printf("%d: op sub arr int\n", code->addr);
}

void bytecode_print_op_sub_arr_long(bytecode * code)
{
    printf("%d: op sub arr long\n", code->addr);
}

void bytecode_print_op_sub_arr_float(bytecode * code)
{
    printf("%d: op sub arr float\n", code->addr);
}

void bytecode_print_op_sub_arr_double(bytecode * code)
{
    printf("%d: op sub arr double\n", code->addr);
}

void bytecode_print_op_mul_arr_int(bytecode * code)
{
    printf("%d: op mul arr int\n", code->addr);
}

void bytecode_print_op_mul_arr_long(bytecode * code)
{
    printf("%d: op mul arr long\n", code->addr);
}

void bytecode_print_op_mul_arr_float(bytecode * code)
{
    printf("%d: op mul arr float\n", code->addr);
}

void bytecode_print_op_mul_arr_double(bytecode * code)
{
    printf("%d: op mul arr double\n", code->addr);
}

void bytecode_print_op_mul_arr_arr_int(bytecode * code)
{
    printf("%d: op mul arr arr int\n", code->addr);
}

void bytecode_print_op_mul_arr_arr_long(bytecode * code)
{
    printf("%d: op mul arr arr long\n", code->addr);
}

void bytecode_print_op_mul_arr_arr_float(bytecode * code)
{
    printf("%d: op mul arr arr float\n", code->addr);
}

void bytecode_print_op_mul_arr_arr_double(bytecode * code)
{
    printf("%d: op mul arr arr double\n", code->addr);
}

void bytecode_print_op_ass_int(bytecode * code)
{
    printf("%d: op ass int\n", code->addr);
}

void bytecode_print_op_ass_long(bytecode * code)
{
    printf("%d: op ass long\n", code->addr);
}

void bytecode_print_op_ass_float(bytecode * code)
{
    printf("%d: op ass float\n", code->addr);
}

void bytecode_print_op_ass_double(bytecode * code)
{
    printf("%d: op ass double\n", code->addr);
}

void bytecode_print_op_ass_char(bytecode * code)
{
    printf("%d: op ass char\n", code->addr);
}

void bytecode_print_op_ass_string(bytecode * code)
{
    printf("%d: op ass string\n", code->addr);
}

void bytecode_print_op_ass_c_ptr(bytecode * code)
{
    printf("%d: op ass c_ptr\n", code->addr);
}

void bytecode_print_op_ass_array(bytecode * code)
{
    printf("%d: op ass array\n", code->addr);
}

void bytecode_print_op_ass_record(bytecode * code)
{
    printf("%d: op ass record\n", code->addr);
}

void bytecode_print_op_ass_func(bytecode * code)
{
    printf("%d: op ass func\n", code->addr);
}

void bytecode_print_op_ass_record_nil(bytecode * code)
{
    printf("%d: op ass record nil\n", code->addr);
}

void bytecode_print_jumpz(bytecode * code)
{
    printf("%d: jumpz %d (%d)\n", code->addr, code->jump.offset,
           code->addr + code->jump.offset);
}

void bytecode_print_jump(bytecode * code)
{
    printf("%d: jump %d (%d)\n", code->addr, code->jump.offset,
           code->addr + code->jump.offset);
}

void bytecode_print_label(bytecode * code)
{
    printf("%d: label\n", code->addr);
}

void bytecode_print_mk_array_int(bytecode * code)
{
    printf("%d: mk array int %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_long(bytecode * code)
{
    printf("%d: mk array long %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_float(bytecode * code)
{
    printf("%d: mk array float %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_double(bytecode * code)
{
    printf("%d: mk array double %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_char(bytecode * code)
{
    printf("%d: mk array char %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_string(bytecode * code)
{
    printf("%d: mk array string %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_array(bytecode * code)
{
    printf("%d: mk array array %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_record(bytecode * code)
{
    printf("%d: mk array record %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_array_func(bytecode * code)
{
    printf("%d: mk array func %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_init_array(bytecode * code)
{
    printf("%d: mk init array %d\n", code->addr, code->mk_array.dims);
}

void bytecode_print_mk_range(bytecode * code)
{
    printf("%d: mk range %u\n", code->addr, code->mk_range.dims);
}

void bytecode_print_slice_array(bytecode * code)
{
    printf("%d: slice array %u\n", code->addr, code->mk_slice.dims);
}

void bytecode_print_slice_range(bytecode * code)
{
    printf("%d: slice range %u\n", code->addr, code->mk_slice.dims);
}

void bytecode_print_slice_slice(bytecode * code)
{
    printf("%d: slice slice %u\n", code->addr, code->mk_slice.dims);
}

void bytecode_print_slice_string(bytecode * code)
{
    printf("%d: slice string %u\n", code->addr, code->mk_slice.dims);
}

void bytecode_print_array_deref(bytecode * code)
{
    printf("%d: array deref %d\n", code->addr, code->array_deref.dims);
}

void bytecode_print_arrayref_deref(bytecode * code)
{
    printf("%d: array deref %d\n", code->addr, code->array_deref.dims);
}

void bytecode_print_range_deref(bytecode * code)
{
    printf("%d: range deref %d\n", code->addr, code->array_deref.dims);
}

void bytecode_print_slice_deref(bytecode * code)
{
    printf("%d: slice deref %d\n", code->addr, code->array_deref.dims);
}

void bytecode_print_string_deref(bytecode * code)
{
    printf("%d: string deref\n", code->addr);
}

void bytecode_print_array_append(bytecode * code)
{
    printf("%d: array append %d %d\n", code->addr, 
                                       code->id_local.stack_level,
                                       code->id_local.index);
}

void bytecode_print_record(bytecode * code)
{
    printf("%d: record count %u\n", code->addr, code->record.count);
}

void bytecode_print_vec_deref(bytecode * code)
{
    printf("%d: vec index %d %u\n", code->addr, code->attr.stack_level, code->attr.index);
}

void bytecode_print_vecref_deref(bytecode * code)
{
    printf("%d: vecref index\n", code->addr);
}

void bytecode_print_vecref_vec_deref(bytecode * code)
{
    printf("%d: vecref vec index %d %u\n", code->addr, code->attr.stack_level, code->attr.index);
}

void bytecode_print_vecref_vec_index_deref(bytecode * code)
{
    printf("%d: vecref vec index\n", code->addr);
}

void bytecode_print_nil_record_ref(bytecode * code)
{
    printf("%d: nil record ref\n", code->addr);
}

void bytecode_print_record_unpack(bytecode * code)
{
    printf("%d: record unpack %u\n", code->addr, code->record.count);
}

void bytecode_print_func_def(bytecode * code)
{
    printf("\n%d: func def\n", code->addr);
}

void bytecode_print_func_obj(bytecode * code)
{
    printf("%d: func obj\n", code->addr);
}

void bytecode_print_func_ffi(bytecode * code)
{
    printf("%d: func ffi %d %d %d\n", code->addr, code->ffi.count,
                                      code->ffi.fname_index, code->ffi.libname_index);
}

void bytecode_print_func_ffi_bool(bytecode * code)
{
    printf("%d: ffi bool\n", code->addr);
}

void bytecode_print_func_ffi_int(bytecode * code)
{
    printf("%d: ffi int\n", code->addr);
}

void bytecode_print_func_ffi_long(bytecode * code)
{
    printf("%d: ffi long\n", code->addr);
}

void bytecode_print_func_ffi_float(bytecode * code)
{
    printf("%d: ffi float\n", code->addr);
}

void bytecode_print_func_ffi_double(bytecode * code)
{
    printf("%d: ffi double\n", code->addr);
}

void bytecode_print_func_ffi_char(bytecode * code)
{
    printf("%d: ffi char\n", code->addr);
}

void bytecode_print_func_ffi_string(bytecode * code)
{
    printf("%d: ffi string\n", code->addr);
}

void bytecode_print_func_ffi_void(bytecode * code)
{
    printf("%d: ffi void\n", code->addr);
}

void bytecode_print_func_ffi_c_ptr(bytecode * code)
{
    printf("%d: ffi c_ptr\n", code->addr);
}

void bytecode_print_func_ffi_record(bytecode * code)
{
    printf("%d: ffi record %u %u\n", code->addr, code->ffi_record.count, code->ffi_record.total_count);
}

void bytecode_print_dup(bytecode * code)
{
    printf("%d: dup %u\n", code->addr, code->dup.n);
}

void bytecode_print_global_vec(bytecode * code)
{
    printf("%d: global vec %d\n", code->addr, code->global_vec.count);
}

void bytecode_print_mark(bytecode * code)
{
    printf("%d: mark %u\n", code->addr, code->mark.addr);
}

void bytecode_print_call(bytecode * code) { printf("%d: call\n", code->addr); }

void bytecode_print_slide(bytecode * code)
{
    printf("%d: slide %u %u\n", code->addr, code->slide.q, code->slide.m);
}

void bytecode_print_clear_stack(bytecode * code)
{
    printf("%d: clear stack\n", code->addr);
}

void bytecode_print_ret(bytecode * code)
{
    printf("%d: ret %d\n", code->addr, code->clear.count);
}

void bytecode_print_line(bytecode * code)
{
    printf("%d: line %u\n", code->addr, code->line.no);
}

void bytecode_print_build_in(bytecode * code)
{
    printf("%d: build in id %u\n", code->addr, code->build_in.id);
}

void bytecode_print_copyglob(bytecode * code)
{
    printf("%d: copyglob\n", code->addr);
}

void bytecode_print_alloc(bytecode * code)
{
    printf("%d: alloc %u\n", code->addr, code->alloc.n);
}

void bytecode_print_rewrite(bytecode * code)
{
    printf("%d: rewrite %u\n", code->addr, code->rewrite.j);
}

void bytecode_print_push_param(bytecode * code)
{
    printf("%d: push param\n", code->addr);
}

void bytecode_print_push_except(bytecode * code)
{
    printf("%d: push except\n", code->addr);
}

void bytecode_print_rethrow(bytecode * code)
{
    printf("%d: rethrow\n", code->addr);
}

void bytecode_print_unhandled_exception(bytecode * code)
{
    printf("%d: unhandled exception\n", code->addr);
}

void bytecode_print_halt(bytecode * code) { printf("%d: halt\n", code->addr); }

bytecode_list_node * bytecode_list_node_new(bytecode * value)
{
    bytecode_list_node * node =
        (bytecode_list_node *)malloc(sizeof(bytecode_list_node));

    node->value = *value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void bytecode_list_node_delete(bytecode_list_node * node) { free(node); }

bytecode_list * bytecode_new()
{
    bytecode_list * list = (bytecode_list *)malloc(sizeof(bytecode_list));

    list->addr = 0;
    list->head = NULL;
    list->tail = NULL;

    bytecode_op_test();

    return list;
}

void bytecode_delete(bytecode_list * code)
{
    bytecode_list_node * node = code->tail;
    while (node != NULL)
    {
        bytecode_list_node * tmp = node->next;
        bytecode_list_node_delete(node);
        node = tmp;
    }
    free(code);
}

bytecode * bytecode_add(bytecode_list * code, bytecode * value)
{
    bytecode_list_node * node;

    value->addr = code->addr++;
    node = bytecode_list_node_new(value);

    if (code->head == NULL && code->tail == NULL)
    {
        code->head = code->tail = node;
    }
    else
    {
        code->head->next = node;
        node->prev = code->head;
        code->head = node;
    }

    return &node->value;
}

void bytecode_func_addr(bytecode_list * code)
{
    bytecode_list_node * node = code->tail;
    while (node != NULL)
    {
        bytecode * code_value = &node->value;
        if (code_value != NULL)
        {
            if (code_value->type == BYTECODE_ID_FUNC_FUNC)
            {
                code_value->type = BYTECODE_ID_FUNC_ADDR;
                code_value->id_func.func_addr = code_value->id_func.func_value->addr;
            }
        }
        node = node->next;
    }
}

void bytecode_print(bytecode * code)
{
    bytecode_op[code->type].print(code);
}

void bytecode_list_print(bytecode_list * code)
{
    bytecode_list_node * node = code->tail;

    printf("---- bytecode beg ---\n");

    while (node != NULL)
    {
        bytecode * code_value = &node->value;
        if (code_value != NULL)
        {
            bytecode_print(code_value);
        }
        node = node->next;
    }

    printf("---- bytecode end ---\n");
}

void bytecode_to_array(bytecode_list * code, bytecode ** code_arr,
                       unsigned int * code_size)
{
    unsigned int addr = 0;
    bytecode_list_node * node = NULL;

    *code_size = code->addr;
    *code_arr = (bytecode *)malloc(code->addr * sizeof(bytecode));

    node = code->tail;
    while (node != NULL)
    {
        if (node->value.addr != addr)
        {
            fprintf(stderr, "incorrectly generated code\n");
            assert(0);
        }
        if (node->value.type == BYTECODE_ID_FUNC_FUNC)
        {
            fprintf(stderr, "cannot generate bytecode array with function "
                            "pointers, use bytecode_func_addr\n");
            assert(0);
        }

        (*code_arr)[addr++] = node->value;

        node = node->next;
    }
}

void bytecode_array_delete(bytecode * code_arr) { free(code_arr); }

void bytecode_array_print(bytecode * code_arr, unsigned int size)
{
    unsigned int i;

    printf("---- bytecode array beg ---\n");

    for (i = 0; i < size; i++)
    {
        bytecode_op[code_arr[i].type].print(code_arr + i);
    }

    printf("---- bytecode array end ---\n");
}

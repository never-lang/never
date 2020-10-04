/** 
 * Copyright 2020 Slawomir Maludzinski
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
#include "vmexec.h"
#include "vm.h"
#include "vmffi.h"
#include "gc.h"
#include "libmath.h"
#include "libvm.h"
#include "utils.h"
#include "module.h"
#include "strutil.h"
#include "dlcache.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vm_execute_str vm_execute_op[] = {
    { BYTECODE_UNKNOWN, vm_execute_unknown },

    { BYTECODE_INT, vm_execute_int },
    { BYTECODE_LONG, vm_execute_long },
    { BYTECODE_FLOAT, vm_execute_float },
    { BYTECODE_DOUBLE, vm_execute_double },
    { BYTECODE_CHAR, vm_execute_char },
    { BYTECODE_STRING, vm_execute_string },
    { BYTECODE_C_NULL, vm_execute_c_null },

    { BYTECODE_ID_TOP, vm_execute_id_top },
    { BYTECODE_ID_LOCAL, vm_execute_id_local },
    { BYTECODE_ID_DIM_LOCAL, vm_execute_id_dim_local },
    { BYTECODE_ID_DIM_SLICE, vm_execute_id_dim_slice },
    { BYTECODE_ID_GLOBAL, vm_execute_id_global },
    { BYTECODE_ID_FUNC_FUNC, vm_execute_id_func_func },
    { BYTECODE_ID_FUNC_ADDR, vm_execute_id_func_addr },
    { BYTECODE_ID_FUNC_ENTRY, vm_execute_id_func_entry },

    { BYTECODE_OP_NEG_INT, vm_execute_op_neg_int },
    { BYTECODE_OP_ADD_INT, vm_execute_op_add_int },
    { BYTECODE_OP_SUB_INT, vm_execute_op_sub_int },
    { BYTECODE_OP_MUL_INT, vm_execute_op_mul_int },
    { BYTECODE_OP_DIV_INT, vm_execute_op_div_int },
    { BYTECODE_OP_MOD_INT, vm_execute_op_mod_int },

    { BYTECODE_OP_NEG_LONG, vm_execute_op_neg_long },
    { BYTECODE_OP_ADD_LONG, vm_execute_op_add_long },
    { BYTECODE_OP_SUB_LONG, vm_execute_op_sub_long },
    { BYTECODE_OP_MUL_LONG, vm_execute_op_mul_long },
    { BYTECODE_OP_DIV_LONG, vm_execute_op_div_long },
    { BYTECODE_OP_MOD_LONG, vm_execute_op_mod_long },

    { BYTECODE_OP_NEG_FLOAT, vm_execute_op_neg_float },
    { BYTECODE_OP_ADD_FLOAT, vm_execute_op_add_float },
    { BYTECODE_OP_SUB_FLOAT, vm_execute_op_sub_float },
    { BYTECODE_OP_MUL_FLOAT, vm_execute_op_mul_float },
    { BYTECODE_OP_DIV_FLOAT, vm_execute_op_div_float },

    { BYTECODE_OP_NEG_DOUBLE, vm_execute_op_neg_double },
    { BYTECODE_OP_ADD_DOUBLE, vm_execute_op_add_double },
    { BYTECODE_OP_SUB_DOUBLE, vm_execute_op_sub_double },
    { BYTECODE_OP_MUL_DOUBLE, vm_execute_op_mul_double },
    { BYTECODE_OP_DIV_DOUBLE, vm_execute_op_div_double },

    { BYTECODE_OP_ADD_STRING, vm_execute_op_add_string },
    { BYTECODE_OP_ADD_INT_STRING, vm_execute_op_add_int_string },
    { BYTECODE_OP_ADD_STRING_INT, vm_execute_op_add_string_int },
    { BYTECODE_OP_ADD_LONG_STRING, vm_execute_op_add_long_string },
    { BYTECODE_OP_ADD_STRING_LONG, vm_execute_op_add_string_long },
    { BYTECODE_OP_ADD_FLOAT_STRING, vm_execute_op_add_float_string },
    { BYTECODE_OP_ADD_STRING_FLOAT, vm_execute_op_add_string_float },
    { BYTECODE_OP_ADD_DOUBLE_STRING, vm_execute_op_add_double_string },
    { BYTECODE_OP_ADD_STRING_DOUBLE, vm_execute_op_add_string_double },
    { BYTECODE_OP_ADD_CHAR_STRING, vm_execute_op_add_char_string },
    { BYTECODE_OP_ADD_STRING_CHAR, vm_execute_op_add_string_char },

    { BYTECODE_OP_LT_INT, vm_execute_op_lt_int },
    { BYTECODE_OP_GT_INT, vm_execute_op_gt_int },
    { BYTECODE_OP_LTE_INT, vm_execute_op_lte_int },
    { BYTECODE_OP_GTE_INT, vm_execute_op_gte_int },
    { BYTECODE_OP_EQ_INT, vm_execute_op_eq_int },
    { BYTECODE_OP_NEQ_INT, vm_execute_op_neq_int },

    { BYTECODE_OP_LT_LONG, vm_execute_op_lt_long },
    { BYTECODE_OP_GT_LONG, vm_execute_op_gt_long },
    { BYTECODE_OP_LTE_LONG, vm_execute_op_lte_long },
    { BYTECODE_OP_GTE_LONG, vm_execute_op_gte_long },
    { BYTECODE_OP_EQ_LONG, vm_execute_op_eq_long },
    { BYTECODE_OP_NEQ_LONG, vm_execute_op_neq_long },

    { BYTECODE_OP_LT_FLOAT, vm_execute_op_lt_float },
    { BYTECODE_OP_GT_FLOAT, vm_execute_op_gt_float },
    { BYTECODE_OP_LTE_FLOAT, vm_execute_op_lte_float },
    { BYTECODE_OP_GTE_FLOAT, vm_execute_op_gte_float },
    { BYTECODE_OP_EQ_FLOAT, vm_execute_op_eq_float },
    { BYTECODE_OP_NEQ_FLOAT, vm_execute_op_neq_float },

    { BYTECODE_OP_LT_DOUBLE, vm_execute_op_lt_double },
    { BYTECODE_OP_GT_DOUBLE, vm_execute_op_gt_double },
    { BYTECODE_OP_LTE_DOUBLE, vm_execute_op_lte_double },
    { BYTECODE_OP_GTE_DOUBLE, vm_execute_op_gte_double },
    { BYTECODE_OP_EQ_DOUBLE, vm_execute_op_eq_double },
    { BYTECODE_OP_NEQ_DOUBLE, vm_execute_op_neq_double },

    { BYTECODE_OP_LT_CHAR, vm_execute_op_lt_char },
    { BYTECODE_OP_GT_CHAR, vm_execute_op_gt_char },
    { BYTECODE_OP_LTE_CHAR, vm_execute_op_lte_char },
    { BYTECODE_OP_GTE_CHAR, vm_execute_op_gte_char },
    { BYTECODE_OP_EQ_CHAR, vm_execute_op_eq_char },
    { BYTECODE_OP_NEQ_CHAR, vm_execute_op_neq_char },

    { BYTECODE_OP_EQ_STRING, vm_execute_op_eq_string },
    { BYTECODE_OP_NEQ_STRING, vm_execute_op_neq_string },

    { BYTECODE_OP_EQ_C_PTR, vm_execute_op_eq_c_ptr },
    { BYTECODE_OP_NEQ_C_PTR, vm_execute_op_neq_c_ptr },

    { BYTECODE_OP_EQ_NIL, vm_execute_op_eq_nil },

    { BYTECODE_OP_EQ_STRING_NIL, vm_execute_op_eq_string_nil },
    { BYTECODE_OP_EQ_ARRAY_NIL, vm_execute_op_eq_array_nil },
    { BYTECODE_OP_EQ_RECORD_NIL, vm_execute_op_eq_record_nil },
    { BYTECODE_OP_EQ_FUNC_NIL, vm_execute_op_eq_func_nil },

    { BYTECODE_OP_EQ_NIL_STRING, vm_execute_op_eq_nil_string },
    { BYTECODE_OP_EQ_NIL_ARRAY, vm_execute_op_eq_nil_array },
    { BYTECODE_OP_EQ_NIL_RECORD, vm_execute_op_eq_nil_record },
    { BYTECODE_OP_EQ_NIL_FUNC, vm_execute_op_eq_nil_func },

    { BYTECODE_OP_NEQ_NIL, vm_execute_op_neq_nil },

    { BYTECODE_OP_NEQ_STRING_NIL, vm_execute_op_neq_string_nil },
    { BYTECODE_OP_NEQ_ARRAY_NIL, vm_execute_op_neq_array_nil },
    { BYTECODE_OP_NEQ_RECORD_NIL, vm_execute_op_neq_record_nil },
    { BYTECODE_OP_NEQ_FUNC_NIL, vm_execute_op_neq_func_nil },

    { BYTECODE_OP_NEQ_NIL_STRING, vm_execute_op_neq_nil_string },
    { BYTECODE_OP_NEQ_NIL_ARRAY, vm_execute_op_neq_nil_array },
    { BYTECODE_OP_NEQ_NIL_RECORD, vm_execute_op_neq_nil_record },
    { BYTECODE_OP_NEQ_NIL_FUNC, vm_execute_op_neq_nil_func },

    { BYTECODE_OP_NOT_INT, vm_execute_op_not_int },

    { BYTECODE_OP_INC_INT, vm_execute_op_inc_int },
    { BYTECODE_OP_DEC_INT, vm_execute_op_dec_int },
    { BYTECODE_OP_DUP_INT, vm_execute_op_dup_int },

    { BYTECODE_INT_TO_LONG, vm_execute_int_to_long },
    { BYTECODE_INT_TO_FLOAT, vm_execute_int_to_float },
    { BYTECODE_INT_TO_DOUBLE, vm_execute_int_to_double },
    { BYTECODE_LONG_TO_INT, vm_execute_long_to_int },
    { BYTECODE_LONG_TO_FLOAT, vm_execute_long_to_float },
    { BYTECODE_LONG_TO_DOUBLE, vm_execute_long_to_double },
    { BYTECODE_FLOAT_TO_INT, vm_execute_float_to_int },
    { BYTECODE_FLOAT_TO_LONG, vm_execute_float_to_long },
    { BYTECODE_FLOAT_TO_DOUBLE, vm_execute_float_to_double },
    { BYTECODE_DOUBLE_TO_INT, vm_execute_double_to_int },
    { BYTECODE_DOUBLE_TO_LONG, vm_execute_double_to_long },
    { BYTECODE_DOUBLE_TO_FLOAT, vm_execute_double_to_float },
    { BYTECODE_ENUMTYPE_RECORD_TO_INT, vm_execute_enumtype_record_to_int},

    { BYTECODE_OP_NEG_ARR_INT, vm_execute_op_neg_arr_int },
    { BYTECODE_OP_NEG_ARR_LONG, vm_execute_op_neg_arr_long },
    { BYTECODE_OP_NEG_ARR_FLOAT, vm_execute_op_neg_arr_float },
    { BYTECODE_OP_NEG_ARR_DOUBLE, vm_execute_op_neg_arr_double },
    { BYTECODE_OP_ADD_ARR_INT, vm_execute_op_add_arr_int },
    { BYTECODE_OP_ADD_ARR_LONG, vm_execute_op_add_arr_long },
    { BYTECODE_OP_ADD_ARR_FLOAT, vm_execute_op_add_arr_float },
    { BYTECODE_OP_ADD_ARR_DOUBLE, vm_execute_op_add_arr_double },
    { BYTECODE_OP_SUB_ARR_INT, vm_execute_op_sub_arr_int },
    { BYTECODE_OP_SUB_ARR_LONG, vm_execute_op_sub_arr_long },
    { BYTECODE_OP_SUB_ARR_FLOAT, vm_execute_op_sub_arr_float },
    { BYTECODE_OP_SUB_ARR_DOUBLE, vm_execute_op_sub_arr_double },
    { BYTECODE_OP_MUL_ARR_INT, vm_execute_op_mul_arr_int },
    { BYTECODE_OP_MUL_ARR_LONG, vm_execute_op_mul_arr_long },
    { BYTECODE_OP_MUL_ARR_FLOAT, vm_execute_op_mul_arr_float },
    { BYTECODE_OP_MUL_ARR_DOUBLE, vm_execute_op_mul_arr_double },
    { BYTECODE_OP_MUL_ARR_ARR_INT, vm_execute_op_mul_arr_arr_int },
    { BYTECODE_OP_MUL_ARR_ARR_LONG, vm_execute_op_mul_arr_arr_long },
    { BYTECODE_OP_MUL_ARR_ARR_FLOAT, vm_execute_op_mul_arr_arr_float },
    { BYTECODE_OP_MUL_ARR_ARR_DOUBLE, vm_execute_op_mul_arr_arr_double },

    { BYTECODE_OP_ASS_INT, vm_execute_op_ass_int },
    { BYTECODE_OP_ASS_LONG, vm_execute_op_ass_long },
    { BYTECODE_OP_ASS_FLOAT, vm_execute_op_ass_float },
    { BYTECODE_OP_ASS_DOUBLE, vm_execute_op_ass_double },
    { BYTECODE_OP_ASS_CHAR, vm_execute_op_ass_char },
    { BYTECODE_OP_ASS_STRING, vm_execute_op_ass_string },
    { BYTECODE_OP_ASS_C_PTR, vm_execute_op_ass_c_ptr },
    { BYTECODE_OP_ASS_ARRAY, vm_execute_op_ass_array },
    { BYTECODE_OP_ASS_RECORD, vm_execute_op_ass_record },
    { BYTECODE_OP_ASS_FUNC, vm_execute_op_ass_func },

    { BYTECODE_OP_ASS_RECORD_NIL, vm_execute_op_ass_record_nil },

    { BYTECODE_JUMPZ, vm_execute_jumpz },
    { BYTECODE_JUMP, vm_execute_jump },
    { BYTECODE_LABEL, vm_execute_label },

    { BYTECODE_MK_ARRAY_INT, vm_execute_mk_array_int },
    { BYTECODE_MK_ARRAY_LONG, vm_execute_mk_array_long },
    { BYTECODE_MK_ARRAY_FLOAT, vm_execute_mk_array_float },
    { BYTECODE_MK_ARRAY_DOUBLE, vm_execute_mk_array_double },
    { BYTECODE_MK_ARRAY_CHAR, vm_execute_mk_array_char },
    { BYTECODE_MK_ARRAY_STRING, vm_execute_mk_array_string },
    { BYTECODE_MK_ARRAY_ARRAY, vm_execute_mk_array_array },
    { BYTECODE_MK_ARRAY_RECORD, vm_execute_mk_array_record },
    { BYTECODE_MK_ARRAY_FUNC, vm_execute_mk_array_func },
    { BYTECODE_MK_INIT_ARRAY, vm_execute_mk_init_array },
    { BYTECODE_MK_RANGE, vm_execute_mk_range },

    { BYTECODE_SLICE_ARRAY, vm_execute_slice_array },
    { BYTECODE_SLICE_RANGE, vm_execute_slice_range },
    { BYTECODE_SLICE_SLICE, vm_execute_slice_slice },
    { BYTECODE_SLICE_STRING, vm_execute_slice_string },

    { BYTECODE_ARRAY_DEREF, vm_execute_array_deref },
    { BYTECODE_ARRAYREF_DEREF, vm_execute_arrayref_deref },
    { BYTECODE_RANGE_DEREF, vm_execute_range_deref },
    { BYTECODE_SLICE_DEREF, vm_execute_slice_deref },
    { BYTECODE_STRING_DEREF, vm_execute_string_deref },
    { BYTECODE_ARRAY_APPEND, vm_execute_array_append },

    { BYTECODE_RECORD, vm_execute_record },
    { BYTECODE_VEC_DEREF, vm_execute_vec_deref },
    { BYTECODE_VECREF_DEREF, vm_execute_vecref_deref },
    { BYTECODE_VECREF_VEC_DEREF, vm_execute_vecref_vec_deref },
    { BYTECODE_NIL_RECORD_REF, vm_execute_nil_record_ref },

    { BYTECODE_FUNC_DEF, vm_execute_func_def },
    { BYTECODE_FUNC_OBJ, vm_execute_func_obj },
    { BYTECODE_FUNC_FFI, vm_execute_func_ffi },
    { BYTECODE_FUNC_FFI_BOOL, vm_execute_func_ffi_bool },
    { BYTECODE_FUNC_FFI_INT, vm_execute_func_ffi_int },
    { BYTECODE_FUNC_FFI_LONG, vm_execute_func_ffi_long },
    { BYTECODE_FUNC_FFI_FLOAT, vm_execute_func_ffi_float },
    { BYTECODE_FUNC_FFI_DOUBLE, vm_execute_func_ffi_double },
    { BYTECODE_FUNC_FFI_CHAR, vm_execute_func_ffi_char },
    { BYTECODE_FUNC_FFI_STRING, vm_execute_func_ffi_string },
    { BYTECODE_FUNC_FFI_VOID, vm_execute_func_ffi_void },
    { BYTECODE_FUNC_FFI_C_PTR, vm_execute_func_ffi_c_ptr },
    { BYTECODE_FUNC_FFI_RECORD, vm_execute_func_ffi_record },

    { BYTECODE_DUP, vm_execute_dup },
    { BYTECODE_GLOBAL_VEC, vm_execute_global_vec },
    { BYTECODE_MARK, vm_execute_mark },
    { BYTECODE_CALL, vm_execute_call },
    { BYTECODE_SLIDE, vm_execute_slide },
    { BYTECODE_CLEAR_STACK, vm_execute_clear_stack },
    { BYTECODE_RET, vm_execute_ret },
    { BYTECODE_LINE, vm_execute_line },
    { BYTECODE_BUILD_IN, vm_execute_build_in },
    { BYTECODE_COPYGLOB, vm_execute_copyglob },
    { BYTECODE_ALLOC, vm_execute_alloc },
    { BYTECODE_REWRITE, vm_execute_rewrite },
    { BYTECODE_PUSH_PARAM, vm_execute_push_param },
    { BYTECODE_PUSH_EXCEPT, vm_execute_push_except },
    { BYTECODE_RETHROW, vm_execute_rethrow },

    { BYTECODE_UNHANDLED_EXCEPTION, vm_execute_unhandled_exception },
    { BYTECODE_HALT, vm_execute_halt }
};

void vm_execute_op_test()
{
    unsigned int i = 0;
    for (i = 0; i < BYTECODE_END; i++)
    {
        assert(i == vm_execute_op[i].type);
    }
}

void vm_check_stack(vm * machine)
{
    if (machine->sp >= machine->stack_size)
    {
        fprintf(stderr, "stack too large\n");
        vm_print(machine, "stack too large");
        exit(1);
    }
}

void vm_execute_unknown(vm * machine, bytecode * code)
{
    fprintf(stderr, "unknown bytecode\n");
    assert(0);
}

#define vm_execute_type(never_type)                                         \
    void vm_execute_##never_type(vm * machine, bytecode * code)             \
    {                                                                       \
        gc_stack entry = { 0 };                                             \
        mem_ptr addr = gc_alloc_##never_type(machine->collector, code->never_type##_t.value); \
                                                                            \
        machine->sp++;                                                      \
        vm_check_stack(machine);                                            \
                                                                            \
        entry.type = GC_MEM_ADDR;                                           \
        entry.addr = addr;                                                  \
                                                                            \
        machine->stack[machine->sp] = entry;                                \
    }                                                                       \

vm_execute_type(int)
vm_execute_type(long)
vm_execute_type(float)
vm_execute_type(double)
vm_execute_type(char)

void vm_execute_string(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    char ** strtab_array = machine->prog->module_value->strtab_array;
    mem_ptr addr = gc_alloc_string(machine->collector, strtab_array[code->string.index]);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_string_ref(machine->collector, addr);

    machine->stack[machine->sp] = entry;
}

void vm_execute_c_null(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_c_ptr(machine->collector, NULL);

    machine->sp++;
    vm_check_stack(machine);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = addr;
    
    machine->stack[machine->sp] = entry;
}

void vm_execute_id_top(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = machine
                       ->stack[code->id_top.index]
                       .addr;

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_local(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = machine
                       ->stack[machine->sp - (code->id_local.stack_level -
                                              code->id_local.index)]
                       .addr;

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_dim_local(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = machine
                       ->stack[machine->sp - (code->id_dim_local.stack_level -
                                              code->id_dim_local.index)]
                       .addr;
    mem_ptr array = gc_get_arr_ref(machine->collector, addr);
    unsigned int dim_elems = gc_get_arr_dim_elems(machine->collector, array,
                                                  code->id_dim_local.dim_index);
    mem_ptr elems = gc_alloc_int(machine->collector, dim_elems);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = elems;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_dim_slice(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr slice_ref = machine
                           ->stack[machine->sp - (code->id_dim_slice.stack_level -
                                                  code->id_dim_slice.index)]
                           .addr;
    mem_ptr slice = gc_get_vec_ref(machine->collector, slice_ref);
    mem_ptr range = gc_get_vec(machine->collector, slice, SLICE_RANGE_INDEX);
    int dim = code->id_dim_slice.dim_index;

    int value = 0;
    if (dim % 2 == 0)
    {
        value = 0;
    }
    else
    {
        mem_ptr range_from_addr = { 0 };
        mem_ptr range_to_addr = { 0 };
        range_from_addr = gc_get_vec(machine->collector, range, dim - 1);
        range_to_addr = gc_get_vec(machine->collector, range, dim);

        int range_from = 0;
        int range_to = 0;
        range_from = gc_get_int(machine->collector, range_from_addr);
        range_to = gc_get_int(machine->collector, range_to_addr);

        if (range_to > range_from)
        {
            value = range_to - range_from;
        }
        else
        {
            value = range_from - range_to;
        }
    }

    mem_ptr value_addr = gc_alloc_int(machine->collector, value);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = value_addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_global(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr =
        gc_get_vec(machine->collector, machine->gp, code->id_global.index);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_func_func(vm * machine, bytecode * code)
{
    fprintf(stderr, "at this stage id_func_func should be set to id_func_addr "
                    "with bytecode_func_addr\n");
    assert(0);
}

void vm_execute_id_func_addr(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr vec = machine->stack[machine->sp].addr;
    mem_ptr addr =
        gc_alloc_func(machine->collector, vec, code->id_func.func_addr);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_func_entry(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr vec = machine->stack[machine->sp].addr;
    mem_ptr addr =
        gc_alloc_func(machine->collector, vec, machine->prog->entry_addr);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;   
}

/* a op b
 * b
 * a
 */
#define vm_execute_op_neg_type(never_type, c_type)              \
    void vm_execute_op_neg_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr); \
        mem_ptr addr = gc_alloc_##never_type(machine->collector, -a);    \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp] = entry;                    \
    }                                                           \

vm_execute_op_neg_type(int, int)
vm_execute_op_neg_type(long, long long)
vm_execute_op_neg_type(float, float)
vm_execute_op_neg_type(double, double)

#define vm_execute_op_add_type(never_type, c_type)              \
    void vm_execute_op_add_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr); \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr); \
        mem_ptr addr = gc_alloc_##never_type(machine->collector, a + b); \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp - 1] = entry;                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_add_type(int, int)
vm_execute_op_add_type(long, long long)
vm_execute_op_add_type(float, float)
vm_execute_op_add_type(double, double)

#define vm_execute_op_sub_type(never_type, c_type)              \
    void vm_execute_op_sub_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = gc_alloc_##never_type(machine->collector, a - b); \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp - 1] = entry;                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_sub_type(int, int)
vm_execute_op_sub_type(long, long long)
vm_execute_op_sub_type(float, float)
vm_execute_op_sub_type(double, double)

#define vm_execute_op_mul_type(never_type, c_type)              \
    void vm_execute_op_mul_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = gc_alloc_##never_type(machine->collector, a * b);  \
                                                                 \
        entry.type = GC_MEM_ADDR;                                \
        entry.addr = addr;                                       \
                                                                 \
        machine->stack[machine->sp - 1] = entry;                 \
        machine->sp--;                                           \
    }                                                            \

vm_execute_op_mul_type(int, int)
vm_execute_op_mul_type(long, long long)
vm_execute_op_mul_type(float, float)
vm_execute_op_mul_type(double, double)

#define vm_execute_op_div_type(never_type, c_type)              \
    void vm_execute_op_div_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = 0;                                       \
                                                                \
        if (b == 0)                                             \
        {                                                       \
            print_error_msg(machine->line_no, "cannot divide by zero"); \
            machine->running = VM_EXCEPTION;                    \
            machine->exception = EXCEPT_NO_DIVISION;            \
            return;                                             \
        }                                                       \
                                                                \
        addr = gc_alloc_##never_type(machine->collector, a / b);  \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp - 1] = entry;                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_div_type(int, int)
vm_execute_op_div_type(long, long long)
vm_execute_op_div_type(float, float)
vm_execute_op_div_type(double, double)

#define vm_execute_op_mod_type(never_type, c_type)              \
    void vm_execute_op_mod_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a = gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b =                                                                         \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);           \
        mem_ptr addr = 0;                                       \
                                                                \
        if (b == 0)                                             \
        {                                                       \
            print_error_msg(machine->line_no, "cannot divide by zero");  \
            machine->running = VM_EXCEPTION;                    \
            machine->exception = EXCEPT_NO_DIVISION;            \
            return;                                             \
        }                                                       \
                                                                \
        addr = gc_alloc_##never_type(machine->collector, a % b);         \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp - 1] = entry;                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_mod_type(int, int)
vm_execute_op_mod_type(long, long long)

void vm_execute_op_add_string(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr str_b = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr);
    if (str_a == nil_ptr || str_b == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    char * a = gc_get_string(machine->collector, str_a);
    char * b = gc_get_string(machine->collector, str_b);
    mem_ptr addr = 0;

    addr = gc_alloc_string_take(machine->collector, string_add(a, b));

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_string_ref(machine->collector, addr);

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

#define vm_execute_op_add_type_string(never_type, c_type)               \
    void vm_execute_op_add_##never_type##_string(vm * machine, bytecode * code)    \
    {                                                                   \
        gc_stack entry = { 0 };                                         \
                                                                        \
        c_type a = gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr); \
        mem_ptr str_b = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr); \
        if (str_b == nil_ptr)                                           \
        {                                                               \
            machine->running = VM_EXCEPTION;                            \
            machine->exception = EXCEPT_NIL_POINTER;                    \
            return;                                                     \
        }                                                               \
                                                                        \
        char * b = gc_get_string(machine->collector, str_b);            \
        mem_ptr addr = 0;                                               \
                                                                        \
        addr = gc_alloc_string_take(machine->collector, string_add_##never_type(b, a, 0));   \
                                                                        \
        entry.type = GC_MEM_ADDR;                                       \
        entry.addr = gc_alloc_string_ref(machine->collector, addr);     \
                                                                        \
        machine->stack[machine->sp - 1] = entry;                        \
        machine->sp--;                                                  \
    }                                                                   \

vm_execute_op_add_type_string(int, int)
vm_execute_op_add_type_string(long, long long)
vm_execute_op_add_type_string(float, float)
vm_execute_op_add_type_string(double, double)
vm_execute_op_add_type_string(char, char)

#define vm_execute_op_add_string_type(never_type, c_type)               \
    void vm_execute_op_add_string_##never_type(vm * machine, bytecode * code)    \
    {                                                                   \
        gc_stack entry = { 0 };                                         \
                                                                        \
        mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr); \
        if (str_a == nil_ptr)                                           \
        {                                                               \
            machine->running = VM_EXCEPTION;                            \
            machine->exception = EXCEPT_NIL_POINTER;                    \
            return;                                                     \
        }                                                               \
                                                                        \
        char * a = gc_get_string(machine->collector, str_a);            \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr); \
        mem_ptr addr = 0;                                               \
                                                                        \
        addr = gc_alloc_string_take(machine->collector, string_add_##never_type(a, b, 1)); \
                                                                        \
        entry.type = GC_MEM_ADDR;                                       \
        entry.addr = gc_alloc_string_ref(machine->collector, addr);     \
                                                                        \
        machine->stack[machine->sp - 1] = entry;                        \
        machine->sp--;                                                  \
    }                                                                   \

vm_execute_op_add_string_type(int, int)
vm_execute_op_add_string_type(long, long long)
vm_execute_op_add_string_type(float, float)
vm_execute_op_add_string_type(double, double)
vm_execute_op_add_string_type(char, char)

#define vm_execute_op_lt_type(never_type, c_type)               \
    void vm_execute_op_lt_##never_type(vm * machine, bytecode * code)    \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr); \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr); \
        mem_ptr addr = gc_alloc_int(machine->collector, a < b); \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp - 1] = entry;                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_lt_type(char, char)
vm_execute_op_lt_type(int, int)
vm_execute_op_lt_type(long, long long)
vm_execute_op_lt_type(float, float)
vm_execute_op_lt_type(double, double)

#define vm_execute_op_gt_type(never_type, c_type)               \
    void vm_execute_op_gt_##never_type(vm * machine, bytecode * code)    \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = gc_alloc_int(machine->collector, a > b); \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp - 1] = entry;                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_gt_type(char, char)
vm_execute_op_gt_type(int, int)
vm_execute_op_gt_type(long, long long)
vm_execute_op_gt_type(float, float)
vm_execute_op_gt_type(double, double)

#define vm_execute_op_lte_type(never_type, c_type)              \
    void vm_execute_op_lte_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = gc_alloc_int(machine->collector, a <= b);    \
                                                                    \
        entry.type = GC_MEM_ADDR;                                   \
        entry.addr = addr;                                          \
                                                                    \
        machine->stack[machine->sp - 1] = entry;                    \
        machine->sp--;                                              \
    }                                                               \

vm_execute_op_lte_type(char, char)
vm_execute_op_lte_type(int, int)
vm_execute_op_lte_type(long, long long)
vm_execute_op_lte_type(float, float)
vm_execute_op_lte_type(double, double)

#define vm_execute_op_gte_type(never_type, c_type)              \
    void vm_execute_op_gte_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = gc_alloc_int(machine->collector, a >= b);    \
                                                                    \
        entry.type = GC_MEM_ADDR;                                   \
        entry.addr = addr;                                          \
                                                                    \
        machine->stack[machine->sp - 1] = entry;                    \
        machine->sp--;                                              \
    }                                                               \

vm_execute_op_gte_type(char, char)
vm_execute_op_gte_type(int, int)
vm_execute_op_gte_type(long, long long)
vm_execute_op_gte_type(float, float)
vm_execute_op_gte_type(double, double)

#define vm_execute_op_eq_type(never_type, c_type)                \
    void vm_execute_op_eq_##never_type(vm * machine, bytecode * code)    \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr); \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr); \
        mem_ptr addr = gc_alloc_int(machine->collector, a == b);    \
                                                                    \
        entry.type = GC_MEM_ADDR;                                   \
        entry.addr = addr;                                          \
                                                                    \
        machine->stack[machine->sp - 1] = entry;                    \
        machine->sp--;                                              \
    }                                                               \

vm_execute_op_eq_type(char, char)
vm_execute_op_eq_type(int, int)
vm_execute_op_eq_type(long, long long)
vm_execute_op_eq_type(float, float)
vm_execute_op_eq_type(double, double)

#define vm_execute_op_neq_type(never_type, c_type)              \
    void vm_execute_op_neq_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        c_type a =                                                 \
            gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr);   \
        c_type b = gc_get_##never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = gc_alloc_int(machine->collector, a != b);    \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp - 1] = entry;                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_neq_type(char, char)
vm_execute_op_neq_type(int, int)
vm_execute_op_neq_type(long, long long)
vm_execute_op_neq_type(float, float)
vm_execute_op_neq_type(double, double)


void vm_execute_op_eq_string(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr str_b = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr);
    if (str_a == nil_ptr || str_b == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    char * a = gc_get_string(machine->collector, str_a);
    char * b = gc_get_string(machine->collector, str_b);
    mem_ptr addr = gc_alloc_int(machine->collector, strcmp(a, b) ? 0 : 1);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_string(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr str_b = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr);
    if (str_a == nil_ptr || str_b == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    char * a = gc_get_string(machine->collector, str_a);
    char * b = gc_get_string(machine->collector, str_b);
    mem_ptr addr = gc_alloc_int(machine->collector, strcmp(a, b) ? 1 : 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_c_ptr(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    void * a_ptr = gc_get_c_ptr(machine->collector, machine->stack[machine->sp - 1].addr);
    void * b_ptr = gc_get_c_ptr(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a_ptr == b_ptr);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_c_ptr(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    void * a_ptr = gc_get_c_ptr(machine->collector, machine->stack[machine->sp - 1].addr);
    void * b_ptr = gc_get_c_ptr(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a_ptr != b_ptr);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr a = gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr b = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a == b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_string_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, str_a == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_array_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr arr = gc_get_arr_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_record_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr arr = gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_func_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    ip_ptr fptr = gc_get_func_addr(machine->collector,
                                   machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, fptr == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_nil_string(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, str_a == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_nil_array(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr arr = gc_get_arr_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_nil_record(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr arr = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_nil_func(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    ip_ptr fptr = gc_get_func_addr(machine->collector,
                                   machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, fptr == 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr a = gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr b = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a != b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_string_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, str_a != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_array_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr arr = gc_get_arr_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_record_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr arr = gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_func_nil(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    ip_ptr fptr = gc_get_func_addr(machine->collector,
                                   machine->stack[machine->sp - 1].addr);
    gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, fptr != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_nil_string(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr str_a = gc_get_string_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, str_a != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_nil_array(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr arr = gc_get_arr_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_nil_record(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr arr = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, arr != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_nil_func(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    ip_ptr fptr = gc_get_func_addr(machine->collector,
                                   machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, fptr != 0);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_not_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, !a);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_op_inc_int(vm * machine, bytecode * code)
{
    mem_ptr addr = machine
                       ->stack[machine->sp - (code->id_local.stack_level -
                                              code->id_local.index)].addr;
    gc_inc_int(machine->collector, addr);
}

void vm_execute_op_dec_int(vm * machine, bytecode * code)
{
    mem_ptr addr = machine
                       ->stack[machine->sp - (code->id_local.stack_level -
                                              code->id_local.index)].addr;
    gc_dec_int(machine->collector, addr);
}

void vm_execute_op_dup_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr_a = machine
                       ->stack[machine->sp - (code->id_local.stack_level -
                                              code->id_local.index)].addr;
    int a = gc_get_int(machine->collector, addr_a);
    mem_ptr addr = gc_alloc_int(machine->collector, a);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

#define vm_execute_type_to_type(from_never_type, from_c_type, to_never_type, to_c_type) \
    void vm_execute_##from_never_type##_to_##to_never_type(vm * machine, bytecode * code)  \
    {                                                           \
        gc_stack entry = { 0 };                                 \
        from_c_type a = gc_get_##from_never_type(machine->collector, machine->stack[machine->sp].addr);   \
        mem_ptr addr = gc_alloc_##to_never_type(machine->collector, (to_c_type)a);             \
                                                                \
        entry.type = GC_MEM_ADDR;                               \
        entry.addr = addr;                                      \
                                                                \
        machine->stack[machine->sp] = entry;                    \
    }                                                           \

vm_execute_type_to_type(int, int, long, long long)
vm_execute_type_to_type(int, int, float, float)
vm_execute_type_to_type(int, int, double, double)

vm_execute_type_to_type(long, long long, int, int)
vm_execute_type_to_type(long, long long, float, float)
vm_execute_type_to_type(long, long long, double, double)

vm_execute_type_to_type(float, float, int, int)
vm_execute_type_to_type(float, float, long, long long)
vm_execute_type_to_type(float, float, double, double)

vm_execute_type_to_type(double, double, int, int)
vm_execute_type_to_type(double, double, long, long long)
vm_execute_type_to_type(double, double, float, float)

void vm_execute_enumtype_record_to_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr record_value = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    if (record_value == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
    }

    mem_ptr index_addr = gc_get_vec(machine->collector, record_value, 0);
    int a = gc_get_int(machine->collector, index_addr);

    mem_ptr int_addr = gc_alloc_int(machine->collector, a);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = int_addr;

    machine->stack[machine->sp] = entry;
}

#define vm_execute_op_neg_arr_type(never_type, c_type)                          \
    void vm_execute_op_neg_arr_##never_type(vm * machine, bytecode * code)      \
    {                                                                           \
        gc_stack entry = { 0 };                                                 \
                                                                                \
        mem_ptr array = gc_get_arr_ref(machine->collector, machine->stack[machine->sp].addr); \
        if (array == nil_ptr)                                                   \
        {                                                                       \
            machine->running = VM_EXCEPTION;                                    \
            machine->exception = EXCEPT_NIL_POINTER;                            \
            return;                                                             \
        }                                                                       \
                                                                                \
        object_arr * m1 = gc_get_arr_obj(machine->collector, array);            \
        mem_ptr mres = gc_copy_arr(machine->collector, array);                  \
                                                                                \
        unsigned int e = 0;                                                     \
        for (e = 0; e < m1->elems; e++)                                         \
        {                                                                       \
            c_type eval = gc_get_##never_type(machine->collector, m1->value[e]); \
            mem_ptr cptr = gc_alloc_##never_type(machine->collector, -1 * eval); \
                                                                                \
            gc_set_arr_elem(machine->collector, mres, e, cptr);                 \
        }                                                                       \
                                                                                \
        entry.type = GC_MEM_ADDR;                                               \
        entry.addr = gc_alloc_arr_ref(machine->collector, mres);                \
                                                                                \
        machine->stack[machine->sp] = entry;                                    \
    }                                                                           \

vm_execute_op_neg_arr_type(int, int)
vm_execute_op_neg_arr_type(long, long long)
vm_execute_op_neg_arr_type(float, float)
vm_execute_op_neg_arr_type(double, double)

#define vm_execute_op_add_arr_type(never_type, c_type)                      \
    void vm_execute_op_add_arr_##never_type(vm * machine, bytecode * code)  \
    {                                                                       \
        gc_stack entry = { 0 };                                             \
                                                                            \
        mem_ptr array_1 = gc_get_arr_ref(machine->collector, machine->stack[machine->sp - 1].addr); \
        mem_ptr array = gc_get_arr_ref(machine->collector, machine->stack[machine->sp].addr);       \
        if (array_1 == nil_ptr || array == nil_ptr)                         \
        {                                                                   \
            machine->running = VM_EXCEPTION;                                \
            machine->exception = EXCEPT_NIL_POINTER;                        \
            return;                                                         \
        }                                                                   \
                                                                            \
        object_arr * m1 = gc_get_arr_obj(machine->collector, array_1);      \
        object_arr * m2 = gc_get_arr_obj(machine->collector, array);        \
                                                                            \
        if (!object_arr_can_add(m1, m2))                                    \
        {                                                                   \
            print_error_msg(machine->line_no, "improper array size");       \
            machine->running = VM_EXCEPTION;                                \
            machine->exception = EXCEPT_NO_ARR_SIZE;                        \
            return;                                                         \
        }                                                                   \
                                                                            \
        mem_ptr mres = gc_copy_arr(machine->collector, array);              \
                                                                            \
        unsigned int e = 0;                                                 \
        for (e = 0; e < m1->elems; e++)                                     \
        {                                                                   \
            c_type eval1 = gc_get_##never_type(machine->collector, m1->value[e]);  \
            c_type eval2 = gc_get_##never_type(machine->collector, m2->value[e]);  \
            mem_ptr cptr = gc_alloc_##never_type(machine->collector, eval1 + eval2);   \
                                                                            \
            gc_set_arr_elem(machine->collector, mres, e, cptr);             \
        }                                                                   \
                                                                            \
        entry.type = GC_MEM_ADDR;                                           \
        entry.addr = gc_alloc_arr_ref(machine->collector, mres);            \
                                                                            \
        machine->stack[machine->sp - 1] = entry;                            \
        machine->sp--;                                                      \
    }                                                                       \

vm_execute_op_add_arr_type(int, int)
vm_execute_op_add_arr_type(long, long long)
vm_execute_op_add_arr_type(float, float)
vm_execute_op_add_arr_type(double, double)

#define vm_execute_op_sub_arr_type(never_type, c_type)                           \
    void vm_execute_op_sub_arr_##never_type(vm * machine, bytecode * code)       \
    {                                                                   \
        gc_stack entry = { 0 };                                         \
                                                                        \
        mem_ptr array_1 = gc_get_arr_ref(machine->collector, machine->stack[machine->sp - 1].addr); \
        mem_ptr array = gc_get_arr_ref(machine->collector, machine->stack[machine->sp].addr);       \
        if (array_1 == nil_ptr || array == nil_ptr)                     \
        {                                                               \
            machine->running = VM_EXCEPTION;                            \
            machine->exception = EXCEPT_NIL_POINTER;                    \
            return;                                                     \
        }                                                               \
                                                                        \
        object_arr * m1 = gc_get_arr_obj(machine->collector, array_1);  \
        object_arr * m2 = gc_get_arr_obj(machine->collector, array);    \
                                                                        \
        if (!object_arr_can_add(m1, m2))                                \
        {                                                               \
            print_error_msg(machine->line_no, "improper array size");   \
            machine->running = VM_EXCEPTION;                            \
            machine->exception = EXCEPT_NO_ARR_SIZE;                    \
            return;                                                     \
        }                                                               \
                                                                        \
        mem_ptr mres = gc_copy_arr(machine->collector, array);          \
                                                                        \
        unsigned int e = 0;                                             \
        for (e = 0; e < m1->elems; e++)                                 \
        {                                                               \
            c_type eval1 = gc_get_##never_type(machine->collector, m1->value[e]);   \
            c_type eval2 = gc_get_##never_type(machine->collector, m2->value[e]);   \
            mem_ptr cptr = gc_alloc_##never_type(machine->collector, eval1 - eval2);    \
                                                                        \
            gc_set_arr_elem(machine->collector, mres, e, cptr);         \
        }                                                               \
                                                                        \
        entry.type = GC_MEM_ADDR;                                       \
        entry.addr = gc_alloc_arr_ref(machine->collector, mres);        \
                                                                        \
        machine->stack[machine->sp - 1] = entry;                        \
        machine->sp--;                                                  \
    }                                                                   \

vm_execute_op_sub_arr_type(int, int)
vm_execute_op_sub_arr_type(long, long long)
vm_execute_op_sub_arr_type(float, float)
vm_execute_op_sub_arr_type(double, double)

#define vm_execute_op_mul_arr_type(never_type, c_type)                                          \
    void vm_execute_op_mul_arr_##never_type(vm * machine, bytecode * code)                      \
    {                                                                                           \
        gc_stack entry = { 0 };                                                                 \
                                                                                                \
        c_type a = gc_get_##never_type(machine->collector, machine->stack[machine->sp - 1].addr); \
        mem_ptr array = gc_get_arr_ref(machine->collector, machine->stack[machine->sp].addr);   \
        if (array == nil_ptr)                                                                   \
        {                                                                                       \
            machine->running = VM_EXCEPTION;                                                    \
            machine->exception = EXCEPT_NIL_POINTER;                                            \
            return;                                                                             \
        }                                                                                       \
                                                                                                \
        object_arr * m1 = gc_get_arr_obj(machine->collector, array);                            \
        mem_ptr mres = gc_copy_arr(machine->collector, array);                                  \
                                                                                                \
        unsigned int e = 0;                                                                     \
        for (e = 0; e < m1->elems; e++)                                                         \
        {                                                                                       \
            c_type eval = gc_get_##never_type(machine->collector, m1->value[e]);                \
            mem_ptr cptr = gc_alloc_##never_type(machine->collector, a * eval);                 \
                                                                                                \
            gc_set_arr_elem(machine->collector, mres, e, cptr);                                 \
        }                                                                                       \
                                                                                                \
        entry.type = GC_MEM_ADDR;                                                               \
        entry.addr = gc_alloc_arr_ref(machine->collector, mres);                                \
                                                                                                \
        machine->stack[machine->sp - 1] = entry;                                                \
        machine->sp--;                                                                          \
    }                                                                                           \

vm_execute_op_mul_arr_type(int, int)
vm_execute_op_mul_arr_type(long, long long)
vm_execute_op_mul_arr_type(float, float)
vm_execute_op_mul_arr_type(double, double)

#define vm_execute_op_mul_arr_arr_type(never_type, c_type)                                          \
    void vm_execute_op_mul_arr_arr_##never_type(vm * machine, bytecode * code)                      \
    {                                                                                               \
        gc_stack entry = { 0 };                                                                     \
                                                                                                    \
        mem_ptr array_1 = gc_get_arr_ref(machine->collector, machine->stack[machine->sp - 1].addr); \
        mem_ptr array = gc_get_arr_ref(machine->collector, machine->stack[machine->sp].addr);       \
        if (array == nil_ptr || array_1 == nil_ptr)                                                 \
        {                                                                                           \
            machine->running = VM_EXCEPTION;                                                        \
            machine->exception = EXCEPT_NIL_POINTER;                                                \
            return;                                                                                 \
        }                                                                                           \
                                                                                                    \
        object_arr * m1 = gc_get_arr_obj(machine->collector, array_1);                              \
        object_arr * m2 = gc_get_arr_obj(machine->collector, array);                                \
                                                                                                    \
        if (!object_arr_can_mult(m1, m2))                                                           \
        {                                                                                           \
            print_error_msg(machine->line_no, "improper array size");                               \
            machine->running = VM_EXCEPTION;                                                        \
            machine->exception = EXCEPT_NO_ARR_SIZE;                                                \
            return;                                                                                 \
        }                                                                                           \
                                                                                                    \
        mem_ptr mres = { 0 };                                                                       \
        object_arr_dim * dv = NULL;                                                                 \
                                                                                                    \
        dv = object_arr_dim_new(2);                                                                 \
        dv[0].elems = m1->dv[0].elems;                                                              \
        dv[1].elems = m2->dv[1].elems;                                                              \
        mres = gc_alloc_arr(machine->collector, 2, dv);                                             \
                                                                                                    \
        unsigned int i, j, k;                                                                       \
        for (i = 0; i < m1->dv[0].elems; i++)                                                       \
        {                                                                                           \
            for (j = 0; j < m2->dv[1].elems; j++)                                                   \
            {                                                                                       \
                c_type sum = 0;                                                                     \
                mem_ptr cptr = { 0 };                                                               \
                                                                                                    \
                for (k = 0; k < m1->dv[1].elems; k++)                                               \
                {                                                                                   \
                    sum += gc_get_##never_type(machine->collector,                                  \
                        m1->value[ i * m1->dv[1].elems + k ])                                       \
                                        *                                                           \
                        gc_get_##never_type(machine->collector,                                     \
                        m2->value[ k * m2->dv[1].elems + j ]);                                      \
                }                                                                                   \
                                                                                                    \
                cptr = gc_alloc_##never_type(machine->collector, sum);                              \
                gc_set_arr_elem(machine->collector, mres, i * dv[1].elems + j, cptr);               \
            }                                                                                       \
        }                                                                                           \
                                                                                                    \
        entry.type = GC_MEM_ADDR;                                                                   \
        entry.addr = gc_alloc_arr_ref(machine->collector, mres);                                    \
                                                                                                    \
        machine->stack[machine->sp - 1] = entry;                                                    \
        machine->sp--;                                                                              \
    }                                                                                               \

vm_execute_op_mul_arr_arr_type(int, int)
vm_execute_op_mul_arr_arr_type(long, long long)
vm_execute_op_mul_arr_arr_type(float, float)
vm_execute_op_mul_arr_arr_type(double, double)

#define vm_execute_op_ass_type(never_type, c_type)              \
    void vm_execute_op_ass_##never_type(vm * machine, bytecode * code)   \
    {                                                           \
        c_type a = gc_get_##never_type(machine->collector,                  \
                        machine->stack[machine->sp].addr);      \
        gc_set_##never_type(machine->collector, machine->stack[machine->sp - 1].addr, a); \
                                                                \
        machine->sp--;                                          \
    }                                                           \

vm_execute_op_ass_type(int, int)
vm_execute_op_ass_type(long, long long)
vm_execute_op_ass_type(float, float)
vm_execute_op_ass_type(double, double)
vm_execute_op_ass_type(char, char)

void vm_execute_op_ass_string(vm * machine, bytecode * code)
{
    mem_ptr str_a = gc_get_string_ref(machine->collector,
                                      machine->stack[machine->sp].addr);
    if (str_a == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    gc_set_string_ref(machine->collector, machine->stack[machine->sp - 1].addr, str_a);

    machine->sp--;
}

void vm_execute_op_ass_c_ptr(vm * machine, bytecode * code)
{
    void * a_ptr = gc_get_c_ptr(machine->collector,
                                machine->stack[machine->sp].addr);
    gc_set_c_ptr(machine->collector, machine->stack[machine->sp - 1].addr, a_ptr);

    machine->sp--;    
}

void vm_execute_op_ass_array(vm * machine, bytecode * code)
{
    mem_ptr array_1 = gc_get_arr_ref(machine->collector,
                                     machine->stack[machine->sp].addr);
    if (array_1 == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    gc_set_arr_ref(machine->collector, machine->stack[machine->sp - 1].addr, array_1);

    machine->sp--;
}

void vm_execute_op_ass_record(vm * machine, bytecode * code)
{
    mem_ptr rec_1 = gc_get_vec_ref(machine->collector,
                                   machine->stack[machine->sp].addr);
    gc_set_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr, rec_1);

    machine->sp--;
}

void vm_execute_op_ass_record_nil(vm * machine, bytecode * code)
{
    mem_ptr nil = gc_get_vec_ref(machine->collector,
                                 machine->stack[machine->sp].addr);
    gc_set_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr, nil);

    machine->sp--;
}

void vm_execute_op_ass_func(vm * machine, bytecode * code)
{
    ip_ptr fptr = gc_get_func_addr(machine->collector,
                                   machine->stack[machine->sp].addr);
    mem_ptr mptr = gc_get_func_vec(machine->collector,
                                   machine->stack[machine->sp].addr);
    if (fptr == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    gc_set_func_addr(machine->collector, machine->stack[machine->sp - 1].addr, fptr);
    gc_set_func_vec(machine->collector, machine->stack[machine->sp - 1].addr, mptr);

    machine->sp--;
}

void vm_execute_jumpz(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp].addr);

    if (a == 0)
    {
        machine->ip = machine->ip + code->jump.offset;
    }
    machine->sp--;
}

void vm_execute_jump(vm * machine, bytecode * code)
{
    machine->ip = machine->ip + code->jump.offset;
}

void vm_execute_label(vm * machine, bytecode * code) { /* no op */ }

void vm_execute_mk_array_num(vm * machine, bytecode * code, param_type value)
{
    gc_stack entry = { 0 };

    unsigned int d = 0;
    unsigned int dims = 0;
    unsigned int elems = 0;
    object_arr_dim * dv = NULL;
    mem_ptr array = { 0 };
    mem_ptr elem = { 0 };

    dims = code->mk_array.dims;
    dv = object_arr_dim_new(dims);
    for (d = 0; d < dims; d++)
    {
        int e =
            gc_get_int(machine->collector, machine->stack[machine->sp--].addr);
        if (e <= 0)
        {
            object_arr_dim_delete(dv);
            print_error_msg(machine->line_no, "array index %d out of bounds",
                            d);
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }
        dv[d].elems = e;
    }

    array = gc_alloc_arr(machine->collector, dims, dv);
    elems = gc_get_arr_elems(machine->collector, array);

    for (d = 0; d < elems; d++)
    {
        if (value == PARAM_INT)
        {
            elem = gc_alloc_int(machine->collector, 0);
        }
        else if (value == PARAM_LONG)
        {
            elem = gc_alloc_long(machine->collector, 0);
        }
        else if (value == PARAM_FLOAT)
        {
            elem = gc_alloc_float(machine->collector, 0.0);
        }
        else if (value == PARAM_DOUBLE)
        {
            elem = gc_alloc_double(machine->collector, 0.0);
        }
        else if (value == PARAM_CHAR)
        {
            elem = gc_alloc_char(machine->collector, 0);
        }
        else if (value == PARAM_STRING)
        {
            elem = gc_alloc_string_ref(machine->collector, nil_ptr);
        }
        else if (value == PARAM_ARRAY)
        {
            elem = gc_alloc_arr_ref(machine->collector, nil_ptr);
        }
        else if (value == PARAM_RANGE)
        {
            elem = gc_alloc_vec_ref(machine->collector, nil_ptr);
        }
        else if (value == PARAM_SLICE)
        {
            elem = gc_alloc_vec_ref(machine->collector, nil_ptr);
        }
        else if (value == PARAM_ENUMTYPE)
        {
            elem = gc_alloc_int(machine->collector, 0);
        }
        else if (value == PARAM_RECORD)
        {
            elem = gc_alloc_vec_ref(machine->collector, nil_ptr);
        }
        else if (value == PARAM_FUNC)
        {
            elem = gc_alloc_func(machine->collector, nil_ptr, nil_ptr);
        }
        else
        {
            assert(0);
        }
        gc_set_arr_elem(machine->collector, array, d, elem);
    }

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_arr_ref(machine->collector, array);

    machine->stack[machine->sp] = entry;
}

void vm_execute_mk_array_int(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_INT);
}

void vm_execute_mk_array_long(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_LONG);
}

void vm_execute_mk_array_float(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_FLOAT);
}

void vm_execute_mk_array_double(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_DOUBLE);
}

void vm_execute_mk_array_char(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_CHAR);
}

void vm_execute_mk_array_string(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_STRING);
}

void vm_execute_mk_array_array(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_ARRAY);
}

void vm_execute_mk_array_record(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_RECORD);
}

void vm_execute_mk_array_func(vm * machine, bytecode * code)
{
    vm_execute_mk_array_num(machine, code, PARAM_FUNC);
}

void vm_execute_mk_init_array(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    unsigned int d = 0;
    unsigned int dims = 0;
    unsigned int elems = 0;
    object_arr_dim * dv = NULL;
    mem_ptr array = { 0 };

    dims = code->mk_array.dims;
    dv = object_arr_dim_new(dims);

    for (d = 0; d < dims; d++)
    {
        dv[d].elems =
            gc_get_int(machine->collector, machine->stack[machine->sp--].addr);
    }

    array = gc_alloc_arr(machine->collector, dims, dv);
    elems = gc_get_arr_elems(machine->collector, array);
    for (d = 0; d < elems; d++)
    {
        gc_set_arr_elem(machine->collector, array, d,
                        machine->stack[machine->sp--].addr);
    }

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_arr_ref(machine->collector, array);

    machine->stack[machine->sp] = entry;
}

void vm_execute_mk_range(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr range = { 0 };
    unsigned int d = 0;
    unsigned int dims = 0;

    dims = 2 * code->mk_range.dims;
    range = gc_alloc_vec(machine->collector, dims);

    for (d = 0;  d < dims; d++)
    {
        gc_set_vec(machine->collector, range, d,
                    machine->stack[machine->sp--].addr);
    }

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_vec_ref(machine->collector, range);

    machine->stack[machine->sp] = entry;
}

/**
 *  [a .. b] [ c .. d ]
 *   a < b     c < d     a+c .. a+d  a+d <= b
 *   a > b     c < d     a-c .. a-d  a-d >= b
 *   a < b     c > d     a+c .. a+d  a+c <= b
 *   a > b     c > d     a-c .. a-d  a-c >= b
 */
void vm_get_slice_range(
    int range1_from, int range1_to,
    int range2_from, int range2_to,
    int * res_from,  int * res_to, int * oob)
{
    if (range1_from < range1_to)
    {
        *res_from = range1_from + range2_from;
        *res_to = range1_from + range2_to;

        if (range2_from < range2_to)
        {
            if (*res_to > range1_to)
            {
                *oob = 1;
                return;
            }
        }
        else
        {
            if (*res_from > range1_to)
            {
                *oob = 1;
                return;
            }
        }
    }
    else
    {
        *res_from = range1_from - range2_from;
        *res_to = range1_from - range2_to;

        if (range2_from < range2_to)
        {
            if (*res_to < range1_to)
            {
                *oob = 1;
                return;
            }
        }
        else
        {
            if (*res_from < range1_to)
            {
                *oob = 1;
                return;
            }
        }
    }
}

void vm_execute_slice_array(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr array = { 0 };
    mem_ptr range = { 0 };

    array = gc_get_arr_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    range = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    if (array == nil_ptr || range == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    mem_ptr slice = { 0 };
    slice = gc_alloc_vec(machine->collector, SLICE_SIZE);

    gc_set_vec(machine->collector, slice, SLICE_ARRAY_INDEX, array);
    gc_set_vec(machine->collector, slice, SLICE_RANGE_INDEX, range);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_vec_ref(machine->collector, slice);

    machine->sp--;
    machine->stack[machine->sp] = entry;
}

void vm_execute_slice_range(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr range1 = gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr range2 = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    if (range1 == nil_ptr || range2 == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    unsigned int d = 0;
    unsigned int dims = 0;
    mem_ptr range = { 0 };

    dims = code->mk_slice.dims;
    range = gc_alloc_vec(machine->collector, 2 * dims);

    for (d = 0; d < dims; d++)
    {
        int oob = 0;
        int res_from = 0;
        int res_to = 0;
        mem_ptr res_from_addr = { 0 };
        mem_ptr res_to_addr = { 0 };

        mem_ptr range1_from_addr = gc_get_vec(machine->collector, range1, d * 2);
        mem_ptr range1_to_addr = gc_get_vec(machine->collector, range1, d * 2 + 1);

        mem_ptr range2_from_addr = gc_get_vec(machine->collector, range2, d * 2);
        mem_ptr range2_to_addr = gc_get_vec(machine->collector, range2, d * 2 + 1);

        int range1_from = gc_get_int(machine->collector, range1_from_addr);
        int range1_to = gc_get_int(machine->collector, range1_to_addr);
        int range2_from = gc_get_int(machine->collector, range2_from_addr);
        int range2_to = gc_get_int(machine->collector, range2_to_addr);

        /* TODO: remove this
        printf("slice range range_dims %d\n", dims);
        printf("[%d..%d][%d..%d]\n", range1_from, range1_to,
                                     range2_from, range2_to);
        */

        vm_get_slice_range(range1_from, range1_to,
                           range2_from, range2_to,
                           &res_from, &res_to, &oob);
        if (oob)
        {
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }

        res_from_addr = gc_alloc_int(machine->collector, res_from);
        res_to_addr = gc_alloc_int(machine->collector, res_to);

        gc_set_vec(machine->collector, range, 2 * d, res_from_addr);
        gc_set_vec(machine->collector, range, 2 * d + 1, res_to_addr);
    }

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_vec_ref(machine->collector, range);

    machine->sp--;
    machine->stack[machine->sp] = entry;
}

void vm_execute_slice_slice(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr slice = gc_get_vec_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr range = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);
    if (slice == nil_ptr || range == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    mem_ptr slice_array = gc_get_vec(machine->collector, slice, SLICE_ARRAY_INDEX);
    mem_ptr slice_range = gc_get_vec(machine->collector, slice, SLICE_RANGE_INDEX);
    if (slice_range == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    /* slice slice */
    unsigned int d = 0;
    unsigned int dims = 0;
    mem_ptr range_res = { 0 };

    dims = code->mk_slice.dims;
    range_res = gc_alloc_vec(machine->collector, 2 * dims);

    for (d = 0; d < dims; d++)
    {
        int oob = 0;
        int res_from = 0;
        int res_to = 0;
        mem_ptr res_from_addr = { 0 };
        mem_ptr res_to_addr = { 0 };

        mem_ptr range1_from_addr = gc_get_vec(machine->collector, slice_range, d * 2);
        mem_ptr range1_to_addr = gc_get_vec(machine->collector, slice_range, d * 2 + 1);

        mem_ptr range2_from_addr = gc_get_vec(machine->collector, range, d * 2);
        mem_ptr range2_to_addr = gc_get_vec(machine->collector, range, d * 2 + 1);

        int range1_from = gc_get_int(machine->collector, range1_from_addr);
        int range1_to = gc_get_int(machine->collector, range1_to_addr);
        int range2_from = gc_get_int(machine->collector, range2_from_addr);
        int range2_to = gc_get_int(machine->collector, range2_to_addr);

        /* TODO: remove this
        printf("slice slice range_dims %d\n", dims);
        printf("[%d..%d][%d..%d]\n", range1_from, range1_to,
                                     range2_from, range2_to);
        */

        vm_get_slice_range(range1_from, range1_to,
                           range2_from, range2_to,
                           &res_from, &res_to, &oob);
        if (oob)
        {
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }

        res_from_addr = gc_alloc_int(machine->collector, res_from);
        res_to_addr = gc_alloc_int(machine->collector, res_to);

        gc_set_vec(machine->collector, range_res, 2 * d, res_from_addr);
        gc_set_vec(machine->collector, range_res, 2 * d + 1, res_to_addr);
    }

    /* create slice */
    mem_ptr slice_res = { 0 };
    slice_res = gc_alloc_vec(machine->collector, SLICE_SIZE);

    gc_set_vec(machine->collector, slice_res, SLICE_ARRAY_INDEX, slice_array);
    gc_set_vec(machine->collector, slice_res, SLICE_RANGE_INDEX, range_res);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_vec_ref(machine->collector, slice_res);

    machine->sp--;
    machine->stack[machine->sp] = entry;
}

void vm_execute_slice_string(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr string = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr range = gc_get_vec_ref(machine->collector, machine->stack[machine->sp].addr);

    mem_ptr range_from_addr = gc_get_vec(machine->collector, range, 0);
    mem_ptr range_to_addr = gc_get_vec(machine->collector, range, 1);

    int range_from = gc_get_int(machine->collector, range_from_addr);
    int range_to = gc_get_int(machine->collector, range_to_addr);
    int string_len = gc_get_string_len(machine->collector, string);

    if (range_from < 0 ||
        range_to < 0 ||
        range_from >= string_len ||
        range_to >= string_len)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NO_INDEX_OOB;
        return;
    }

    char * string_res = NULL;
    if (range_from < range_to)
    {
        unsigned int sl = range_to - range_from + 1;
        string_res = strndup(gc_get_string(machine->collector, string) + range_from, sl);
    }
    else
    {
        unsigned int i = 0;
        unsigned int sl = range_from - range_to + 1;
        string_res = strndup(gc_get_string(machine->collector, string) + range_to, sl);
        for (i = 0 ; i < sl / 2; i++)
        {
            char tmp;
            tmp = string_res[i];
            string_res[i] = string_res[sl - i - 1];
            string_res[sl - i - 1] = tmp;
        }
    }

    mem_ptr string_res_addr = gc_alloc_string_take(machine->collector, string_res);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_string_ref(machine->collector, string_res_addr);

    machine->sp--;
    machine->stack[machine->sp] = entry;
}

inline void vm_execute_array_deref_univ(
    vm * machine,
    bytecode * code,
    mem_ptr (* gc_get_array) (gc * collector, mem_ptr addr))
{
    gc_stack entry = { 0 };

    unsigned int d = 0;
    unsigned int dims = 0;
    object_arr_dim * addr = NULL;

    dims = code->array_deref.dims;
    addr = object_arr_dim_new(dims);
    for (d = 0; d < dims; d++)
    {
        int e =
            gc_get_int(machine->collector, machine->stack[machine->sp--].addr);
        if (e < 0)
        {
            object_arr_dim_delete(addr);
            print_error_msg(machine->line_no, "array index %d out of bounds",
                            d);
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }
        addr[d].mult = e;
    }

    mem_ptr array = { 0 };
    array = gc_get_array(machine->collector, machine->stack[machine->sp--].addr);
    if (array == nil_ptr)
    {
        object_arr_dim_delete(addr);
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    assert(gc_get_arr_dims(machine->collector, array) == dims);

    int oobounds = 0;
    unsigned int elem_index = 0;
    object_arr_dim * dv = NULL;

    dv = gc_get_arr_dv(machine->collector, array);
    elem_index = object_arr_dim_addr(dims, dv, addr, &oobounds);
    object_arr_dim_delete(addr);

    if (oobounds >= 0)
    {
        print_error_msg(machine->line_no, "array index %d out of bounds",
                        oobounds);
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NO_INDEX_OOB;
        return;
    }

    mem_ptr elem = { 0 };
    elem = gc_get_arr_elem(machine->collector, array, elem_index);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = elem;

    machine->stack[machine->sp] = entry;
}

void vm_execute_array_deref(vm * machine, bytecode * code)
{
    vm_execute_array_deref_univ(machine, code, gc_get_arr);
}

void vm_execute_arrayref_deref(vm * machine, bytecode * code)
{
    vm_execute_array_deref_univ(machine, code, gc_get_arr_ref);
}

void vm_execute_range_deref(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    unsigned int d = 0;
    unsigned int dims = 0;

    dims = code->array_deref.dims;

    mem_ptr range = { 0 };
    range = gc_get_vec_ref(machine->collector, machine->stack[machine->sp - dims].addr);
    if (range == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    object_arr_dim * dv = NULL;
    dv = object_arr_dim_new(1);
    dv[0].elems = dims;
    dv[0].mult = 1;

    mem_ptr array = { 0 };
    array = gc_alloc_arr(machine->collector, 1, dv);

    for (d = 0; d < dims; d++)
    {
        mem_ptr range_from_addr = { 0 };
        mem_ptr range_to_addr = { 0 };
        mem_ptr res_from_addr = { 0 };
        int oob = 0;
        int res_from = 0;
        int res_to = 0;
        int range_from = 0;
        int range_to = 0;
        int range_indx = 0;

        range_from_addr = gc_get_vec(machine->collector, range, d * 2);
        range_to_addr = gc_get_vec(machine->collector, range, d * 2 + 1);
        range_from = gc_get_int(machine->collector, range_from_addr);
        range_to = gc_get_int(machine->collector, range_to_addr);
        range_indx = gc_get_int(machine->collector, machine->stack[machine->sp--].addr);

        if (range_indx < 0)
        {
            print_error_msg(machine->line_no, "range index %d out of bounds", d);
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }

        vm_get_slice_range(range_from, range_to,
                           range_indx, range_indx,
                           &res_from, &res_to, &oob);
        if (oob)
        {
            print_error_msg(machine->line_no, "range index %d out of bounds", d);
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }

        res_from_addr = gc_alloc_int(machine->collector, res_from);
        gc_set_arr_elem(machine->collector, array, d, res_from_addr);
    }

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_arr_ref(machine->collector, array);

    machine->stack[machine->sp] = entry;
}

void vm_execute_slice_deref(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    unsigned int d = 0;
    unsigned int dims = 0;
    object_arr_dim * addr = NULL;

    dims = code->array_deref.dims;
    addr = object_arr_dim_new(dims);
    for (d = 0; d < dims; d++)
    {
        int e = gc_get_int(machine->collector, machine->stack[machine->sp--].addr);
        if (e < 0)
        {
            object_arr_dim_delete(addr);
            print_error_msg(machine->line_no, "slice index %d out of bounds", d);
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }
        addr[d].mult = e;
    }

    mem_ptr slice = gc_get_vec_ref(machine->collector, machine->stack[machine->sp--].addr);
    if (slice == nil_ptr)
    {
        object_arr_dim_delete(addr);
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    mem_ptr array = gc_get_vec(machine->collector, slice, SLICE_ARRAY_INDEX);
    mem_ptr range = gc_get_vec(machine->collector, slice, SLICE_RANGE_INDEX);
    if (array == nil_ptr || range == nil_ptr)
    {
        object_arr_dim_delete(addr);
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    assert(gc_get_arr_dims(machine->collector, array) == dims);

    for (d = 0; d < dims; d++)
    {
        mem_ptr range_from_addr = { 0 };
        mem_ptr range_to_addr = { 0 };
        int oob = 0;
        int res_from = 0;
        int res_to = 0;
        int range_from = 0;
        int range_to = 0;

        range_from_addr = gc_get_vec(machine->collector, range, 2 * d);
        range_to_addr = gc_get_vec(machine->collector, range, 2 * d + 1);
        range_from = gc_get_int(machine->collector, range_from_addr);
        range_to = gc_get_int(machine->collector, range_to_addr);

        vm_get_slice_range(range_from, range_to,
                           addr[d].mult, addr[d].mult,
                           &res_from, &res_to, &oob);
        if (oob)
        {
            object_arr_dim_delete(addr);
            print_error_msg(machine->line_no, "slice index %d out of bounds", d);
            machine->running = VM_EXCEPTION;
            machine->exception = EXCEPT_NO_INDEX_OOB;
            return;
        }
        addr[d].mult = res_from;
    }

    int oobounds = 0;
    unsigned int elem_index = 0;
    object_arr_dim * dv = NULL;

    dv = gc_get_arr_dv(machine->collector, array);
    elem_index = object_arr_dim_addr(dims, dv, addr, &oobounds);
    object_arr_dim_delete(addr);

    if (oobounds >= 0)
    {
        print_error_msg(machine->line_no, "slice index %d out of bounds",
                        oobounds);
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NO_INDEX_OOB;
        return;
    }

    mem_ptr elem = { 0 };
    elem = gc_get_arr_elem(machine->collector, array, elem_index);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = elem;

    machine->stack[machine->sp] = entry;
}

void vm_execute_string_deref(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    char c = 0;

    mem_ptr str_ptr = gc_get_string_ref(machine->collector, machine->stack[machine->sp - 1].addr);
    int index = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    if (str_ptr == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    char * str = gc_get_string(machine->collector, str_ptr);
    if (index >= (int)strlen(str))
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NO_INDEX_OOB;
        return;
    }

    c = str[index];

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_char(machine->collector, c);

    machine->sp--;
    machine->stack[machine->sp] = entry;
}

void vm_execute_array_append(vm * machine, bytecode * code)
{
    mem_ptr addr = machine
                       ->stack[machine->sp - (code->id_local.stack_level -
                                              code->id_local.index)]
                       .addr;
    mem_ptr array = gc_get_arr_ref(machine->collector, addr);
    if (array == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    mem_ptr obj = machine->stack[machine->sp--].addr;

    gc_append_arr_elem(machine->collector, array, obj);
}

void vm_execute_record(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_vec(machine->collector, code->record.count);

    unsigned int c;
    for (c = 0; c < code->record.count; c++)
    {
        gc_set_vec(machine->collector, addr, c,
                   machine->stack[machine->sp--].addr);
    }

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = gc_alloc_vec_ref(machine->collector, addr);

    machine->stack[machine->sp] = entry;
}

void vm_execute_vec_deref(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr vec = machine->stack[machine->sp - code->attr.stack_level].addr;
    mem_ptr addr = gc_get_vec(machine->collector, vec, code->attr.index);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_vecref_deref(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr vecref = machine->stack[machine->sp].addr;
    mem_ptr vec = gc_get_vec_ref(machine->collector, vecref);
    if (vec == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    entry.type = GC_MEM_ADDR;
    entry.addr = vec;

    machine->stack[machine->sp] = entry;
}

void vm_execute_vecref_vec_deref(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    mem_ptr record_ref = machine->stack[machine->sp - code->attr.stack_level].addr;
    mem_ptr record_value = gc_get_vec_ref(machine->collector, record_ref);
    if (record_value == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    /* TODO: consider changing into assert as vectors are used only intenally
     * and should always correctly calculated indexed
     */
    unsigned int size = gc_get_vec_size(machine->collector, record_value);
    if (code->attr.index >= size)
    {
        print_error_msg(machine->line_no, "attribute index out of bounds");
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NO_INDEX_OOB;
        return;
    }

    mem_ptr addr =
        gc_get_vec(machine->collector, record_value, code->attr.index);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_nil_record_ref(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_vec_ref(machine->collector, nil_ptr);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_func_def(vm * machine, bytecode * code) { /* no op */ }

void vm_execute_func_obj(vm * machine, bytecode * code) { /* no op */ }

void vm_execute_dup(vm * machine, bytecode * code)
{
    machine->sp++;
    machine->stack[machine->sp] = machine->stack[machine->sp - code->dup.n];
}

void vm_execute_global_vec(vm * machine, bytecode * code)
{
    int c;
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_vec(machine->collector, code->global_vec.count);

    for (c = code->global_vec.count - 1; c >= 0; c--)
    {
        gc_set_vec(machine->collector, addr, c,
                   machine->stack[machine->sp--].addr);
    }

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_mark(vm * machine, bytecode * code)
{
    gc_stack entry1 = { 0 };
    gc_stack entry2 = { 0 };
    gc_stack entry3 = { 0 };
    gc_stack entryL = { 0 };
    gc_stack entryP = { 0 };

    entry3.type = GC_MEM_IP;
    entry3.ip = code->mark.addr;

    entry2.type = GC_MEM_STACK;
    entry2.addr = machine->fp;

    entry1.type = GC_MEM_ADDR;
    entry1.addr = machine->gp;

    entryL.type = GC_MEM_IP;
    entryL.ip = machine->line_no;

    entryP.type = GC_MEM_STACK;
    entryP.addr = machine->pp;

    machine->stack[machine->sp + 5] = entry3;
    machine->stack[machine->sp + 4] = entry2;
    machine->stack[machine->sp + 3] = entry1;
    machine->stack[machine->sp + 2] = entryL;
    machine->stack[machine->sp + 1] = entryP;

    machine->fp = machine->sp = machine->sp + 5;
    vm_check_stack(machine);
}

void vm_execute_call(vm * machine, bytecode * code)
{
    mem_ptr gp =
        gc_get_func_vec(machine->collector, machine->stack[machine->sp].addr);
    ip_ptr ip =
        gc_get_func_addr(machine->collector, machine->stack[machine->sp].addr);
    if (ip == nil_ptr)
    {
        machine->running = VM_EXCEPTION;
        machine->exception = EXCEPT_NIL_POINTER;
        return;
    }

    machine->gp = gp;
    machine->ip = ip;
    machine->pp = machine->fp;
    machine->sp--;
}

/** move m items from the top of the stock q items down */
void vm_execute_slide(vm * machine, bytecode * code)
{
    if (code->slide.q == 0)
    {
        return;
    }

    if (code->slide.m == 0)
    {
        machine->sp = machine->sp - code->slide.q;
    }
    else
    {
        unsigned int i;
        machine->sp = machine->sp - code->slide.q - code->slide.m;

        for (i = 0; i < code->slide.m; i++)
        {
            machine->sp++;
            machine->stack[machine->sp] =
                machine->stack[machine->sp + code->slide.q];
        }
    }

    gc_run(machine->collector, machine->stack, machine->sp + 1, machine->gp);
}

void vm_execute_clear_stack(vm * machine, bytecode * code)
{
    /* actualy there should be a better way to remove local params */
    /* when variadic parametes (...) are introduced machine->params_pointers */
    /* is likely needed  */
    unsigned int param_count = code->clear.count;
    machine->fp = machine->pp;
    machine->sp = machine->fp + param_count;

    machine->running = VM_RUNNING;
}

void vm_execute_ret(vm * machine, bytecode * code)
{
    machine->gp = machine->stack[machine->fp - 2].addr;
    machine->ip = machine->stack[machine->fp].ip;
    machine->pp = machine->stack[machine->fp - 4].addr;
    machine->stack[machine->fp - 4] = machine->stack[machine->sp];
    machine->sp = machine->fp - 4;
    machine->fp = machine->stack[machine->fp - 1].sp;

    gc_run(machine->collector, machine->stack, machine->sp + 1, machine->gp);
}

void vm_execute_rethrow(vm * machine, bytecode * code)
{
    vm_execute_ret(machine, code);
    machine->running = VM_EXCEPTION;
}

void vm_execute_line(vm * machine, bytecode * code)
{
    machine->line_no = code->line.no;
}

void vm_execute_build_in(vm * machine, bytecode * code)
{
    libvm_execute_build_in(machine, code);
}

void vm_execute_copyglob(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = machine->gp;

    machine->stack[machine->sp] = entry;
}

void vm_execute_alloc(vm * machine, bytecode * code)
{
    unsigned int i;

    for (i = 0; i < code->alloc.n; i++)
    {
        gc_stack entry = { 0 };
        mem_ptr addr = gc_alloc_func(machine->collector, 0, 0);

        entry.type = GC_MEM_ADDR;
        entry.addr = addr;

        machine->sp++;
        machine->stack[machine->sp] = entry;
    }

    vm_check_stack(machine);
}

void vm_execute_rewrite(vm * machine, bytecode * code)
{
    unsigned int j = code->rewrite.j;

    mem_ptr gp =
        gc_get_func_vec(machine->collector, machine->stack[machine->sp].addr);
    ip_ptr ip =
        gc_get_func_addr(machine->collector, machine->stack[machine->sp].addr);

    gc_set_func_vec(machine->collector, machine->stack[machine->sp - j].addr,
                    gp);
    gc_set_func_addr(machine->collector, machine->stack[machine->sp - j].addr,
                     ip);

    machine->sp--;
    vm_check_stack(machine);
}

void vm_execute_push_param(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = { 0 };
    unsigned int i;

    for (i = machine->prog->params_count; i > 0; i--)
    {
        if (machine->prog->params[i - 1].type == OBJECT_INT)
        {
            addr = gc_alloc_int(machine->collector,
                                machine->prog->params[i - 1].int_value);
        }
        else if (machine->prog->params[i - 1].type == OBJECT_FLOAT)
        {
            addr = gc_alloc_float(machine->collector,
                                  machine->prog->params[i - 1].float_value);
        }
        else
        {
            printf("unsupported type\n");
            assert(0);
        }

        machine->sp++;
        vm_check_stack(machine);

        entry.type = GC_MEM_ADDR;
        entry.addr = addr;

        machine->stack[machine->sp] = entry;
    }
}

void vm_execute_push_except(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_int(machine->collector, machine->exception);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_unhandled_exception(vm * machine, bytecode * code)
{
    printf("unhandled %s exception\n", except_to_str(machine->exception));
    machine->running = VM_ERROR;
}

void vm_execute_halt(vm * machine, bytecode * code)
{
    machine->running = VM_HALT;
}

int vm_execute(vm * machine, program * prog, object * result)
{
    bytecode * bc = NULL;

    machine->prog = prog;
    machine->running = VM_RUNNING;
    while (machine->running == VM_RUNNING)
    {
        bc = prog->module_value->code_arr + machine->ip;
        machine->ip++;
        vm_execute_op[bc->type].execute(machine, bc);

        if (machine->running == VM_EXCEPTION)
        {
            machine->ip = exception_tab_search(prog->module_value->exctab_value,
                                               machine->ip - 1);
            machine->running = VM_RUNNING;
        }
    }

    if (machine->running == VM_ERROR)
    {
        vm_print(machine, "VM_ERROR");
        vm_print_stack_trace(machine);    
    }
    else if (machine->running == VM_HALT)
    {
        *result =
            *gc_get_object(machine->collector, machine->stack[machine->sp].addr);
        return 0;
    }

    return 1;
}

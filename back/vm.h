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
#ifndef __VM_H__
#define __VM_H__

#include "bytecode.h"
#include "program.h"
#include "except.h"

typedef struct gc gc;

typedef enum vm_state
{
    VM_HALT = 0,
    VM_RUNNING = 1,
    VM_EXCEPTION = 2,
    VM_ERROR = 3
} vm_state;

typedef struct vm
{
    stack_ptr sp;
    stack_ptr fp;
    stack_ptr pp;
    mem_ptr gp;
    ip_ptr ip;

    int stack_size;
    struct gc_stack * stack;
    struct gc * collector;
    struct dlcache * dlib_cache;

    except_no exception;
    unsigned int line_no;
    vm_state running;

    program * prog;
} vm;

typedef struct vm_execute_str
{
    bytecode_type type;
    void (*execute)(vm * machine, bytecode * code);
} vm_execute_str;

void vm_execute_unknown(vm * machine, bytecode * code);

void vm_execute_int(vm * machine, bytecode * code);
void vm_execute_float(vm * machine, bytecode * code);
void vm_execute_char(vm * machine, bytecode * code);
void vm_execute_string(vm * machine, bytecode * code);

void vm_execute_id_local(vm * machine, bytecode * code);
void vm_execute_id_dim_local(vm * machine, bytecode * code);
void vm_execute_id_dim_slice(vm * machine, bytecode * code);
void vm_execute_id_global(vm * machine, bytecode * code);
void vm_execute_id_func_func(vm * machine, bytecode * code);
void vm_execute_id_func_addr(vm * machine, bytecode * code);

void vm_execute_op_neg_int(vm * machine, bytecode * code);
void vm_execute_op_add_int(vm * machine, bytecode * code);
void vm_execute_op_sub_int(vm * machine, bytecode * code);
void vm_execute_op_mul_int(vm * machine, bytecode * code);
void vm_execute_op_div_int(vm * machine, bytecode * code);
void vm_execute_op_mod_int(vm * machine, bytecode * code);

void vm_execute_op_neg_float(vm * machine, bytecode * code);
void vm_execute_op_add_float(vm * machine, bytecode * code);
void vm_execute_op_sub_float(vm * machine, bytecode * code);
void vm_execute_op_mul_float(vm * machine, bytecode * code);
void vm_execute_op_div_float(vm * machine, bytecode * code);

void vm_execute_op_add_string(vm * machine, bytecode * code);
void vm_execute_op_add_int_string(vm * machine, bytecode * code);
void vm_execute_op_add_string_int(vm * machine, bytecode * code);
void vm_execute_op_add_float_string(vm * machine, bytecode * code);
void vm_execute_op_add_string_float(vm * machine, bytecode * code);

void vm_execute_op_lt_int(vm * machine, bytecode * code);
void vm_execute_op_gt_int(vm * machine, bytecode * code);
void vm_execute_op_lte_int(vm * machine, bytecode * code);
void vm_execute_op_gte_int(vm * machine, bytecode * code);
void vm_execute_op_eq_int(vm * machine, bytecode * code);
void vm_execute_op_neq_int(vm * machine, bytecode * code);

void vm_execute_op_lt_float(vm * machine, bytecode * code);
void vm_execute_op_gt_float(vm * machine, bytecode * code);
void vm_execute_op_lte_float(vm * machine, bytecode * code);
void vm_execute_op_gte_float(vm * machine, bytecode * code);
void vm_execute_op_eq_float(vm * machine, bytecode * code);
void vm_execute_op_neq_float(vm * machine, bytecode * code);

void vm_execute_op_lt_char(vm * machine, bytecode * code);
void vm_execute_op_gt_char(vm * machine, bytecode * code);
void vm_execute_op_lte_char(vm * machine, bytecode * code);
void vm_execute_op_gte_char(vm * machine, bytecode * code);
void vm_execute_op_eq_char(vm * machine, bytecode * code);
void vm_execute_op_neq_char(vm * machine, bytecode * code);

void vm_execute_op_eq_string(vm * machine, bytecode * code);
void vm_execute_op_neq_string(vm * machine, bytecode * code);

void vm_execute_op_eq_nil(vm * machine, bytecode * code);

void vm_execute_op_eq_string_nil(vm * machine, bytecode * code);
void vm_execute_op_eq_array_nil(vm * machine, bytecode * code);
void vm_execute_op_eq_record_nil(vm * machine, bytecode * code);
void vm_execute_op_eq_func_nil(vm * machine, bytecode * code);

void vm_execute_op_eq_nil_string(vm * machine, bytecode * code);
void vm_execute_op_eq_nil_array(vm * machine, bytecode * code);
void vm_execute_op_eq_nil_record(vm * machine, bytecode * code);
void vm_execute_op_eq_nil_func(vm * machine, bytecode * code);

void vm_execute_op_neq_nil(vm * machine, bytecode * code);

void vm_execute_op_neq_string_nil(vm * machine, bytecode * code);
void vm_execute_op_neq_array_nil(vm * machine, bytecode * code);
void vm_execute_op_neq_record_nil(vm * machine, bytecode * code);
void vm_execute_op_neq_func_nil(vm * machine, bytecode * code);

void vm_execute_op_neq_nil_string(vm * machine, bytecode * code);
void vm_execute_op_neq_nil_array(vm * machine, bytecode * code);
void vm_execute_op_neq_nil_record(vm * machine, bytecode * code);
void vm_execute_op_neq_nil_func(vm * machine, bytecode * code);

void vm_execute_op_not_int(vm * machine, bytecode * code);

void vm_execute_op_inc_int(vm * machine, bytecode * code);
void vm_execute_op_dec_int(vm * machine, bytecode * code);
void vm_execute_op_dup_int(vm * machine, bytecode * code);

void vm_execute_int_to_float(vm * machine, bytecode * code);
void vm_execute_float_to_int(vm * machine, bytecode * code);

void vm_execute_op_neg_arr_int(vm * machine, bytecode * code);
void vm_execute_op_neg_arr_float(vm * machine, bytecode * code);
void vm_execute_op_add_arr_int(vm * machine, bytecode * code);
void vm_execute_op_add_arr_float(vm * machine, bytecode * code);
void vm_execute_op_sub_arr_int(vm * machine, bytecode * code);
void vm_execute_op_sub_arr_float(vm * machine, bytecode * code);
void vm_execute_op_mul_arr_int(vm * machine, bytecode * code);
void vm_execute_op_mul_arr_float(vm * machine, bytecode * code);
void vm_execute_op_mul_arr_arr_int(vm * machine, bytecode * code);
void vm_execute_op_mul_arr_arr_float(vm * machine, bytecode * code);

void vm_execute_op_ass_int(vm * machine, bytecode * code);
void vm_execute_op_ass_float(vm * machine, bytecode * code);
void vm_execute_op_ass_char(vm * machine, bytecode * code);
void vm_execute_op_ass_string(vm * machine, bytecode * code);
void vm_execute_op_ass_array(vm * machine, bytecode * code);
void vm_execute_op_ass_record(vm * machine, bytecode * code);
void vm_execute_op_ass_func(vm * machine, bytecode * code);

void vm_execute_op_ass_record_nil(vm * machine, bytecode * code);

void vm_execute_jumpz(vm * machine, bytecode * code);
void vm_execute_jump(vm * machine, bytecode * code);
void vm_execute_label(vm * machine, bytecode * code);

void vm_execute_mk_array_int(vm * machine, bytecode * code);
void vm_execute_mk_array_float(vm * machine, bytecode * code);
void vm_execute_mk_array_char(vm * machine, bytecode * code);
void vm_execute_mk_array_string(vm * machine, bytecode * code);
void vm_execute_mk_array_array(vm * machine, bytecode * code);
void vm_execute_mk_array_record(vm * machine, bytecode * code);
void vm_execute_mk_array_func(vm * machine, bytecode * code);
void vm_execute_mk_init_array(vm * machine, bytecode * code);
void vm_execute_mk_range(vm * machine, bytecode * code);

void vm_get_slice_range(int range1_from, int range1_to,
                        int range2_from, int range2_to,
                        int * res_from,  int * res_to, int * oob);
void vm_execute_slice_array(vm * machine, bytecode * code);
void vm_execute_slice_range(vm * machine, bytecode * code);
void vm_execute_slice_slice(vm * machine, bytecode * code);
void vm_execute_slice_string(vm * machine, bytecode * code);

void vm_execute_array_deref_univ(vm * machine, bytecode * code,  mem_ptr (* gc_get_array) (gc * collector, mem_ptr addr));
void vm_execute_array_deref(vm * machine, bytecode * code);
void vm_execute_arrayref_deref(vm * machine, bytecode * code);
void vm_execute_range_deref(vm * machine, bytecode * code);
void vm_execute_slice_deref(vm * machine, bytecode * code);
void vm_execute_string_deref(vm * machine, bytecode * code);
void vm_execute_array_append(vm * machine, bytecode * code);

void vm_execute_record(vm * machine, bytecode * code);
void vm_execute_vec_deref(vm * machine, bytecode * code);
void vm_execute_vecref_deref(vm * machine, bytecode * code);
void vm_execute_vecref_vec_deref(vm * machine, bytecode * code);
void vm_execute_nil_record_ref(vm * machine, bytecode * code);

void vm_execute_func_def(vm * machine, bytecode * code);
void vm_execute_func_obj(vm * machine, bytecode * code);
void vm_execute_func_ffi(vm * machine, bytecode * code);
void vm_execute_func_ffi_int(vm * machine, bytecode * code);
void vm_execute_func_ffi_float(vm * machine, bytecode * code);
void vm_execute_func_ffi_char(vm * machine, bytecode * code);
void vm_execute_func_ffi_string(vm * machine, bytecode * code);

void vm_execute_dup(vm * machine, bytecode * code);
void vm_execute_global_vec(vm * machine, bytecode * code);
void vm_execute_mark(vm * machine, bytecode * code);
void vm_execute_call(vm * machine, bytecode * code);
void vm_execute_slide(vm * machine, bytecode * code);
void vm_execute_clear_stack(vm * machine, bytecode * code);
void vm_execute_ret(vm * machine, bytecode * code);
void vm_execute_line(vm * machine, bytecode * code);
void vm_execute_build_in(vm * machine, bytecode * code);
void vm_execute_copyglob(vm * machine, bytecode * code);
void vm_execute_alloc(vm * machine, bytecode * code);
void vm_execute_rewrite(vm * machine, bytecode * code);
void vm_execute_push_param(vm * machine, bytecode * code);
void vm_execute_push_except(vm * machine, bytecode * code);
void vm_execute_rethrow(vm * machine, bytecode * code);

void vm_execute_unhandled_exception(vm * machine, bytecode * code);
void vm_execute_halt(vm * machine, bytecode * code);

void vm_check_stack(vm * machine);
int vm_execute(vm * machine, program * prog, object * result);

vm * vm_new(unsigned int mem_size, unsigned int stack_size);
void vm_delete(vm * machine);

void vm_print_stack_trace(vm * machine);
void vm_print(vm * machine);

#endif /* __VM_H__ */

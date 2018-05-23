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

typedef enum bytecode_type
{
    BYTECODE_UNKNOWN = 0,
    BYTECODE_FLOAT,
    BYTECODE_ID_LOCAL,
    BYTECODE_ID_GLOBAL,
    BYTECODE_ID_FUNC_FUNC,
    BYTECODE_ID_FUNC_ADDR,
    BYTECODE_JUMPZ,
    BYTECODE_JUMP,
    BYTECODE_LABEL,
    BYTECODE_OP_NEG,
    BYTECODE_OP_ADD,
    BYTECODE_OP_SUB,
    BYTECODE_OP_MUL,
    BYTECODE_OP_DIV,
    BYTECODE_OP_LT,
    BYTECODE_OP_GT,
    BYTECODE_OP_LTE,
    BYTECODE_OP_GTE,
    BYTECODE_OP_EQ,
    BYTECODE_OP_NEQ,
    BYTECODE_FUNC_DEF,
    BYTECODE_GLOBAL_VEC,
    BYTECODE_MARK,
    BYTECODE_CALL,
    BYTECODE_RET,
    BYTECODE_LINE,
    BYTECODE_BUILD_IN,
    BYTECODE_HALT,
    BYTECODE_END
} bytecode_type;

typedef struct bytecode
{
    bytecode_type type;
    unsigned int addr;
    union
    {
        struct
        {
            float value;     /* BYTECODE_FLOAT */
        } real;
        struct             /* BYTECODE_ID_LOCAL */
        {
            int stack_level;
            int index;            
        } id_local;
        struct             /* BYTECODE_ID_GLOBAL */ 
        {
            int index;
        } id_global;
        union              /* BYTECODE_ID_FUNC_FUNC BYTECODE_ID_FUNC_ADDR */
        {
            struct func * func_value;
            unsigned int func_addr;
        } id_func;
        struct             /* BYTECODE_JUMPZ BYTECODE_JUMP */
        {
            int offset;
        } jump;
        struct             /* BYTECODE_GLOBAL_VEC */
        {
            unsigned int count;
        } global_vec;
        struct             /* BYTECODE_MARK */
        {
            unsigned int addr;
        } mark;
        struct             /* BYTECODE_RET */
        {
            unsigned int count;
        } ret;
        struct             /* BYTECODE_LINE */
        {
            unsigned int no;
        } line;
        struct
        {
            unsigned int id;
        } build_in;
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
void bytecode_print_float(bytecode * code);
void bytecode_print_id_local(bytecode * code);
void bytecode_print_id_global(bytecode * code);
void bytecode_print_id_func_func(bytecode * code);
void bytecode_print_id_func_addr(bytecode * code);
void bytecode_print_jumpz(bytecode * code);
void bytecode_print_jump(bytecode * code);
void bytecode_print_label(bytecode * code);
void bytecode_print_op_neg(bytecode * code);
void bytecode_print_op_add(bytecode * code);
void bytecode_print_op_sub(bytecode * code);
void bytecode_print_op_mul(bytecode * code);
void bytecode_print_op_div(bytecode * code);
void bytecode_print_op_lt(bytecode * code);
void bytecode_print_op_gt(bytecode * code);
void bytecode_print_op_lte(bytecode * code);
void bytecode_print_op_gte(bytecode * code);
void bytecode_print_op_eq(bytecode * code);
void bytecode_print_op_neq(bytecode * code);
void bytecode_print_func_def(bytecode * code);
void bytecode_print_global_vec(bytecode * code);
void bytecode_print_mark(bytecode * code);
void bytecode_print_call(bytecode * code);
void bytecode_print_ret(bytecode * code);
void bytecode_print_line(bytecode * code);
void bytecode_print_build_in(bytecode * code);
void bytecode_print_halt(bytecode * code);
 
bytecode_list_node * bytecode_list_node_new(bytecode * value);
void bytecode_list_node_delete(bytecode_list_node * node);

bytecode_list * bytecode_new();
void bytecode_delete(bytecode_list * code);

bytecode * bytecode_add(bytecode_list * code, bytecode * value);
void bytecode_func_addr(bytecode_list * code);
void bytecode_print(bytecode_list * code);

void bytecode_to_array(bytecode_list * code, bytecode ** code_arr, unsigned int * code_size);
void bytecode_array_delete(bytecode * code_arr);
void bytecode_array_print(bytecode * code_arr, unsigned int size);

#endif /* __BYTECODE_H__ */


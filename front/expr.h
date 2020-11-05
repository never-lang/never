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
#ifndef __EXPR_H__
#define __EXPR_H__

typedef float func_float_float(float x);

typedef enum expr_type
{
    EXPR_BOOL = 1,
    EXPR_INT = 2,
    EXPR_LONG = 3,
    EXPR_FLOAT = 4,
    EXPR_DOUBLE = 5,
    EXPR_CHAR = 6,
    EXPR_STRING = 7,
    EXPR_ENUMTYPE = 8,
    EXPR_ID = 9,
    EXPR_NEG = 10,
    EXPR_ADD = 11,
    EXPR_SUB = 12,
    EXPR_MUL = 13,
    EXPR_DIV = 14,
    EXPR_MOD = 15,
    EXPR_LT = 16,
    EXPR_GT = 17,
    EXPR_LTE = 18,
    EXPR_GTE = 19,
    EXPR_EQ = 20,
    EXPR_NEQ = 21,
    EXPR_AND = 22,
    EXPR_OR = 23,
    EXPR_BIN_NOT = 24,
    EXPR_BIN_AND = 25,
    EXPR_BIN_OR = 26,
    EXPR_BIN_XOR = 27,
    EXPR_BIN_SHL = 28,
    EXPR_BIN_SHR = 29,
    EXPR_PIPEL = 30,
    EXPR_NOT = 31,
    EXPR_SUP = 32,         /* ( expr ) */
    EXPR_COND = 33,        /* expr ? expr : expr */
    EXPR_ARRAY = 34,       /* [ expr_list ] */
    EXPR_ARRAY_DEREF = 35, /* expr [ expr_list ] */
    EXPR_CALL = 36,        /* ID ( expr_list) */
    EXPR_LAST_CALL = 37,   /* ID (expr_list */
    EXPR_FUNC = 38,        /* func ID ( ... ) */
    EXPR_RANGE_DIM = 39,
    EXPR_RANGE = 40,
    EXPR_SLICE = 41,
    EXPR_SEQ = 42,
    EXPR_ASS = 43,
    EXPR_WHILE = 44,
    EXPR_DO_WHILE = 45,
    EXPR_FOR = 46,
    EXPR_FORIN = 47,
    EXPR_IFLET = 48,
    EXPR_MATCH = 49,
    EXPR_BUILD_IN = 50,
    EXPR_CONV = 51,
    EXPR_LISTCOMP = 52,
    EXPR_ATTR = 53,
    EXPR_NIL = 54,
    EXPR_C_NULL = 55,
    EXPR_BIND = 56
} expr_type;

typedef enum conv_type
{
    CONV_UNKNOWN,
    CONV_INT_TO_LONG,
    CONV_INT_TO_FLOAT,
    CONV_INT_TO_DOUBLE,

    CONV_LONG_TO_INT,
    CONV_LONG_TO_FLOAT,
    CONV_LONG_TO_DOUBLE,

    CONV_FLOAT_TO_INT,
    CONV_FLOAT_TO_LONG,
    CONV_FLOAT_TO_DOUBLE,
    
    CONV_DOUBLE_TO_INT,
    CONV_DOUBLE_TO_LONG,
    CONV_DOUBLE_TO_FLOAT,

    CONV_ENUMTYPE_RECORD_TO_INT
} conv_type;

typedef enum comb_type
{
    COMB_TYPE_UNKNOWN = 0,
    COMB_TYPE_ERR = 1,
    COMB_TYPE_NIL = 2,
    COMB_TYPE_BOOL = 3,
    COMB_TYPE_INT = 4,
    COMB_TYPE_LONG = 5,
    COMB_TYPE_FLOAT = 6,
    COMB_TYPE_DOUBLE = 7,
    COMB_TYPE_CHAR = 8,
    COMB_TYPE_STRING = 9,
    COMB_TYPE_VOID = 10,
    COMB_TYPE_C_PTR = 11,
    COMB_TYPE_ARRAY = 12,
    COMB_TYPE_RANGE = 13,
    COMB_TYPE_SLICE = 14,
    COMB_TYPE_FUNC = 15,
    COMB_TYPE_ENUMTYPE = 16,
    COMB_TYPE_ENUMTYPE_ID = 17,
    COMB_TYPE_RECORD = 18,
    COMB_TYPE_RECORD_ID = 19,
    COMB_TYPE_MODULE = 20
} comb_type;

typedef enum id_type
{
    ID_TYPE_UNKNOWN = 0,
    ID_TYPE_LOCAL = 1,
    ID_TYPE_GLOBAL = 2,
    ID_TYPE_BIND_TOP = 3,
    ID_TYPE_BIND = 4,
    ID_TYPE_MATCHBIND = 5,
    ID_TYPE_QUALIFIER = 6,
    ID_TYPE_FORIN = 7,
    ID_TYPE_FUNC_TOP = 8,
    ID_TYPE_FUNC = 9,
    ID_TYPE_FUNC_NEST = 10,
    ID_TYPE_ENUMTYPE = 11,
    ID_TYPE_RECORD = 12,
    ID_TYPE_MODULE = 13
} id_type;

typedef struct array array;
typedef struct bind bind;
typedef struct func func;
typedef struct qualifier qualifier;
typedef struct expr_list expr_list;
typedef struct listcomp listcomp;
typedef struct record record;
typedef struct enumtype enumtype;
typedef struct enumerator enumerator;
typedef struct iflet iflet;
typedef struct forin forin;
typedef struct matchbind matchbind;
typedef struct match_guard_list match_guard_list;
typedef struct freevar freevar;
typedef struct param param;
typedef struct module_decl module_decl;

typedef struct expr_comb
{
    comb_type comb;
    struct param_list * comb_params; /* function arguments */
    struct param * comb_ret;       /* function ret */
    unsigned int comb_dims;               /* array dimensions */
    struct record * comb_record;   /* record */ 
    struct enumtype * comb_enumtype; /* enum */
    struct matchbind * comb_matchbind; /* match bind (id in record match guard) */
    struct module_decl * comb_module_decl; /* used module */
} expr_comb;

typedef struct expr
{
    expr_type type;
    expr_comb comb;
    unsigned int line_no;
    union {
        int int_value;       /* EXPR_INT */
        long long long_value; /* EXPR_LONG */
        float float_value;   /* EXPR_FLOAT */
        double double_value; /* EXPR_DOUBLE */
        char char_value;     /* EXPR_CHAR */
        char * string_value; /* EXPR_STRING */
        struct
        {
            conv_type type; /* EXPR_CONV */
            struct expr * expr_value;
        } conv;
        struct
        {
            char called;
            struct expr * enum_id;
            char * item_id;
            enumerator * id_enumerator_value;
            enumtype * id_enumtype_value;
        } enumtype;
        struct               /* EXPR_ID */
        {
            char * id;
            id_type id_type_value;
            union {
                freevar * id_freevar_value;
                param * id_param_value;
                bind * id_bind_value;
                matchbind * id_matchbind_value;
                qualifier * id_qualifier_value;
                forin * id_forin_value;
                func * id_func_value;
                record * id_record_value;
                enumtype * id_enumtype_value;
                module_decl * id_module_decl_value;
            };
        } id;
        struct func * func_value; /* EXPR_FUNC */
        struct
        {
            struct expr * left;
            struct expr * right;
            struct expr * middle; /* in ternary left_expr ? middle_expr :
                                     right_expr */
        };
        struct                        /* EXPR_SEQ */
        {
            struct expr_list * list;
        } seq;
        struct
        {
            struct expr * cond;
            struct expr * do_value;
        } whileloop;
        struct
        {
            struct expr * init;
            struct expr * cond;
            struct expr * incr;
            struct expr * do_value;
        } forloop;
        struct forin * forin_value;
        struct iflet * iflet_value;
        struct
        {
            struct expr * expr_value;
            struct match_guard_list * match_guards;
        } match;
        struct
        {
            struct expr * func_expr; /* EXPR_CALL, EXPR_LAST_CALL */
            struct expr_list * params;
        } call;
        struct
        {
            unsigned int id; /* EXPR_BUILD_IN */
            struct expr_list * param;
            struct param * ret;
        } func_build_in;
        struct
        {
            struct array * array_value; /* EXPR_ARRAY */
        } array;
        struct
        {
            struct expr * array_expr; /* EXPR_ARRAY_DEREF */
            struct expr_list * ref;
        } array_deref;
        struct
        {
            struct expr * from;
            struct expr * to;
        } range_dim;
        struct
        {
            struct param * ret;
            struct expr_list * range_dims;
        } range;
        struct
        {
            struct expr * array_expr;
            struct expr_list * range_dims;
        } slice;
        listcomp * listcomp_value; /* EXPR_LISTCOMP */
        struct
        {
            struct expr * record_value; /* record_value . id */
            struct expr * id;
        } attr;
        struct
        {
            struct bind * bind_value; /* EXPR_BIND */
        } bind;        
    };
} expr;

typedef struct expr_list_node
{
    expr * value;
    struct expr_list_node * prev;
    struct expr_list_node * next;
} expr_list_node;

typedef struct expr_list
{
    unsigned int count;
    expr_list_node * head;
    expr_list_node * tail;
} expr_list;

expr * expr_new_bool(int int_value);
expr * expr_new_int(int int_value);
expr * expr_new_long(long long long_value);
expr * expr_new_float(float float_value);
expr * expr_new_double(double double_value);
expr * expr_new_char(char char_value);
expr * expr_new_string(char * string_value);
expr * expr_new_enumtype(expr * enum_id, char * item_id);
expr * expr_new_id(char * id);
expr * expr_new_nil();
expr * expr_new_c_null();
expr * expr_new_one(int type, expr * expr_left);
expr * expr_new_two(int type, expr * expr_left, expr * expr_right);
expr * expr_new_three(int type, expr * expr_left, expr * expr_middle,
                      expr * expr_right);
expr * expr_new_array(array * value);
expr * expr_new_array_deref(expr * array_expr, expr_list * ref);
expr * expr_new_range(expr_list * range_dims);
expr * expr_new_range_dim(expr * from, expr * to);
expr * expr_new_slice(expr * array_expr, expr_list * range_dims);
expr * expr_new_seq(expr_list * list);
expr * expr_new_func(func * value);
expr * expr_new_call(expr * func_expr, expr_list * params);
expr * expr_new_ass(expr * left, expr * right);
expr * expr_new_while(expr * cond, expr * do_value);
expr * expr_new_do_while(expr * cond, expr * do_value);
expr * expr_new_for(expr * init, expr * cond, expr * incr, expr * do_value);
expr * expr_new_for_in(char * id, expr * in_value, expr * do_value);
expr * expr_new_iflet(iflet * iflet_value);
expr * expr_new_match(expr * expr_value, match_guard_list * match_guards);
expr * expr_new_build_in(unsigned int id, expr_list * params, param * param_ret);
expr * expr_new_listcomp(listcomp * listcomp_value);
expr * expr_new_attr(expr * record_value, expr * id);
expr * expr_new_bind(bind * bind_value);

comb_type conv_to_comb_type(conv_type conv);
expr * expr_conv(expr * expr_value, conv_type conv);

void expr_delete(expr * value);

expr_list_node * expr_list_node_new(expr * value);
void expr_list_node_delete(expr_list_node * node);

expr_list * expr_list_new();
void expr_list_delete(expr_list * list);

void expr_list_add_beg(expr_list * list, expr * value);
void expr_list_add_end(expr_list * list, expr * value);

const char * expr_type_str(expr_type type);
const char * comb_type_str(comb_type type);
const char * conv_type_str(conv_type conv);

#endif /* __EXPR_H__ */

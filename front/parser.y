/**
 * Copyright 2018-2020 Slawomir Maludzinski
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

%{

#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "utils.h"
#include "types.h"
#include "scanner.h"
#include "never.h"

int parse_result;

int yylex(token * tokp)
{
    return lex_scan(tokp);
}

int yyerror(never ** nev, char * str)
{
    parse_result = 1;
    print_error_msg(line_no, "%s", str);
    
    return 1;
}
%}

%token <val.str_value> TOK_ID
%token <val.float_value> TOK_NUM_FLOAT
%token <val.int_value> TOK_NUM_INT
%token <val.str_value> TOK_NUM_STRING
%token <val.char_value> TOK_NUM_CHAR
%token <val.str_value> TOK_BOOL
%token <val.str_value> TOK_TRUE
%token <val.str_value> TOK_FALSE
%token <val.str_value> TOK_CHAR
%token <val.str_value> TOK_STRING
%token <val.str_value> TOK_VOID
%token <val.str_value> TOK_C_PTR
%token <val.str_value> TOK_C_NULL
%token <val.str_value> TOK_FLOAT
%token <val.str_value> TOK_INT
%token <val.str_value> TOK_LET
%token <val.str_value> TOK_VAR
%token <val.str_value> TOK_FUNC
%token <val.str_value> TOK_RET /* -> */
%token <val.str_value> TOK_WHILE
%token <val.str_value> TOK_DO
%token <val.str_value> TOK_IF
%token <val.str_value> TOK_THEN
%token <val.str_value> TOK_ELSE
%token <val.str_value> TOK_FOR
%token <val.str_value> TOK_CATCH
%token <val.str_value> TOK_THROW
%token <val.str_value> TOK_IN
%token <val.str_value> TOK_RECORD
%token <val.str_value> TOK_NIL
%token <val.str_value> TOK_ENUM
%token <val.str_value> TOK_EXTERN
%token <val.str_value> TOK_MATCH
%token <val.str_value> TOK_TODOTS
%token <val.str_value> TOK_RANGE
%token <val.str_value> TOK_MODULE
%token <val.str_value> TOK_USE
%token <val.int_value> TOK_MODULE_REF

%type <val.expr_value> expr
%type <val.expr_list_value> expr_list
%type <val.expr_seq_value> expr_seq
%type <val.param_value> dim
%type <val.param_list_value> dim_list
%type <val.range_value> range_dim
%type <val.range_list_value> range_dim_list
%type <val.expr_value> expr_range_dim
%type <val.expr_list_value> expr_range_dim_list
%type <val.param_value> param
%type <val.param_list_value> param_list
%type <val.param_list_value> param_seq
%type <val.array_value> array;
%type <val.array_value> array_sub;
%type <val.listcomp_value> listcomp
%type <val.expr_list_value> array_sub_list
%type <val.qualifier_value> generator
%type <val.qualifier_value> qualifier
%type <val.qualifier_list_value> qualifier_list
%type <val.iflet_value> iflet
%type <val.matchbind_value> matchbind
%type <val.matchbind_list_value> matchbind_list
%type <val.match_guard_item_value> match_guard_item
%type <val.match_guard_record_value> match_guard_record
%type <val.match_guard_value> match_guard
%type <val.match_guard_list_value> match_guard_list
%type <val.let_value> let
%type <val.var_value> var
%type <val.bind_value> bind
%type <val.bind_list_value> bind_list
%type <val.func_decl_value> func_decl
%type <val.func_value> func
%type <val.func_list_value> func_list
%type <val.func_body_value> func_body
%type <val.func_except_value> func_except
%type <val.except_value> except_all
%type <val.except_value> except
%type <val.except_list_value> except_list
%type <val.enumerator_value> enum_item
%type <val.enumerator_list_value> enum_list
%type <val.enumtype_value> enumtype
%type <val.record_value> record
%type <val.decl_value> decl
%type <val.decl_list_value> decl_list
%type <val.use_value> use
%type <val.use_list_value> use_list
%type <val.module_decl_value> module_decl
%type <val.never_value> never
%type <val.never_value> start

%right TOK_IF TOK_ELSE TOK_FOR
%right TOK_RET

%right '='
%right <val.str_value> '?' ':'
%left TOK_OR
%left TOK_AND
%left TOK_EQ TOK_NEQ
%left <val.str_value> '<' '>' TOK_LTE TOK_GTE
%left <val.str_value> '+' '-'
%left <val.str_value> '*' '/' '%'
%right TOK_NOT /* %precedence NEG */
%left <val.str_value> '(' ')' '[' ']' ARR_DIM_BEG ARR_DIM_END TOK_DOT TOK_DDOT

%start start

%destructor { if ($$) free($$); } TOK_ID
%destructor { if ($$) free($$); } TOK_NUM_STRING
%destructor { if ($$) param_delete($$); } dim
%destructor { if ($$) param_list_delete($$); } dim_list
%destructor { if ($$) range_delete($$); } range_dim
%destructor { if ($$) range_list_delete($$); } range_dim_list
%destructor { if ($$) param_delete($$); } param
%destructor { if ($$) param_list_delete($$); } param_list
%destructor { if ($$) expr_delete($$); } expr
%destructor { if ($$) expr_list_delete($$); } expr_list
%destructor { if ($$) expr_list_delete($$); } expr_seq
%destructor { if ($$) expr_delete($$); } expr_range_dim
%destructor { if ($$) expr_list_delete($$); } expr_range_dim_list
%destructor { if ($$) array_delete($$); } array
%destructor { if ($$) array_delete($$); } array_sub
%destructor { if ($$) listcomp_delete($$); } listcomp
%destructor { if ($$) qualifier_delete($$); } generator
%destructor { if ($$) qualifier_delete($$); } qualifier
%destructor { if ($$) qualifier_list_delete($$); } qualifier_list
%destructor { if ($$) expr_list_delete($$); } array_sub_list
%destructor { if ($$) bind_delete($$); } let
%destructor { if ($$) bind_delete($$); } var
%destructor { if ($$) bind_delete($$); } bind
%destructor { if ($$) bind_list_delete($$); } bind_list
%destructor { if ($$) func_delete($$); } func
%destructor { if ($$) func_decl_delete($$); } func_decl
%destructor { if ($$) func_list_delete($$); } func_list
%destructor { if ($$) func_body_delete($$); } func_body
%destructor { if ($$) func_except_delete($$); } func_except
%destructor { if ($$) except_delete($$); } except_all
%destructor { if ($$) except_list_delete($$); } except_list
%destructor { if ($$) enumerator_delete($$); } enum_item
%destructor { if ($$) enumerator_list_delete($$); } enum_list
%destructor { if ($$) enumtype_delete($$); } enumtype
%destructor { if ($$) decl_delete($$); } decl
%destructor { if ($$) decl_list_delete($$); } decl_list
%destructor { if ($$) record_delete($$); } record
%destructor { if ($$) iflet_delete($$); } iflet
%destructor { if ($$) matchbind_delete($$); } matchbind
%destructor { if ($$) matchbind_list_delete($$); } matchbind_list
%destructor { if ($$) match_guard_item_delete($$); } match_guard_item
%destructor { if ($$) match_guard_record_delete($$); } match_guard_record
%destructor { if ($$) match_guard_delete($$); } match_guard
%destructor { if ($$) match_guard_list_delete($$); } match_guard_list
%destructor { if ($$) use_delete($$); } use
%destructor { if ($$) use_list_delete($$); } use_list
%destructor { if ($$) module_decl_delete($$); } module_decl
%destructor { if ($$) never_delete($$); } never
%destructor { } start

%pure-parser
%parse-param { never ** nev }

%%

expr: TOK_TRUE
{
    $$ = expr_new_bool(1);
    $$->line_no = $<line_no>1;
};

expr: TOK_FALSE
{
    $$ = expr_new_bool(0);
    $$->line_no = $<line_no>1;
};

expr: TOK_ID
{
    $$ = expr_new_id($1);
    $$->line_no = $<line_no>1;
};

expr: TOK_NUM_INT
{
    $$ = expr_new_int($1);
    $$->line_no = $<line_no>1;
};

expr: TOK_NUM_FLOAT
{
    $$ = expr_new_float($1);
    $$->line_no = $<line_no>1;
};

expr: TOK_NUM_CHAR
{
    $$ = expr_new_char($1);
    $$->line_no = $<line_no>1;
};

expr: TOK_NUM_STRING
{
    $$ = expr_new_string($1);
    $$->line_no = $<line_no>1;
};

expr: expr TOK_DDOT TOK_ID
{
    $$ = expr_new_enumtype($1, $3);
    $$->line_no = $<line_no>1;
};

expr: TOK_NIL
{
    $$ = expr_new_nil();
    $$->line_no = $<line_no>1;
};

expr: TOK_C_NULL
{
    $$ = expr_new_c_null();
    $$->line_no = $<line_no>1;  
};

expr: '-' expr %prec TOK_NOT
{
    $$ = expr_new_one(EXPR_NEG, $2);
    $$->line_no = $<line_no>1;
};

expr: expr '+' expr
{
    $$ = expr_new_two(EXPR_ADD, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr '-' expr
{
    $$ = expr_new_two(EXPR_SUB, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr '*' expr
{
    $$ = expr_new_two(EXPR_MUL, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr '/' expr
{
    $$ = expr_new_two(EXPR_DIV, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr '%' expr
{
    $$ = expr_new_two(EXPR_MOD, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr '<' expr
{
    $$ = expr_new_two(EXPR_LT, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr TOK_LTE expr
{
    $$ = expr_new_two(EXPR_LTE, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr '>' expr
{
    $$ = expr_new_two(EXPR_GT, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr TOK_GTE expr
{
    $$ = expr_new_two(EXPR_GTE, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr TOK_EQ expr
{
    $$ = expr_new_two(EXPR_EQ, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr TOK_NEQ expr
{
    $$ = expr_new_two(EXPR_NEQ, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr TOK_AND expr
{
    $$ = expr_new_two(EXPR_AND, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: expr TOK_OR expr
{
    $$ = expr_new_two(EXPR_OR, $1, $3);
    $$->line_no = $<line_no>2;
};

expr: TOK_NOT expr
{
    $$ = expr_new_one(EXPR_NOT, $2);
    $$->line_no = $<line_no>1;
};

expr: '(' expr ')'
{
    $$ = expr_new_one(EXPR_SUP, $2);
    $$->line_no = $<line_no>1;
};

expr: expr '?' expr ':' expr
{
    $$ = expr_new_three(EXPR_COND, $1, $3, $5);
    $$->line_no = $<line_no>2;
};

expr: TOK_IF '(' expr ')' expr %prec TOK_IF
{
    $$ = expr_new_three(EXPR_COND, $3, $5, expr_new_int(0));
    $$->line_no = $<line_no>1;
};

expr: TOK_IF '(' expr ')' expr TOK_ELSE expr %prec TOK_ELSE
{
    $$ = expr_new_three(EXPR_COND, $3, $5, $7);
    $$->line_no = $<line_no>1;
};

expr: array
{
    $$ = expr_new_array($1);
    $$->line_no = $1->line_no;
};

/* array dereference */
expr: expr '[' expr_list ']'
{
    $$ = expr_new_array_deref($1, $3);
    $$->line_no = $<line_no>2;
};

/* array or slice or range slice */
expr: expr '[' expr_range_dim_list ']'
{
    $$ = expr_new_slice($1, $3);
    $$->line_no = $<line_no>2;
};

array: ARR_DIM_BEG expr_list ARR_DIM_END ':' param
{
    $$ = array_new_dims($2, $5);
    $$->line_no = $<line_no>1;
};

array: '[' expr_list ']' ':' param
{
    $$ = array_new($2, $5);
    $$->line_no = $<line_no>1;
};

array: '[' array_sub_list ']' ':' param
{
    $$ = array_new($2, $5);
    $$->line_no = $<line_no>1;
};

array_sub: '[' expr_list ']'
{
    $$ = array_new_sub($2);
    $$->line_no = $<line_no>1;
};

array_sub: '[' array_sub_list ']'
{
    $$ = array_new_sub($2);
    $$->line_no = $<line_no>1;
};

array_sub_list: array_sub
{
    $$ = expr_list_new();
    expr_list_add_end($$, expr_new_array($1));
};

array_sub_list: array_sub_list ',' array_sub
{
    expr_list_add_end($1, expr_new_array($3));
    $$ = $1;
};

expr: listcomp
{
    $$ = expr_new_listcomp($1);
    $$->line_no = $<line_no>1;
};

generator: TOK_ID TOK_IN expr
{
    $$ = qualifier_new_generator($1, $3);
    $$->line_no = $<line_no>1;
};

generator: expr /* filter */
{
    $$ = qualifier_new_filter($1);
    $$->line_no = $1->line_no;
};

qualifier: generator
{
    $$ = $1;
};

qualifier_list: qualifier
{
    $$ = qualifier_list_new();
    qualifier_list_add_end($$, $1);
};

qualifier_list: qualifier_list ';' qualifier
{
    qualifier_list_add_end($1, $3);
    $$ = $1;
};

listcomp: '[' expr '|' qualifier_list ']' ':' param
{
    $$ = listcomp_new($2, $4, $7);
    $$->line_no = $<line_no>1;
};

expr: TOK_LET func
{
    $$ = expr_new_func($2);
};

expr: expr '(' ')'
{
    $$ = expr_new_call($1, NULL);
    $$->line_no = $1->line_no;
};

expr: expr '(' expr_list ')'
{
    $$ = expr_new_call($1, $3);
    $$->line_no = $1->line_no;
};

expr: expr '=' expr
{
    $$ = expr_new_ass($1, $3);
    $$->line_no = $<line_no>2;
};

expr: '{' expr_seq '}'
{
    $$ = expr_new_seq($2);
    $$->line_no = $<line_no>1;
};

expr: TOK_WHILE '(' expr ')' expr
{
    $$ = expr_new_while($3, $5);
    $$->line_no = $<line_no>1;
};

expr: TOK_DO expr TOK_WHILE '(' expr ')'
{
    $$ = expr_new_do_while($5, $2);
    $$->line_no = $<line_no>1;
};

expr: TOK_FOR '(' expr ';' expr ';' expr ')' expr %prec TOK_FOR
{
    $$ = expr_new_for($3, $5, $7, $9);
    $$->line_no = $<line_no>1;
};

expr: TOK_FOR '(' TOK_ID TOK_IN expr ')' expr %prec TOK_FOR
{
    $$ = expr_new_for_in($3, $5, $7);
    $$->line_no = $<line_no>1;
};

iflet: TOK_IF TOK_LET '(' match_guard_item '=' expr ')' expr %prec TOK_IF
{
    $$ = iflet_item_new($4, $6, $8, expr_new_int(0));
    $$->line_no = $<line_no>1;
};

iflet: TOK_IF TOK_LET '(' match_guard_record '=' expr ')' expr %prec TOK_IF
{
    $$ = iflet_record_new($4, $6, $8, expr_new_int(0));
    $$->line_no = $<line_no>1;
};

iflet: TOK_IF TOK_LET '(' match_guard_item '=' expr ')' expr TOK_ELSE expr %prec TOK_ELSE
{
    $$ = iflet_item_new($4, $6, $8, $10);
    $$->line_no = $<line_no>1;
};

iflet: TOK_IF TOK_LET '(' match_guard_record '=' expr ')' expr TOK_ELSE expr %prec TOK_ELSE
{
    $$ = iflet_record_new($4, $6, $8, $10);
    $$->line_no = $<line_no>1;
};

expr: iflet
{
    $$ = expr_new_iflet($1);
    $$->line_no = $<line_no>1;
};

matchbind: TOK_ID
{
    $$ = matchbind_new($1);
    $$->line_no = $<line_no>1;
};

matchbind_list: matchbind
{
    $$ = matchbind_list_new();
    matchbind_list_add_end($$, $1);
};

matchbind_list: matchbind_list ',' matchbind
{
    matchbind_list_add_end($1, $3);
    $$ = $1;
};

match_guard_item: TOK_ID TOK_DDOT TOK_ID
{
    $$ = match_guard_item_new($1, $3);
    $$->line_no = $<line_no>1;
};

match_guard_item: TOK_ID TOK_DOT TOK_ID TOK_DDOT TOK_ID
{
    $$ = match_guard_item_new_module($3, $5, $1);
    $$->line_no = $<line_no>1;
};

match_guard_record: TOK_ID TOK_DDOT TOK_ID '(' matchbind_list ')'
{
    $$ = match_guard_record_new($1, $3, $5);
    $$->line_no = $<line_no>1;
};

match_guard_record: TOK_ID TOK_DOT TOK_ID TOK_DDOT TOK_ID '(' matchbind_list ')'
{
    $$ = match_guard_record_new_module($3, $5, $1, $7);
    $$->line_no = $<line_no>1;
};

match_guard: match_guard_item TOK_RET expr
{
    $$ = match_guard_new_item($1, $3);
    $$->line_no = $<line_no>1;
};

match_guard: match_guard_record TOK_RET expr
{
    $$ = match_guard_new_record($1, $3);
    $$->line_no = $<line_no>1;
};

match_guard: TOK_ELSE TOK_RET expr
{
    $$ = match_guard_new_else($3);
    $$->line_no = $<line_no>1;
};

match_guard_list: match_guard ';'
{
    $$ = match_guard_list_new();
    match_guard_list_add_end($$, $1);
};

match_guard_list: match_guard_list match_guard ';'
{
    match_guard_list_add_end($1, $2);
    $$ = $1;
};

expr: TOK_MATCH expr '{' '}'
{
    $$ = expr_new_match($2, NULL);
    $$->line_no = $<line_no>1;
};

expr: TOK_MATCH expr '{' match_guard_list '}'
{
    $$ = expr_new_match($2, $4);
    $$->line_no = $<line_no>1;
};

expr: expr TOK_DOT TOK_ID
{
    $$ = expr_new_attr($1, $3);
    $$->line_no = $<line_no>1;
};

expr_list: expr
{
    $$ = expr_list_new();
    expr_list_add_end($$, $1);
};

expr_list: expr_list ',' expr
{
    expr_list_add_end($1, $3);
    $$ = $1;
};

expr_seq: expr
{
    $$ = expr_list_new();
    expr_list_add_end($$, $1);
};

expr_seq: expr_seq ';' expr
{
    expr_list_add_end($1, $3);
    $$ = $1;
};

expr_range_dim: expr TOK_TODOTS expr
{
    $$ = expr_new_range_dim($1, $3);
    $$->line_no = $<line_no>2;
};

expr_range_dim_list: expr_range_dim
{
    $$ = expr_list_new();
    expr_list_add_end($$, $1);
};

expr_range_dim_list: expr_range_dim_list ',' expr_range_dim
{
    expr_list_add_end($1, $3);
    $$ = $1;
};

expr: '[' expr_range_dim_list ']'
{
    $$ = expr_new_range($2);
    $$->line_no = $<line_no>1;
};

dim: TOK_ID
{
    $$ = param_new_dim($1);
    $$->line_no = $<line_no>1;
};

dim_list: dim
{
    $$ = param_list_new();
    param_list_add_end($$, $1);
};

dim_list: dim_list ',' dim
{
    param_list_add_end($1, $3);
    $$ = $1;
};

range_dim: TOK_TODOTS
{
    param * from = param_new_range_dim(NULL);
    param * to = param_new_range_dim(NULL);

    $$ = range_new(from, to);
    $$->line_no = $<line_no>1;
};

range_dim: TOK_ID TOK_TODOTS TOK_ID
{
    param * from = param_new_range_dim($1);
    param * to = param_new_range_dim($3);

    from->line_no = $<line_no>1;
    to->line_no = $<line_no>3;

    $$ = range_new(from, to);
    $$->line_no = $<line_no>2;
};

range_dim_list: range_dim
{
    $$ = range_list_new();
    range_list_add_end($$, $1);
};

range_dim_list: range_dim_list ',' range_dim
{
    range_list_add_end($1, $3);
    $$ = $1;
};

param: TOK_BOOL
{
    $$ = param_new_bool(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_BOOL
{
    $$ = param_new_bool($1);
    $$->line_no = $<line_no>1;
};

param: TOK_INT
{
    $$ = param_new_int(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_INT
{
    $$ = param_new_int($1);
    $$->line_no = $<line_no>1;
};

param: TOK_FLOAT
{
    $$ = param_new_float(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_FLOAT 
{
    $$ = param_new_float($1);
    $$->line_no = $<line_no>1;
};

param: TOK_CHAR
{
    $$ = param_new_char(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_CHAR
{
    $$ = param_new_char($1);
    $$->line_no = $<line_no>1;
};

param: TOK_STRING
{
    $$ = param_new_string(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_STRING
{
    $$ = param_new_string($1);
    $$->line_no = $<line_no>1;
};

param: TOK_VOID
{
    $$ = param_new_void(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_C_PTR
{
    $$ = param_new_c_ptr(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_C_PTR
{
    $$ = param_new_c_ptr($1);
    $$->line_no = $<line_no>1;
};

param: TOK_ID %prec TOK_RET
{
    $$ = param_new_record(NULL, $1);
    $$->line_no = $<line_no>1;
};

param: TOK_ID TOK_DOT TOK_ID %prec TOK_RET
{
    $$ = param_new_record_module(NULL, $3, $1);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_ID %prec TOK_RET
{
    $$ = param_new_record($1, $3);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_ID TOK_DOT TOK_ID %prec TOK_RET
{
    $$ = param_new_record_module($1, $5, $3);
    $$->line_no = $<line_no>1;
};

param: '[' dim_list ']' ':' param
{
    $$ = param_new_array(NULL, $2, $5);
    $$->line_no = $<line_no>1;
};

param: TOK_ID '[' dim_list ']' ':' param
{
    $$ = param_new_array($1, $3, $6);
    $$->line_no = $<line_no>1;
};

param: '[' range_dim_list ']' ':' TOK_RANGE
{
    $$ = param_new_range(NULL, $2);
    $$->line_no = $<line_no>1;
};

param: TOK_ID '[' range_dim_list ']' ':' TOK_RANGE
{
    $$ = param_new_range($1, $3);
    $$->line_no = $<line_no>1;
};

param: '[' range_dim_list ']' ':' param
{
    $$ = param_new_slice(NULL, $2, $5);
    $$->line_no = $<line_no>1;
};

param: TOK_ID '[' range_dim_list ']' ':' param
{
    $$ = param_new_slice($1, $3, $6);
    $$->line_no = $<line_no>1;
};

param: '(' ')' TOK_RET param
{
    $$ = param_new_func(NULL, NULL, $4);
    $$->line_no = $<line_no>4;
};

param: '(' param_list ')' TOK_RET param
{
    $$ = param_new_func(NULL, $2, $5);
    $$->line_no = $<line_no>5;
};

param: TOK_ID '(' ')' TOK_RET param
{
    $$ = param_new_func($1, NULL, $5);
    $$->line_no = $<line_no>5;
};

param: TOK_ID '(' param_list ')' TOK_RET param
{
    $$ = param_new_func($1, $3, $6);
    $$->line_no = $<line_no>6;
};

param_list: param
{
    $$ = param_list_new();
    param_list_add_end($$, $1);
};

param_list: param_list ',' param
{
    param_list_add_end($1, $3);
    $$ = $1;
};

param_seq: param ';'
{
    $$ = param_list_new();
    param_list_add_end($$, $1);
};

param_seq: param_seq param ';'
{
    param_list_add_end($1, $2);
    $$ = $1;
};

let: TOK_LET TOK_ID '=' expr
{
    $$ = bind_new_let($2, $4);
    $$->line_no = $<line_no>1;
};

var: TOK_VAR TOK_ID '=' expr
{
    $$ = bind_new_var($2, $4);
    $$->line_no = $<line_no>1;
};

bind: let
{
    $$ = $1;
};

bind: var
{
    $$ = $1;
};

bind_list: bind ';'
{
    $$ = bind_list_new();
    bind_list_add_end($$, $1);
};

bind_list: bind_list bind ';'
{
    bind_list_add_end($1, $2);
    $$ = $1;
};

func_decl: '(' ')' TOK_RET param
{
    $$ = func_decl_new(NULL, NULL,$4);
};

func_decl: '(' param_list ')' TOK_RET param
{
    $$ = func_decl_new(NULL, $2, $5);
};

func_decl: TOK_ID '(' ')' TOK_RET param
{
    $$ = func_decl_new($1, NULL, $5);
};

func_decl: TOK_ID '(' param_list ')' TOK_RET param
{
    $$ = func_decl_new($1, $3, $6);
};

func_body: '{' bind_list expr_seq '}'
{
    $$ = func_body_new($2, NULL, $3);
};

func_body: '{' func_list expr_seq '}'
{
    $$ = func_body_new(NULL, $2, $3);
};

func_body: '{' bind_list func_list expr_seq '}'
{
    $$ = func_body_new($2, $3, $4);
};

func_body: '{' expr_seq '}'
{
    $$ = func_body_new(NULL, NULL, $2);
};

func_body: '{' '}'
{
    $$ = func_body_new(NULL, NULL, NULL);
};

except_all: TOK_CATCH '{' expr_seq '}'
{
    $$ = except_new_all($3);
    $$->line_no = $<line_no>1;
};

except: TOK_CATCH '(' TOK_ID ')' '{' expr_seq '}'
{
    $$ = except_new_id($3, $6);
    $$->line_no = $<line_no>1;
};

except_list: except
{
    $$ = except_list_new();
    except_list_add_end($$, $1);
};

except_list: except_list except
{
    except_list_add_end($1, $2);
    $$ = $1;
};

func_except: except_all
{
    $$ = func_except_new($1, NULL);
};

func_except: except_list
{
    $$ = func_except_new(NULL, $1);
};

func_except: except_list except_all
{
    $$ = func_except_new($2, $1);
};

func: TOK_FUNC func_decl func_body
{
    $$ = func_new($2, $3);
    $$->line_no = $<line_no>1;
};

func: TOK_FUNC func_decl func_body func_except
{
    $$ = func_new_except($2, $3, $4);
    $$->line_no = $<line_no>1;
};

func: TOK_EXTERN TOK_NUM_STRING TOK_FUNC func_decl
{
    $$ = func_new_ffi($2, $4);
    $$->line_no = $<line_no>1;   
};

func: TOK_FUNC TOK_ID error
{
    print_error_msg(line_no, "error in function %s defined at %d\n", $2, $<line_no>2);
    free($2);
    
    yyclearin;
    yyerrok;
    $$ = NULL;
};

func_list: func
{
    $$ = func_list_new();
    func_list_add_end($$, $1);
};

func_list: func_list func
{
    func_list_add_end($1, $2);
    $$ = $1;
};

enum_item: TOK_ID '{' param_seq '}'
{
    $$ = enumerator_new_record($1, record_new(NULL, $3));
    $$->line_no = $<line_no>1;
};

enum_item: TOK_ID
{
    $$ = enumerator_new($1);
    $$->line_no = $<line_no>1;
};

enum_list: enum_item
{
    $$ = enumerator_list_new();
    enumerator_list_add_end($$, $1);
};

enum_list: enum_list ',' enum_item
{
    enumerator_list_add_end($1, $3);
    $$ = $1;
};

enumtype: TOK_ENUM TOK_ID '{' enum_list '}'
{
    $$ = enumtype_new($2, $4);
    $$->line_no = $<line_no>1;
};

record: TOK_RECORD TOK_ID '{' param_seq '}'
{
    $$ = record_new($2, $4);
    $$->line_no = $<line_no>1;
};

decl: enumtype
{
    $$ = decl_new_enumtype($1);
};

decl: record
{
    $$ = decl_new_record($1);
};

decl_list: decl
{
    $$ = decl_list_new();
    decl_list_add_end($$, $1);
};

decl_list: decl_list decl
{
    decl_list_add_end($1, $2);
    $$ = $1;
};

use: TOK_USE TOK_NUM_STRING module_decl
{
    $$ = use_new($2, $3);
    $$->line_no = $<line_no>1;
};

use_list: use
{
    $$ = use_list_new();
    use_list_add_end($$, $1);
};

use_list: use_list use
{
    use_list_add_end($1, $2);
    $$ = $1;
};

never: func_list
{
    $$ = never_new(NULL, NULL, NULL, $1);
};

never: bind_list func_list
{
    $$ = never_new(NULL, NULL, $1, $2);
};

never: decl_list func_list
{
    $$ = never_new(NULL, $1, NULL, $2);
};

never: decl_list bind_list func_list
{
    $$ = never_new(NULL, $1, $2, $3);
};

never: use_list func_list
{
    $$ = never_new($1, NULL, NULL, $2);
};

never: use_list bind_list func_list
{
    $$ = never_new($1, NULL, $2, $3);
};

never: use_list decl_list func_list
{
    $$ = never_new($1, $2, NULL, $3);
};

never: use_list decl_list bind_list func_list
{
    $$ = never_new($1, $2, $3, $4);
};

module_decl: TOK_MODULE_REF
{
    $$ = module_decl_new_ref(NULL, NULL);
    $$->line_no = $<line_no>1;
};

module_decl: TOK_MODULE TOK_ID '{' never '}'
{
    $$ = module_decl_new($2, $4);
    $$->line_no = $<line_no>1;
};

start: never
{
    $$ = *nev = $1;
};

%%



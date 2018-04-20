
%{
#include <stdio.h>
#include "types.h"
#include "scanner.h"
#include "never.h"

int yylex(token * tokp)
{
    return lex_scan(tokp);
}

int yyerror(never ** nev, char * str)
{
    fprintf(stderr, "error: %s\n", str);
    return 0;
}
%}
%pure-parser

%token <val.str_value> TOK_ID
%token <val.int_value> TOK_NUM
%token <val.str_value> TOK_INT
%token <val.str_value> TOK_FUNC
%token <val.str_value> TOK_RET /* -> */
%token <val.str_value> TOK_RETURN

%type <val.expr_value> expr
%type <val.expr_list_value> expr_list
%type <val.arg_value> arg
%type <val.arg_list_value> arg_list
%type <val.func_value> func
%type <val.func_list_value> func_list
%type <val.func_body_value> func_body
%type <val.never_value> never

%right <val.str_value> '?' ':'
%left <val.str_value> '<' '>' TOK_LTE TOK_GTE TOK_EQ
%left <val.str_value> '+' '-'
%left <val.str_value> '*' '/'
%precedence NEG
%left <val.str_value> '(' ')'

%start never

%parse-param { never ** nev }

%%

expr: TOK_ID
{
    $$ = expr_new_id($1);
};

expr: TOK_NUM
{
    $$ = expr_new_int($1);
};

expr: '-' expr %prec NEG
{
    $$ = expr_new_one(EXPR_NEG, $2);
};

expr: expr '+' expr
{
    $$ = expr_new_two(EXPR_ADD, $1, $3);
};

expr: expr '-' expr
{
    $$ = expr_new_two(EXPR_SUB, $1, $3);
};

expr: expr '*' expr
{
    $$ = expr_new_two(EXPR_MUL, $1, $3);
};

expr: expr '/' expr
{
    $$ = expr_new_two(EXPR_DIV, $1, $3);
};

expr: expr '<' expr
{
    $$ = expr_new_two(EXPR_LT, $1, $3);
};

expr: expr TOK_LTE expr
{
    $$ = expr_new_two(EXPR_LTE, $1, $3);
};

expr: expr '>' expr
{
    $$ = expr_new_two(EXPR_GT, $1, $3);
};

expr: expr TOK_GTE expr
{
    $$ = expr_new_two(EXPR_GTE, $1, $3);
};

expr: expr TOK_EQ expr
{
    $$ = expr_new_two(EXPR_EQ, $1, $3);
};

expr: '(' expr ')'
{
    $$ = expr_new_one(EXPR_SUP, $2);
};

expr: expr '?' expr ':' expr
{
    $$ = expr_new_three(EXPR_COND, $1, $3, $5);
};

expr: func
{
    $$ = expr_new_func($1);
};

expr: expr '(' ')'
{
    $$ = expr_new_call($1, NULL);
};

expr: expr '(' expr_list ')'
{
    $$ = expr_new_call($1, $3);
};

expr_list: expr
{
    $$ = expr_list_new();
    expr_list_add_end($$, $1);
};

expr_list: expr_list ',' expr
{
    expr_list_add_end($$, $3);
};

arg: TOK_INT
{
    $$ = arg_new_int(NULL);
};

arg: TOK_INT TOK_ID
{
    $$ = arg_new_int($2);
};

arg: '(' ')' TOK_RET arg
{
    $$ = arg_new_func(NULL, NULL, $4);
};

arg: '(' arg_list ')' TOK_RET arg
{
    $$ = arg_new_func(NULL, $2, $5);
};

arg: TOK_ID '(' ')' TOK_RET arg
{
    $$ = arg_new_func($1, NULL, $5);
};

arg: TOK_ID '(' arg_list ')' TOK_RET arg
{
    $$ = arg_new_func($1, $3, $6);
};

arg_list: arg
{
    $$ = arg_list_new();
    arg_list_add_end($$, $1);
};

arg_list: arg_list ',' arg
{
    arg_list_add_end($$, $3);
};

func: TOK_FUNC TOK_ID '(' ')' TOK_RET arg func_body
{
    $$ = func_new($2, NULL, $6, $7);
};

func: TOK_FUNC TOK_ID '(' arg_list ')' TOK_RET arg func_body
{
    $$ = func_new($2, $4, $7, $8);
};

func_body: '{' func_list TOK_RETURN expr ';' '}'
{
    $$ = func_body_new($2, $4);
};

func_body: '{' TOK_RETURN expr ';' '}'
{
    $$ = func_body_new(NULL, $3);
};

func_body: '{' '}'
{
    $$ = func_body_new(NULL, NULL);
};

func_list: func
{
    $$ = func_list_new();
    func_list_add_end($$, $1);
};

func_list: func_list func
{
    func_list_add_end($$, $2);
};

never: func_list
{
    $$ = *nev = never_new($1);
};

%%



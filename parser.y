
%{
#include <stdio.h>
#include "types.h"
#include "scanner.h"

int yylex(token * tokp)
{
    return lex_scan(tokp);
}

int yyerror(char * str)
{
    fprintf(stderr, "error: %s\n", str);
    return 0;
}
%}
%pure-parser

%token <val.str_value> TOK_ID
%token <val.str_int> TOK_NUM
%token <val.str_value> TOK_INT
%token <val.str_value> TOK_FUNC
%token <val.str_value> TOK_RET /* -> */
%token <val.str_value> TOK_RETURN

%type <val.expr_value> expr
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

%start never

%%

expr: TOK_ID
{
};

expr: TOK_NUM
{
};

expr: '-' expr %prec NEG
{
};

expr: expr '+' expr
{
};

expr: expr '-' expr
{
};

expr: expr '*' expr
{
};

expr: expr '/' expr
{
};

expr: expr '<' expr
{
};

expr: expr TOK_LTE expr
{
};

expr: expr '>' expr
{
};

expr: expr TOK_GTE expr
{
};

expr: expr TOK_EQ expr
{
};

expr: '(' expr ')'
{
};

expr: expr '?' expr ':' expr
{
}
;

expr: func
{
};

expr: TOK_ID '(' expr_list ')'
{
};

expr_list: expr
{
};

expr_list: expr_list ',' expr
{
};

arg: TOK_INT
{
};

arg: TOK_INT TOK_ID
{
};

arg: '(' ')'
{
};

arg: '(' ')' TOK_RET arg
{
};

arg: '(' arg_list ')'
{
};

arg: '(' arg_list ')' TOK_RET arg
{
};

arg: TOK_ID '(' ')'
{
};

arg: TOK_ID '(' ')' TOK_RET arg
{
};

arg: TOK_ID '(' arg_list ')'
{
};

arg: TOK_ID '(' arg_list ')' TOK_RET arg
{
};

arg_list: arg
{
};

arg_list: arg_list ',' arg
{
};

func: TOK_FUNC TOK_ID '(' ')' func_body
{
};

func: TOK_FUNC TOK_ID '(' ')' TOK_RET arg func_body
{
};

func: TOK_FUNC TOK_ID '(' arg_list ')' func_body
{
};

func: TOK_FUNC TOK_ID '(' arg_list ')' TOK_RET arg func_body
{
};

func_body: '{' func_list TOK_RETURN expr ';' '}'
{
};

func_body: '{' TOK_RETURN expr ';' '}'
{
};

func_list: func
{
};

func_list: func_list func
{
};

never: func_list
{
};

%%


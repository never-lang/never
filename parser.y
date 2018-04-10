
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

%token TOK_LTE
%token TOK_GTE
%token TOK_ID
%token TOK_NUM
%token <val.int_value> TOK_INT
%token TOK_FUNC
%token TOK_RET /* -> */
%token TOK_RETURN

%right '?' ':'
%left '<' '>' TOK_LTE TOK_GTE TOK_EQ
%left '+' '-'
%left '*' '/'
%precedence NEG

%start never

%%

expr: TOK_ID;
expr: TOK_NUM;
expr: '-' expr %prec NEG;
expr: expr '+' expr;
expr: expr '-' expr;
expr: expr '*' expr;
expr: expr '/' expr;
expr: expr '<' expr;
expr: expr TOK_LTE expr;
expr: expr '>' expr;
expr: expr TOK_GTE expr;
expr: expr TOK_EQ expr;
expr: '(' expr ')';
expr: expr '?' expr ':' expr;
expr: func;

expr: TOK_ID '(' expr_list ')';

expr_list: expr;
expr_list: expr_list ',' expr;

arg: TOK_INT;
arg: TOK_INT TOK_ID;
arg: '(' ')';
arg: '(' ')' TOK_RET arg;
arg: '(' args_list ')';
arg: '(' args_list ')' TOK_RET arg;
arg: TOK_ID '(' ')';
arg: TOK_ID '(' ')' TOK_RET arg;
arg: TOK_ID '(' args_list ')';
arg: TOK_ID '(' args_list ')' TOK_RET arg;

args_list: arg;
args_list: args_list ',' arg;

func: TOK_FUNC TOK_ID '(' ')' func_body;
func: TOK_FUNC TOK_ID '(' ')' TOK_RET arg func_body;
func: TOK_FUNC TOK_ID '(' args_list ')' func_body;
func: TOK_FUNC TOK_ID '(' args_list ')' TOK_RET arg func_body;

func_body: '{' func_list TOK_RETURN expr ';' '}';
func_body: '{' TOK_RETURN expr ';' '}';

func_list: func;
func_list: func_list func;

never: func_list;

%%


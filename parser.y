
%{
#include <stdio.h>
#include "scanner.h"

int yyerror(char * str)
{
    fprintf(stderr, "error: %s\n", str);
    return 0;
}
%}

%token TOK_ID
%token TOK_NUM
%token TOK_INT
%token TOK_FUNC
%token TOK_RET /* -> */
%token TOK_RETURN

%left '+' '-'
%left '*' '/'
%precedence NEG
%right '?' ':'
%left '<' '>' '='

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
expr: expr '>' expr;
expr: expr '=' expr;
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


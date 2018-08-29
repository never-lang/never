
%{
#include <stdio.h>
#include "utils.h"
#include "types.h"
#include "scanner.h"
#include "never.h"

int parse_result;
extern int line_no;

int yylex(token * tokp)
{
    return lex_scan(tokp);
}

int yyerror(never ** nev, char * str)
{
    parse_result = 1;
    print_error_msg(line_no, "%s\n", str);
    
    return 1;
}
%}

%token <val.str_value> TOK_ID
%token <val.float_value> TOK_NUM_FLOAT
%token <val.int_value> TOK_NUM_INT
%token <val.str_value> TOK_FLOAT
%token <val.str_value> TOK_INT
%token <val.str_value> TOK_LET
%token <val.str_value> TOK_VAR
%token <val.str_value> TOK_FUNC
%token <val.str_value> TOK_RET /* -> */
%token <val.str_value> TOK_LAMBDA
%token <val.str_value> TOK_WHILE
%token <val.str_value> TOK_DO
%token <val.str_value> TOK_IF
%token <val.str_value> TOK_THEN
%token <val.str_value> TOK_ELSE

%type <val.expr_value> expr
%type <val.expr_list_value> expr_list
%type <val.expr_seq_value> expr_seq
%type <val.param_value> dim
%type <val.param_list_value> dim_list
%type <val.param_value> param
%type <val.param_list_value> param_list
%type <val.array_value> array;
%type <val.let_value> let
%type <val.var_value> var
%type <val.bind_value> bind
%type <val.bind_list_value> bind_list
%type <val.func_value> func
%type <val.func_list_value> func_list
%type <val.func_body_value> func_body
%type <val.never_value> never

%right TOK_IF TOK_ELSE

%right '='
%right <val.str_value> '?' ':'
%left TOK_OR
%left TOK_AND
%left TOK_EQ TOK_NEQ
%left <val.str_value> '<' '>' TOK_LTE TOK_GTE
%left <val.str_value> '+' '-'
%left <val.str_value> '*' '/' '%'
%right TOK_NOT /* %precedence NEG */
%left <val.str_value> '(' ')' '[' ']' ARR_DIM_BEG ARR_DIM_END

%start never

%destructor { if ($$) free($$); } TOK_ID
%destructor { if ($$) param_delete($$); } dim
%destructor { if ($$) param_list_delete($$); } dim_list
%destructor { if ($$) param_delete($$); } param
%destructor { if ($$) param_list_delete($$); } param_list
%destructor { if ($$) expr_delete($$); } expr
%destructor { if ($$) expr_list_delete($$); } expr_list
%destructor { if ($$) expr_list_delete($$); } expr_seq
%destructor { if ($$) array_delete($$); } array
%destructor { if ($$) bind_delete($$); } let
%destructor { if ($$) bind_delete($$); } var
%destructor { if ($$) bind_delete($$); } bind
%destructor { if ($$) bind_list_delete($$); } bind_list
%destructor { if ($$) func_delete($$); } func
%destructor { if ($$) func_list_delete($$); } func_list
%destructor { if ($$) func_body_delete($$); } func_body
%destructor {  } never

%pure-parser
%parse-param { never ** nev }

%%

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

expr: expr '[' expr_list ']' /* array dereference */
{
    $$ = expr_new_array_deref($1, $3);
    $$->line_no = $<line_no>2;
};

array: ARR_DIM_BEG expr_list ARR_DIM_END TOK_RET param
{
    $$ = array_new_dims($2, $5);
    $$->line_no = $<line_no>1;
};

array: '[' expr_list ']' TOK_RET param
{
    $$ = array_new($2, $5);
    $$->line_no = $<line_no>1;
};

array: '[' expr_list ']'
{
    $$ = array_new_sub($2);
    $$->line_no = $<line_no>1;
};

expr: TOK_LAMBDA func
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
    $$ = expr_new_while($2, $5);
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

param: TOK_INT
{
    $$ = param_new_int(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID TOK_RET TOK_INT
{
    $$ = param_new_int($1);
    $$->line_no = $<line_no>2;
};

param: TOK_FLOAT
{
    $$ = param_new_float(NULL);
    $$->line_no = $<line_no>1;
};

param: TOK_ID TOK_RET TOK_FLOAT 
{
    $$ = param_new_float($1);
    $$->line_no = $<line_no>2;
};

param: '[' dim_list ']' TOK_RET param
{
    $$ = param_new_array(NULL, $2, $5);
    $$->line_no = $<line_no>1;
};

param: TOK_ID '[' dim_list ']' TOK_RET param
{
    $$ = param_new_array($1, $3, $6);
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

let: TOK_LET TOK_ID '=' expr
{
    $$ = bind_new_let($2, $4);
    $$->line_no = $<line_no>2;
};

var: TOK_VAR TOK_ID '=' expr
{
    $$ = bind_new_var($2, $4);
    $$->line_no = $<line_no>2;
};

bind: let
{
    $$ = $1;
};

bind: var
{
    $$ = $1;
};

bind_list: bind
{
    $$ = bind_list_new();
    bind_list_add_end($$, $1);
};

bind_list: bind_list bind
{
    bind_list_add_end($1, $2);
    $$ = $1;
};

func: TOK_FUNC '(' ')' TOK_RET param func_body
{
    $$ = func_new(NULL, NULL, $5, $6);
    $$->line_no = $<line_no>2;
};

func: TOK_FUNC '(' param_list ')' TOK_RET param func_body
{
    $$ = func_new(NULL, $3, $6, $7);
};

func: TOK_FUNC TOK_ID '(' ')' TOK_RET param func_body
{
    $$ = func_new($2, NULL, $6, $7);
    $$->line_no = $<line_no>2;
};

func: TOK_FUNC TOK_ID '(' param_list ')' TOK_RET param func_body
{
    $$ = func_new($2, $4, $7, $8);
    $$->line_no = $<line_no>2;
};

func: TOK_FUNC TOK_ID error
{
    print_error_msg(line_no, "error in function %s defined at %d\n", $2, $<line_no>2);
    free($2);
    
    yyclearin;
    yyerrok;
    $$ = NULL;
};

func_body: '{' bind_list ';' expr_seq '}'
{
    $$ = func_body_new($2, NULL, $4);
};

func_body: '{' func_list ';' expr_seq '}'
{
    $$ = func_body_new(NULL, $2, $4);
};

func_body: '{' bind_list func_list ';' expr_seq '}'
{
    $$ = func_body_new($2, $3, $5);
};

func_body: '{' expr_seq '}'
{
    $$ = func_body_new(NULL, NULL, $2);
};

func_body: '{' '}'
{
    $$ = func_body_new(NULL, NULL, NULL);
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

never: func_list
{
    $$ = *nev = never_new($1);
};

%%




%{
#include <unistd.h>
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
%token <val.str_value> TOK_NUM_STRING
%token <val.char_value> TOK_NUM_CHAR
%token <val.str_value> TOK_CHAR
%token <val.str_value> TOK_STRING
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

%type <val.expr_value> expr
%type <val.expr_list_value> expr_list
%type <val.expr_seq_value> expr_seq
%type <val.param_value> dim
%type <val.param_list_value> dim_list
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
%type <val.enumtype_list_value> enumtype_list
%type <val.record_value> record
%type <val.record_list_value> record_list
%type <val.never_value> never

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
%left <val.str_value> '(' ')' '[' ']' ARR_DIM_BEG ARR_DIM_END TOK_DOT

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
%destructor { if ($$) enumtype_list_delete($$); } enumtype_list
%destructor { if ($$) record_delete($$); } record
%destructor { if ($$) record_list_delete($$); } record_list
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

expr: TOK_NIL
{
    $$ = expr_new_nil();
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
    $$ = expr_new_while($2, $5);
    $$->line_no = $<line_no>1;
};

expr: TOK_FOR '(' expr ';' expr ';' expr ')' expr %prec TOK_FOR
{
    $$ = expr_new_for($3, $5, $7, $9);
    $$->line_no = $<line_no>1;
};

match_guard: TOK_ID TOK_DOT TOK_ID TOK_RET expr
{
    $$ = match_guard_new_item($1, $3, $5);
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
    $$->line_no = $<line_no>2;
};

param: TOK_ID %prec TOK_RET
{
    $$ = param_new_record(NULL, $1);
    $$->line_no = $<line_no>1;
};

param: TOK_ID ':' TOK_ID
{
    $$ = param_new_record($1, $3);
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
};

enum_item: TOK_ID
{
    $$ = enumerator_new($1);
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

enumtype_list: enumtype
{
    $$ = enumtype_list_new();
    enumtype_list_add_end($$, $1);
};

enumtype_list: enumtype_list enumtype
{
    enumtype_list_add_end($1, $2);
    $$ = $1;
};

record: TOK_RECORD TOK_ID '{' param_seq '}'
{
    $$ = record_new($2, $4);
    $$->line_no = $<line_no>1;
};

record_list: record
{
    $$ = record_list_new();
    record_list_add_end($$, $1);
};

record_list: record_list record
{
    record_list_add_end($1, $2);
    $$ = $1;
};

never: func_list
{
    $$ = *nev = never_new(NULL, NULL, $1);
};

never: enumtype_list func_list
{
    $$ = *nev = never_new($1, NULL, $2);
};

never: record_list func_list
{
    $$ = *nev = never_new(NULL, $1, $2);
};

never: enumtype_list record_list func_list
{
    $$ = *nev = never_new($1, $2, $3);
};

%%



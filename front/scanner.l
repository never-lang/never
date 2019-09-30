/**
 * a scanner for never programming language
 */
%{
#include <stdio.h>
#include <stdarg.h>
#include "types.h"
#include "parser.h"
#include "utils.h"
#include "strutil.h"

#define YY_DECL int lex_scan(token * tokp)

unsigned int line_no = 1;
string * string_value = NULL;

void scanner_printf(const char * format, ...)
{
    va_list ap;

    va_start(ap, format);
    /* vprintf(format, ap); */
    va_end(ap);
}

%}

DIGIT [0-9]
HEX_DIGIT[0-9a-fA-F]
ID [a-zA-Z_][a-zA-Z0-9_]*

%option pointer
%option noyylineno
%option noyywrap
%option nounistd

%x C_COMMENT
%x C_STRING

%%
[ \t\r]+

\n {
	line_no++;
}

"#".* {
     /* DO NOTHING */
}

catch {
    tokp->line_no = line_no;
    scanner_printf("catch\n");
    return TOK_CATCH;
}

for {
    tokp->line_no = line_no;
    scanner_printf("for\n");
    return TOK_FOR;
}

let {
    tokp->line_no = line_no;
    scanner_printf("let\n");
    return TOK_LET;
}

var {
    tokp->line_no = line_no;
    scanner_printf("var\n");
    return TOK_VAR;
}

throw {
    tokp->line_no = line_no;
    scanner_printf("throw\n");
    return TOK_THROW;
}

func {
    tokp->line_no = line_no;
    scanner_printf("func\n");
    return TOK_FUNC;
}

while {
    tokp->line_no = line_no;
    scanner_printf("while\n");
    return TOK_WHILE;
}

do {
    tokp->line_no = line_no;
    scanner_printf("do\n");
    return TOK_DO;
}

if {
    tokp->line_no = line_no;
    scanner_printf("if\n");
    return TOK_IF;
}

in {
    tokp->line_no = line_no;
    scanner_printf("in\n");
    return TOK_IN;
}

enum {
    tokp->line_no = line_no;
    scanner_printf("enum\n");
    return TOK_ENUM;
}

else {
    tokp->line_no = line_no;
    scanner_printf("if\n");
    return TOK_ELSE;
}

extern {
    tokp->line_no = line_no;
    scanner_printf("\n");
    return TOK_EXTERN;
}

int {
    tokp->line_no = line_no;
    scanner_printf("int\n");
    return TOK_INT;
}

float {
    tokp->line_no = line_no;
    scanner_printf("float\n");
    return TOK_FLOAT;
}

record {
    tokp->line_no = line_no;
    scanner_printf("record\n");
    return TOK_RECORD;
} 

string {
    tokp->line_no = line_no;
    scanner_printf("string\n");
    return TOK_STRING;
}

nil {
    tokp->line_no = line_no;
    scanner_printf("nil\n");
    return TOK_NIL;
}

"+"|"-"|"/"|"*"|"%"|"=" {
    tokp->line_no = line_no;
    scanner_printf("operator %s\n", yytext);
    return yytext[0];
}

"?"|":" {
    tokp->line_no = line_no;
    scanner_printf("%s\n", yytext);
    return yytext[0];
}

"==" {
    tokp->line_no = line_no;
    scanner_printf("==\n");
    return TOK_EQ;
}

"!=" {
    tokp->line_no = line_no;
    scanner_printf("!=\n");
    return TOK_NEQ;
}

"<"|">" {
    tokp->line_no = line_no;
    scanner_printf("<>= %s\n", yytext);
    return yytext[0];
}

"<=" {
    tokp->line_no = line_no;
    scanner_printf("<=\n");
    return TOK_LTE;
}

">=" {
    tokp->line_no = line_no;
    scanner_printf("=>\n");
    return TOK_GTE;
}

"->" {
    tokp->line_no = line_no;
    scanner_printf("return %s\n", yytext);
    return TOK_RET;
}

"&&" {
    tokp->line_no = line_no;
    scanner_printf("&&\n", yytext);
    return TOK_AND;
}

"|" {
    tokp->line_no = line_no;
    scanner_printf("|\n", yytext);
    return yytext[0];
}

"||" {
    tokp->line_no = line_no;
    scanner_printf("||\n", yytext);
    return TOK_OR;
}

"!" {
    tokp->line_no = line_no;
    scanner_printf("!\n", yytext);
    return TOK_NOT;
}

"{[" {
    tokp->line_no = line_no;
    scanner_printf("beg array dims\n");
    return ARR_DIM_BEG;
}

"]}" {
    tokp->line_no = line_no;
    scanner_printf("end array dims\n");
    return ARR_DIM_END;
}

"("|")" {
    tokp->line_no = line_no;
    scanner_printf("open|close paren\n");
    return yytext[0];
}

"{"|"}" {
    tokp->line_no = line_no;
    scanner_printf("open|close bracket\n");
    return yytext[0];
}

"["|"]" {
    tokp->line_no = line_no;
    scanner_printf("open|close quare backet\n");
    return yytext[0];
}

","|";" {
    tokp->line_no = line_no;
    scanner_printf("%s\n", yytext);
    return yytext[0];
}

"." {
    tokp->line_no = line_no;
    scanner_printf("%s\n", yytext);
    return TOK_DOT;
}

{DIGIT}+ {
    int result;
    tokp->line_no = line_no;
    sscanf(yytext, "%d", &result);
    tokp->val.int_value = result;
    scanner_printf("num_int %d\n", result);
    return TOK_NUM_INT;
}

0x{HEX_DIGIT}+|0X{HEX_DIGIT}+ {
    int result;
    tokp->line_no = line_no;
    sscanf(yytext, "%x", &result);
    tokp->val.int_value = result;
    scanner_printf("num_int %x\n", result);
    return TOK_NUM_INT;
}

{DIGIT}+"."{DIGIT}+ {
    float result;
    tokp->line_no = line_no;
    sscanf(yytext, "%f", &result);
    tokp->val.float_value = result;
    scanner_printf("num_float %f\n", result);
    return TOK_NUM_FLOAT;
}

{ID} {
    tokp->line_no = line_no;
    tokp->val.str_value = strdup(yytext);
    scanner_printf("id %s\n", yytext);
    return TOK_ID;
}

\" {
    string_value = string_new();
    BEGIN(C_STRING);
}

<C_STRING>\" {
    BEGIN(INITIAL);
    string_add_char(string_value, '\0');

    tokp->line_no = line_no;
    tokp->val.str_value = string_take(string_value);
    
    if (string_value)
    {
        scanner_printf("string %s\n", string_value->buf);

        string_delete(string_value);
        string_value = NULL;
    }

    return TOK_NUM_STRING;    
}

<C_STRING>\n {
    print_error_msg(line_no, "unterminated string\n");

    if (string_value) { string_delete(string_value); string_value = NULL; }
    yyterminate();
    return yytext[0];
}

<C_STRING>\\[0-7]{1,3} {
    int result;
    
    sscanf(yytext + 1, "%o", &result);
    if (result > 0xff)
    {
        print_error_msg(line_no, "constant out of range\n");
        
        if (string_value) { string_delete(string_value); string_value = NULL; }
        yyterminate();
        return yytext[0];
    }
    string_add_char(string_value, result);
}

<C_STRING>\\[0-9]+ {
    print_error_msg(line_no, "bad escape sequence\n");

    if (string_value) { string_delete(string_value); string_value = NULL; }
    yyterminate();
    return yytext[0];
}

<C_STRING>\\n {
    string_add_char(string_value, '\n');
}
<C_STRING>\\t {
    string_add_char(string_value, '\t');
}
<C_STRING>\\r {
    string_add_char(string_value, '\r');
}
<C_STRING>\\b {
    string_add_char(string_value, '\b');
}
<C_STRING>\\f {
    string_add_char(string_value, '\f');
}

<C_STRING>\\(.|\n) {
    line_no++;
    string_add_char(string_value, yytext[1]);
}

<C_STRING>[^\\\n\"]+ {
    char *yptr = yytext;
    while (*yptr)
    {
        string_add_char(string_value, *yptr++);
    }
}

"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { line_no++; }
<C_COMMENT>.    { }

<<EOF>> {
    yyterminate();
    return yytext[0];
}

%%

void scan_string(const char *src)
{
    yy_switch_to_buffer(yy_scan_string(src));
}


/**
 * a scanner for never programming language
 */
%{
#include <stdio.h>
#include <stdarg.h>
#include "types.h"
#include "parser.h"

#define YY_DECL int lex_scan(token * tokp)

unsigned int line_no = 1;

void scanner_printf(const char * format, ...)
{
    va_list ap;

    va_start(ap, format);
    /* vprintf(format, ap); */
    va_end(ap);
}

%}

DIGIT [0-9]
ID [a-zA-Z_][a-zA-Z0-9_]*

%option pointer
%option noyylineno
%option noyywrap
%option nounistd

%x C_COMMENT

%%
[ \t\r]+

\n {
	line_no++;
}

"#".* {
     /* DO NOTHING */
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

else {
    tokp->line_no = line_no;
    scanner_printf("if\n");
    return TOK_ELSE;
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

{DIGIT}+ {
    int result;
    tokp->line_no = line_no;
    sscanf(yytext, "%d", &result);
    tokp->val.int_value = result;
    scanner_printf("num_int %d\n", result);
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


/**
 * a scanner for never programming language
 */
%{
#include <stdio.h>
#include <stdarg.h>
#include "parser.h"
#include "types.h"

#define YY_DECL int lex_scan(token * tokp)

unsigned int line_number = 1;

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

%x C_COMMENT

%%
[ \t\r]+

\n {
	line_number++;
}

func {
    scanner_printf("func\n");
    return TOK_FUNC;
}

return {
    scanner_printf("return\n");
    return TOK_RETURN;    
}

int {
    scanner_printf("int\n");
    return TOK_INT;
}

"+"|"-"|"/"|"*" {
    scanner_printf("operator %s\n", yytext);
    return yytext[0];
}

"?"|":" {
    scanner_printf("%s\n", yytext);
    return yytext[0];
}

"==" {
    scanner_printf("==\n");
    return TOK_EQ;
}

"<"|">" {
    scanner_printf("<>= %s\n", yytext);
    return yytext[0];
}

"<=" {
    scanner_printf("<=\n");
    return TOK_LTE;
}

">=" {
    scanner_printf("=>\n");
    return TOK_GTE;
}

"->" {
    scanner_printf("return %s\n", yytext);
    return TOK_RET;
}

"("|")" {
    scanner_printf("open|close paren\n");
    return yytext[0];
}

"{"|"}" {
    scanner_printf("open|close bracket\n");
    return yytext[0];
}

","|";" {
    scanner_printf("%s\n", yytext);
    return yytext[0];
}

{DIGIT}+ {
    int result;
    sscanf(yytext, "%i", &result);
    tokp->val.int_value = result;

    scanner_printf("number %d\n", result);
    return TOK_NUM;
}

{ID} {
    tokp->val.str_value = strdup(yytext);
    scanner_printf("id %s\n", yytext);
    return TOK_ID;
}

"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { }
<C_COMMENT>.    { }

<<EOF>> {
    yyterminate();
    return yytext[0];
}

%%


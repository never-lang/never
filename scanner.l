/**
 * a scanner for never programming language
 */
%{
#include <stdio.h>
#include "parser.h"
%}

DIGIT [0-9]
ID [a-zA-Z_][a-zA-Z0-9_]*

%option noyywrap

%%

func {
    printf("func\n");
    return TOK_FUNC;
}

return {
    printf("return\n");
    return TOK_RETURN;    
}

int {
    printf("int\n");
    return TOK_INT;
}

"+"|"-"|"/"|"*" {
    printf("operator %s\n", yytext);
    return yytext[0];
}

"?"|":" {
    printf("%s\n", yytext);
    return yytext[0];
}

"<"|">"|"=" {
    printf("<>= %s\n", yytext);
    return yytext[0];
}

"->" {
    printf("return %s\n", yytext);
    return TOK_RET;
}

"("|")" {
    printf("open|close paren\n");
    return yytext[0];
}

"{"|"}" {
    printf("open|close bracket\n");
    return yytext[0];
}

","|";" {
    printf("%s\n", yytext);
    return yytext[0];
}

{DIGIT}+ {
    printf("number %s\n", yytext);
    return TOK_NUM;
}

{ID} {
    printf("id %s\n", yytext);
    return TOK_ID;
}

<<EOF>> {
    yyterminate();
    return yytext[0];
}

%%


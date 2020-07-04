/**
 * Copyright 2018-2020 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

%{

#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include "types.h"
#include "parser.h"
#include "utils.h"
#include "strutil.h"
#include "moduletab.h"

#define MAX_USE_DEPTH 10
#define MAX_FILE_NAME_LEN 100
#define YY_DECL int lex_scan(token * tokp)

unsigned int line_no = 1;
string * string_value = NULL;

typedef struct use_descr
{
    unsigned int line_no;
    const char * file_name;
    FILE * yyin;
    YY_BUFFER_STATE buffer_state;
} use_descr;

int use_stack_ptr = 0;
use_descr use_stack[MAX_USE_DEPTH];
moduletab * modtab = NULL;

void set_line_no(unsigned int value)
{
    line_no = value;
}

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
%x USE
%x MODULE_REF

%%
<INITIAL,USE>[ \t\r]+

<INITIAL,USE>\n {
	line_no++;
}

"#".* {
     /* DO NOTHING */
}

bool {
    tokp->line_no = line_no;
    scanner_printf("bool\n");
    return TOK_BOOL;
}

true {
    tokp->line_no = line_no;
    scanner_printf("true\n");
    return TOK_TRUE;
}

false {
    tokp->line_no = line_no;
    scanner_printf("false\n");
    return TOK_FALSE;
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

char {
    tokp->line_no = line_no;
    scanner_printf("char\n");
    return TOK_CHAR;
}

string {
    tokp->line_no = line_no;
    scanner_printf("string\n");
    return TOK_STRING;
}

c_ptr {
    tokp->line_no = line_no;
    scanner_printf("c_ptr\n");
    return TOK_C_PTR;
}

c_null {
    tokp->line_no = line_no;
    scanner_printf("c_null\n");
    return TOK_C_NULL;
}

void {
    tokp->line_no = line_no;
    scanner_printf("void\n");
    return TOK_VOID;
}

nil {
    tokp->line_no = line_no;
    scanner_printf("nil\n");
    return TOK_NIL;
}

match {
    tokp->line_no = line_no;
    scanner_printf("match\n");
    return TOK_MATCH;
}

range {
    tokp->line_no = line_no;
    scanner_printf("range\n");
    return TOK_RANGE;
}

module {
    tokp->line_no = line_no;
    scanner_printf("module\n");
    return TOK_MODULE;
}

use {
    tokp->line_no = line_no;
    scanner_printf("use\n");
    BEGIN(USE);
    return TOK_USE;
}

"::" {
    tokp->line_no = line_no;
    scanner_printf("::\n");
    return TOK_DDOT;
}

".." {
    tokp->line_no = line_no;
    scanner_printf("..\n");
    return TOK_TODOTS;
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

\'[[:print:]]\' {
    tokp->line_no = line_no;
    tokp->val.char_value = yytext[1];
    scanner_printf("char %c\n", yytext[1]);
    return TOK_NUM_CHAR;
}

\" {
    string_value = string_new();
    BEGIN(C_STRING);
}

<MODULE_REF>[.\n] {
    tokp->line_no = line_no;
    line_no++;
    BEGIN(INITIAL);
    return TOK_MODULE_REF;
}

<USE>[a-zA-Z_./]+ {
    char file_name[MAX_FILE_NAME_LEN];
    
    if (use_stack_ptr >= MAX_USE_DEPTH)
    {
        fprintf(stderr, "module uses are nested too deep\n");

        tokp->line_no = line_no;
        tokp->val.str_value = strdup(yytext);
        scanner_printf("id %s\n", yytext);

        BEGIN(MODULE_REF);
        return TOK_NUM_STRING;
    }

    moduletab_entry * entry = moduletab_lookup_module(modtab, yytext);
    if (entry != NULL)
    {
        tokp->line_no = line_no;
        tokp->val.str_value = strdup(yytext);
        scanner_printf("use %s\n", yytext);

        BEGIN(MODULE_REF);
        return TOK_NUM_STRING;
    }

    use_descr descr = { line_no, get_utils_file_name(), yyin, YY_CURRENT_BUFFER };

    FILE * current_yyin = yyin;
    snprintf(file_name, MAX_FILE_NAME_LEN, "%s.nev", yytext);
    yyin = fopen(file_name, "r");
    if (!yyin)
    {
        fprintf(stderr, "cannot open module %s\n", file_name);
        yyin = current_yyin;
        return yytext[0];
    }

    use_stack[use_stack_ptr++] = descr;

    tokp->line_no = line_no;
    tokp->val.str_value = strdup(yytext);
    scanner_printf("use %s\n", yytext);

    moduletab_add_module(modtab, strdup(yytext));

    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    line_no = 1;
    set_utils_file_name(yytext);

    BEGIN(INITIAL);
    return TOK_NUM_STRING;
}

<C_STRING>\" {
    string_add_char(string_value, '\0');

    tokp->line_no = line_no;
    tokp->val.str_value = string_take(string_value);
    
    if (string_value)
    {
        scanner_printf("string %s\n", string_value->buf);

        string_delete(string_value);
        string_value = NULL;
    }

    BEGIN(INITIAL);
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
    if (yyin)
    {
        fclose(yyin);
        yyin = NULL;
    }

    if (--use_stack_ptr < 0)
    {
        yyterminate();
    }
    else
    {
        line_no = use_stack[use_stack_ptr].line_no;
        set_utils_file_name(use_stack[use_stack_ptr].file_name);
        yy_delete_buffer(YY_CURRENT_BUFFER);
        yy_switch_to_buffer(use_stack[use_stack_ptr].buffer_state);
    }
}

%%

void scan_string(const char *src)
{
    yy_switch_to_buffer(yy_scan_string(src));
}

void scan_file(const char * file)
{
    set_utils_file_name(file);

    yyin = fopen(file, "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "cannot open %s. %s\n", file, strerror(errno));
        exit(1);
    }

    modtab = moduletab_new(8);
    moduletab_add_module(modtab, strdup(file));
}

void scanner_destroy()
{
    if (yyin)
    {
        fclose(yyin);
        yyin = NULL;
    }

    while (--use_stack_ptr >= 0)
    {
        if (use_stack[use_stack_ptr].yyin)
        {
            fclose(use_stack[use_stack_ptr].yyin);
        }
        if (use_stack[use_stack_ptr].buffer_state)
        {
            yy_delete_buffer(use_stack[use_stack_ptr].buffer_state);
        }
    }

    if (modtab)
    {
        moduletab_delete(modtab);
    }

    yylex_destroy();
}

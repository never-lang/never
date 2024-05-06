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
#include "assert.h"

#define NEVER_PATH "NEVER_PATH"
#define MAX_USE_DEPTH 16
#define MAX_FILE_NAME_LEN 256
#define MAX_NEVER_PATH_LEN 1024
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

FILE * fopen_path(const char * file_name);

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

const {
    tokp->line_no = line_no;
    scanner_printf("const\n");
    return TOK_CONST;
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

long {
    tokp->line_no = line_no;
    scanner_printf("long\n");
    return TOK_LONG;
}

double {
    tokp->line_no = line_no;
    scanner_printf("double\n");
    return TOK_DOUBLE;
}

"~~~" {
    tokp->line_no = line_no;
    scanner_printf("~~~\n", yytext);
    return TOK_BIN_NOT;
}

"&&&" {
    tokp->line_no = line_no;
    scanner_printf("&&&\n", yytext);
    return TOK_BIN_AND;
}

"|||" {
    tokp->line_no = line_no;
    scanner_printf("|||\n", yytext);
    return TOK_BIN_OR;
}

"^^^" {
    tokp->line_no = line_no;
    scanner_printf("^^^\n", yytext);
    return TOK_BIN_XOR;
}

"<<<" {
    tokp->line_no = line_no;
    scanner_printf("<<<\n", yytext);
    return TOK_BIN_SHL;
}

">>>" {
    tokp->line_no = line_no;
    scanner_printf(">>>\n", yytext);
    return TOK_BIN_SHR;
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

"|" {
    tokp->line_no = line_no;
    scanner_printf("|\n", yytext);
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

"<-" {
    tokp->line_no = line_no;
    scanner_printf("<-\n", yytext);
    return TOK_IN;
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

"." {
    tokp->line_no = line_no;
    scanner_printf("%s\n", yytext);
    return TOK_DOT;
}

"|>" {
    tokp->line_no = line_no;
    scanner_printf("%s\n", yytext);
    return TOK_PIPEL;
}

{DIGIT}+[lL] {
    tokp->line_no = line_no;
    sscanf(yytext, "%lld", &tokp->val.long_value);
    scanner_printf("num_long %lld\n", tokp->val.long_value);
    return TOK_NUM_LONG;
}

{DIGIT}+ {
    tokp->line_no = line_no;
    sscanf(yytext, "%d", &tokp->val.int_value);
    scanner_printf("num_int %d\n", tokp->val.int_value);
    return TOK_NUM_INT;
}

0[xX]{HEX_DIGIT}+[lL] {
    tokp->line_no = line_no;
    sscanf(yytext, "%llx", &tokp->val.long_value);
    scanner_printf("long_int %llx\n", tokp->val.long_value);
    return TOK_NUM_LONG;
}

0[xX]{HEX_DIGIT}+ {
    tokp->line_no = line_no;
    sscanf(yytext, "%x", &tokp->val.int_value);
    scanner_printf("num_int %x\n", tokp->val.int_value);
    return TOK_NUM_INT;
}

{DIGIT}+"."{DIGIT}+[fF]* {
    tokp->line_no = line_no;
    sscanf(yytext, "%f", &tokp->val.float_value);
    scanner_printf("num_float %f\n", tokp->val.float_value);
    return TOK_NUM_FLOAT;
}

{DIGIT}+"."{DIGIT}+[dD] {
    tokp->line_no = line_no;
    sscanf(yytext, "%lf", &tokp->val.double_value);
    scanner_printf("num_double %f\n", tokp->val.double_value);
    return TOK_NUM_DOUBLE;
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

. {
    scanner_printf("unknown %c\n", yytext[0]);
    return yytext[0];
}

<MODULE_REF>[. \t] {
    BEGIN(INITIAL);
    return TOK_MODULE_REF;
}

<MODULE_REF>[\n] {
    line_no++;
    BEGIN(INITIAL);
    return TOK_MODULE_REF;
}

<USE>[a-zA-Z_./]+ {
    char file_name[MAX_FILE_NAME_LEN] = { 0 };
    
    if (use_stack_ptr >= MAX_USE_DEPTH)
    {
        print_error_msg(line_no, "module uses are nested too deep");

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

    yyin = fopen_path(file_name);
    if (!yyin)
    {
        print_error_msg(line_no, "cannot open module %s", yytext);
        yyin = current_yyin;

        tokp->line_no = line_no;
        tokp->val.str_value = strdup(yytext);
        scanner_printf("use %s\n", yytext);

        BEGIN(MODULE_REF);
        return TOK_NUM_STRING;
    }

    use_stack[use_stack_ptr++] = descr;

    tokp->line_no = line_no;
    tokp->val.str_value = strdup(yytext);
    scanner_printf("use %s\n", yytext);

    moduletab_add_module(modtab, strdup(yytext));

    line_no = 1;
    set_utils_file_name(tokp->val.str_value);
    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

    BEGIN(INITIAL);
    return TOK_NUM_STRING;
}

<C_STRING>\" {
    string_add_char_end(string_value, '\0');

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
    print_error_msg(line_no, "unterminated string");

    if (string_value) { string_delete(string_value); string_value = NULL; }

    yyterminate();
    return yytext[0];
}

<C_STRING>\\[0-7]{1,3} {
    int items;
    int result;
    
    items = sscanf(yytext + 1, "%o", &result);
    if (items > 3 || result > 0xff)
    {
        print_error_msg(line_no, "constant out of range");
        
        if (string_value) { string_delete(string_value); string_value = NULL; }

        yyterminate();
        return yytext[0];
    }
    string_add_char_end(string_value, result);
}

<C_STRING>\\[0-9]+ {
    print_error_msg(line_no, "bad escape sequence");

    if (string_value) { string_delete(string_value); string_value = NULL; }

    yyterminate();
    return yytext[0];
}

<C_STRING>\\n {
    string_add_char_end(string_value, '\n');
}
<C_STRING>\\t {
    string_add_char_end(string_value, '\t');
}
<C_STRING>\\r {
    string_add_char_end(string_value, '\r');
}
<C_STRING>\\b {
    string_add_char_end(string_value, '\b');
}
<C_STRING>\\f {
    string_add_char_end(string_value, '\f');
}

<C_STRING>\\(.|\n) {
    line_no++;
    string_add_char_end(string_value, yytext[1]);
}

<C_STRING>[^\\\n\"]+ {
    char *yptr = yytext;
    while (*yptr)
    {
        string_add_char_end(string_value, *yptr++);
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
        yy_flush_buffer(YY_CURRENT_BUFFER);
        yy_delete_buffer(YY_CURRENT_BUFFER);
        yy_switch_to_buffer(use_stack[use_stack_ptr].buffer_state);
    }
}

%%

FILE * fopen_path(const char * file_name)
{
    char * strp = NULL;
    char PATH[MAX_NEVER_PATH_LEN];

    strp = getenv(NEVER_PATH);
    if (strp == NULL || strcmp(strp, "") == 0)
    {
        return fopen(file_name, "r");
    }

    strncpy(PATH, strp, MAX_NEVER_PATH_LEN);
    PATH[MAX_NEVER_PATH_LEN - 1] = '\0';

    char * cwd = getcwd(NULL, 0);
    if (cwd == NULL)
    {
        print_error_msg(line_no, "cannot getcwd %s: %s", file_name, strerror(errno));
        return NULL;
    }

    int ret = chdir(cwd);
    if (ret < 0)
    {
        print_error_msg(line_no, "cannot chdir %s: %s", file_name, strerror(errno));
        free(cwd);
        return NULL;
    }

    strp = PATH;
    char * path = NULL;
    FILE * ffile = NULL;
    while ((path = strtok(strp, ":")) != NULL)
    {
        strp = NULL;

        ret = chdir(path);
        if (ret == 0)
        {
            ffile = fopen(file_name, "r");
            if (ffile != NULL)
            {
                ret = chdir(cwd);
                if (ret < 0)
                {
                    print_warning_msg(line_no, "cannot chdir to %s skipped: %s", cwd, strerror(errno));
                }
                break;
            }
        }
        else
        {
            print_warning_msg(line_no, "cannot chdir to %s skipped: %s", path, strerror(errno));
        }

        ret = chdir(cwd);
        if (ret < 0)
        {
            print_warning_msg(line_no, "cannot chdir to %s skipped: %s", cwd, strerror(errno));
        }
    }

    free(cwd);
    return ffile;
}

int scan_string(const char * src)
{
    use_stack_ptr = 0;
    set_utils_file_name("<stdin>");
    modtab = moduletab_new(8);

    yy_switch_to_buffer(yy_scan_string(src));

    return 0;
}

int scan_file(const char * file)
{
    use_stack_ptr = 0;
    set_utils_file_name(file);

    yyin = fopen(file, "r");
    if (yyin == NULL)
    {
        print_error_msg(line_no, "cannot open %s: %s", file, strerror(errno));
        return 1;
    }

    modtab = moduletab_new(8);
    moduletab_add_module(modtab, strdup(file));

    return 0;
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
            yy_flush_buffer(use_stack[use_stack_ptr].buffer_state);
            yy_delete_buffer(use_stack[use_stack_ptr].buffer_state);
        }
    }

    if (modtab)
    {
        moduletab_delete(modtab);
        modtab = NULL;
    }

    yylex_destroy();
}

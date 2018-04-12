#ifndef __SCANNER_H__
#define __SCANNER_H__ 1

#include "types.h"

int yylex_destroy();

int lex_scan(token *tokp);

#endif


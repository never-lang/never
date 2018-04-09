#ifndef __TYPES_H__
#define __TYPES_H__

typedef struct token_value
{
    int int_value;
    char * str_value;
} token_value;

typedef struct token
{
    int type;
    token_value val;
} token;

#define YYSTYPE token

#endif /* __TYPES_H__ */


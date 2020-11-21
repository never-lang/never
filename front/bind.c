/**
 * Copyright 2018 Slawomir Maludzinski
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
#include "bind.h"
#include <stdlib.h>
#include <stdio.h>

bind * bind_new_let(char * id, expr * expr_value)
{
    bind * value = (bind *)malloc(sizeof(bind));

    value->type = BIND_LET;
    value->id = id;
    value->expr_value = expr_value;
    value->index = 0;
    value->line_no = 0;

    return value;
}

bind * bind_new_var(char * id, expr * expr_value)
{
    bind * value = (bind *)malloc(sizeof(bind));

    value->type = BIND_VAR;
    value->id = id;
    value->expr_value = expr_value;
    value->index = 0;
    value->line_no = 0;

    return value;
}

void bind_delete(bind * value)
{
    if (value->id)
    {
        free(value->id);
    }
    if (value->expr_value)
    {
        expr_delete(value->expr_value);
    }

    free(value);
}

void bind_print(bind * value)
{
    printf("bind %s\n", value->id);
}




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

#include "never.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

never * never_new(use_list * uses, decl_list * decls, seq_list * exprs)
{
    never * n = (never *)malloc(sizeof(never));

    n->stab = NULL;
    n->uses = uses;
    n->decls = decls;
    n->exprs = exprs;

    return n;
}

void never_delete(never * nev)
{
    if (nev->uses)
    {
        use_list_delete(nev->uses);
    }
    if (nev->decls)
    {
        decl_list_delete(nev->decls);
    }
    if (nev->exprs)
    {
        seq_list_delete(nev->exprs);
    }
    if (nev->stab)
    {
        symtab_delete(nev->stab);
    }
    free(nev);
}

CC=gcc
CFLAGS=-Wall -g
LDFLAGS=
LEX=flex
BISON=bison
BFLAGS=--report=solved --defines

test_object: object.o

test_scanner: scanner.o

test_parser: scanner.o parser.o expr.o var.o func.o never.o symtab.o typecheck.o utils.o

test_symtab: symtab.o var.o func.o expr.o

test_freevar: freevar.o

gc: gc.o object.o scanner.o

%.c : %.y
	${BISON} ${BFLAGS} -o $@ $<

test_parser.o: test_parser.c parser.y expr.h var.h func.h never.h

expr.o: expr.c expr.h
var.o: var.c var.h
func.o: func.c func.h
symtab.o: symtab.c symtab.h
typecheck.o: typecheck.c typecheck.h symtab.h expr.h var.h func.h
utils.o: utils.c utils.h


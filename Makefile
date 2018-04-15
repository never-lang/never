CC=gcc
CFLAGS=-Wall -g
LDFLAGS=
LEX=flex
BISON=bison
BFLAGS=--report=solved --defines

test_object: object.o

test_scanner: scanner.o

test_parser: scanner.o parser.o expr.o arg.o func.o never.o symtab.o typecheck.o

test_symtab: symtab.o arg.o func.o expr.o

gc: gc.o object.o scanner.o

%.c : %.y
	${BISON} ${BFLAGS} -o $@ $<

test_parser.o: test_parser.c parser.y expr.h arg.h func.h never.h

expr.o: expr.c expr.h
arg.o: arg.c arg.h
func.o: func.c func.h
symtab.o: symtab.c symtab.h



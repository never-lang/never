CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-parameter -g
LDFLAGS=
LEX=flex
BISON=bison
BFLAGS=--report=solved --defines

test_object: object.o

test_scanner: scanner.o

test_parser: scanner.o parser.o expr.o var.o freevar.o func.o never.o symtab.o typecheck.o gencode.o utils.o bytecode.o vm.o gc.o object.o

test_symtab: symtab.o var.o freevar.o func.o expr.o

test_freevar: symtab.o var.o freevar.o func.o expr.o

test_vm: vm.o gc.o object.o utils.o

test_gc: gc.o object.o

gc: gc.o object.o scanner.o

%.c : %.y
	${BISON} ${BFLAGS} -o $@ $<

test_parser.o: test_parser.c parser.y expr.h var.h func.h never.h

deps:
	$(CC) -MM *.c *.h > .deps
 
include .deps




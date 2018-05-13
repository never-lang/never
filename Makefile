CC=gcc
CFLAGS=-Wall -g
LDFLAGS=
LEX=flex
BISON=bison
BFLAGS=--report=solved --defines

test_object: object.o

test_scanner: scanner.o

test_parser: scanner.o parser.o expr.o var.o freevar.o func.o never.o symtab.o typecheck.o gencode.o utils.o bytecode.o

test_symtab: symtab.o var.o freevar.o func.o expr.o

test_freevar: symtab.o var.o freevar.o func.o expr.o

test_vm: vm.o

test_gc: gc.o object.o

gc: gc.o object.o scanner.o

%.c : %.y
	${BISON} ${BFLAGS} -o $@ $<

test_parser.o: test_parser.c parser.y expr.h var.h func.h never.h

bytecode.o: bytecode.c bytecode.h func.h expr.h freevar.h var.h symtab.h
expr.o: expr.c expr.h freevar.h var.h func.h symtab.h
freevar.o: freevar.c freevar.h var.h func.h expr.h symtab.h
func.o: func.c func.h expr.h freevar.h var.h symtab.h
gc.o: gc.c gc.h object.h
gencode.o: gencode.c gencode.h never.h func.h expr.h freevar.h var.h symtab.h bytecode.h
never.o: never.c never.h func.h expr.h freevar.h var.h symtab.h
object.o: object.c object.h
parser.o: parser.c utils.h types.h expr.h freevar.h var.h func.h symtab.h never.h scanner.h
scanner.o: scanner.c types.h expr.h freevar.h var.h func.h symtab.h never.h parser.h
symtab.o: symtab.c symtab.h var.h func.h expr.h freevar.h
test_freevar.o: test_freevar.c freevar.h var.h
test_gc.o: test_gc.c gc.h object.h
test_object.o: test_object.c object.h
test_parser.o: test_parser.c scanner.h types.h expr.h freevar.h var.h func.h symtab.h never.h parser.h typecheck.h gencode.h bytecode.h
test_scanner.o: test_scanner.c types.h expr.h freevar.h var.h func.h symtab.h never.h scanner.h
test_symtab.o: test_symtab.c symtab.h var.h func.h expr.h freevar.h
test_vm.o: test_vm.c vm.h bytecode.h
typecheck.o: typecheck.c typecheck.h never.h func.h expr.h freevar.h var.h symtab.h
utils.o: utils.c
var.o: var.c var.h
vm.o: vm.c vm.h bytecode.h
bytecode.o: bytecode.h
expr.o: expr.h freevar.h var.h
freevar.o: freevar.h var.h
func.o: func.h expr.h freevar.h var.h symtab.h func.h
gc.o: gc.h object.h
gencode.o: gencode.h never.h func.h expr.h freevar.h var.h symtab.h bytecode.h
never.o: never.h func.h expr.h freevar.h var.h symtab.h
object.o: object.h
parser.o: parser.h
scanner.o: scanner.h types.h expr.h freevar.h var.h func.h symtab.h never.h
symtab.o: symtab.h var.h func.h expr.h freevar.h symtab.h
typecheck.o: typecheck.h never.h func.h expr.h freevar.h var.h symtab.h
types.o: types.h expr.h freevar.h var.h func.h symtab.h never.h
utils.o: utils.h
var.o: var.h
vm.o: vm.h bytecode.h




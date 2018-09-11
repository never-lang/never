cmake_minimum_required(VERSION 2.8)
enable_language(C)

if (MSVC)
  # Statically links against the standard library
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MT")
endif (MSVC)


# test
include_directories(".." "../include" "../front" "../back")
add_executable(test_exec test_exec.c)

add_executable(test_freevar test_freevar.c)
add_executable(test_gc test_gc.c)
add_executable(test_libmath test_libmath.c)
add_executable(test_object test_object.c)
# add_executable(test_scanner test_scanner.c)
add_executable(test_symtab test_symtab.c)
add_executable(test_vm test_vm.c)
add_executable(test_exctab test_exctab.c)

add_test(NAME test_vm COMMAND test_vm)
add_test(NAME test_symtab COMMAND test_symtab)
# add_test(NAME test_scanner COMMAND test_scanner)
add_test(NAME test_object COMMAND test_object)
add_test(NAME test_libmath COMMAND test_libmath)
add_test(NAME test_gc COMMAND test_gc)
add_test(NAME test_freevar COMMAND test_freevar)
add_test(NAME test_exec COMMAND test_exec)
add_test(NAME test_exctab COMMAND test_exctab)

target_link_libraries(test_vm nev)
# target_link_libraries(test_scanner nev)
target_link_libraries(test_symtab nev)
target_link_libraries(test_object nev)
target_link_libraries(test_libmath nev)
target_link_libraries(test_gc nev)
target_link_libraries(test_freevar nev)
target_link_libraries(test_exec nev)
target_link_libraries(test_exctab nev)


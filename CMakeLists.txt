cmake_minimum_required(VERSION 3.0.2)
project(never)
enable_language(C)

find_package(BISON 2.7.1)
find_package(FLEX)
BISON_TARGET(parser ${CMAKE_CURRENT_SOURCE_DIR}/front/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/front/parser.c VERBOSE parser.txt)
FLEX_TARGET(scanner ${CMAKE_CURRENT_SOURCE_DIR}/front/scanner.l  ${CMAKE_CURRENT_SOURCE_DIR}/front/scanner.c)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)

if (MSVC)
  # Statically links against the standard library
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MT")

  # Use secure functions by defaualt and suppress warnings about "deprecated" functions
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_NONSTDC_NO_WARNINGS=1 /D _CRT_SECURE_NO_WARNINGS=1")
endif (MSVC)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-unused-function")
  if(ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  endif(ENABLE_COVERAGE)
endif ()

file(GLOB SRC
	"back/*.c"
	"front/*.c"
)

include_directories("include" "front" "back" ".")

add_library(nev STATIC
	${BISON_parser_OUTPUTS}
	${FLEX_scanner_OUTPUTS}
	${SRC}
)

if (WASM_ONLY OR JS_ONLY)
  target_link_libraries(nev m dl)
else()
  target_link_libraries(nev m dl ffi)
endif()

add_executable(${CMAKE_PROJECT_NAME} main.c getopt.c)
target_link_libraries(${CMAKE_PROJECT_NAME} nev)

if(WASM_ONLY)
  message(STATUS "Setting compilation target to native WASM")
  set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -DNO_FFI")
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-s WASM=1 -s EXPORTED_FUNCTIONS='[_never]' -s EXTRA_EXPORTED_RUNTIME_METHODS='[ccall, cwrap]'")
endif(WASM_ONLY)

if(JS_ONLY)
  message(STATUS "Setting compilation target to native JavaScript")
  set(CMAKE_EXECUTABLE_SUFFIX ".js")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNDEBUG -DNO_FFI")
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-s WASM=0 -s EXPORTED_FUNCTIONS='[_never]' -s EXTRA_EXPORTED_RUNTIME_METHODS='[ccall, cwrap]' --pre-js ../never-init.js")
endif(JS_ONLY)

# test
enable_testing()
add_subdirectory(test)


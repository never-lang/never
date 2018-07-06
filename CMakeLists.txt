cmake_minimum_required(VERSION 2.8)
project(never)
enable_language(C)

find_package(BISON)
find_package(FLEX)
BISON_TARGET(parser ${CMAKE_CURRENT_SOURCE_DIR}/front/parser.y ${CMAKE_CURRENT_SOURCE_DIR}/front/parser.c)
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
target_link_libraries(nev m)

add_executable(${CMAKE_PROJECT_NAME} main.c getopt.c)
target_link_libraries(${CMAKE_PROJECT_NAME} nev)


# test
enable_testing()
add_subdirectory(test)


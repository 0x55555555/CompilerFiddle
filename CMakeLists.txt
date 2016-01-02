cmake_minimum_required(VERSION 3.4)

project("CompilerFiddle")

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:\\GnuWin32")
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(MyScanner lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(test_compiler
  main.cpp
  lexer.l
  parser.y
  ${BISON_MyParser_OUTPUTS}
  ${FLEX_MyScanner_OUTPUTS}
)

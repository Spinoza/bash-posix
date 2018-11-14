cmake_minimum_required(VERSION 3.0)

project(42SH LANGUAGES C)

add_compile_options(-g -Wall -Wextra -Werror -pedantic -std=c99)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/includes
)

add_library(linked_list ${CMAKE_SOURCE_DIR}/src/linked_list.c)

add_library(lexer ${CMAKE_SOURCE_DIR}/src/lexer.c)
target_link_libraries(lexer PRIVATE linked_list)

add_library(grammar_check ${CMAKE_SOURCE_DIR}/src/grammar_check.c)
target_link_libraries(grammar_check PUBLIC lexer)

add_executable(lexer_main ${CMAKE_SOURCE_DIR}/src/tests/lexer_main.c)
target_link_libraries(lexer_main lexer)

add_executable(grammar_main ${CMAKE_SOURCE_DIR}/src/tests/grammar_main.c)
target_link_libraries(grammar_main grammar_check)

## TO FIX LATER WHEN TESTSUITE IS DONE
#[[add_custom_target(check_lexer
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_suite.py \
    ${CMAKE_SOURCE_DIR}/build/lexer_main

    COMMENT "Running tests on lexer, building token linked_list"
    DEPENDS lexer_main
)]]

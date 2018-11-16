cmake_minimum_required(VERSION 3.0)

project(42SH LANGUAGES C)

add_compile_options(-g -Wall -Wextra -Werror -pedantic -std=c99)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/includes
)

add_library(linked_list ${CMAKE_SOURCE_DIR}/src/linked_list.c)

add_library(lexer ${CMAKE_SOURCE_DIR}/src/lexer.c)
target_link_libraries(lexer PUBLIC linked_list)

add_library(grammar ${CMAKE_SOURCE_DIR}/src/grammar_check.c
    ${CMAKE_SOURCE_DIR}/src/grammar_command.c
    ${CMAKE_SOURCE_DIR}/src/grammar_rule.c
    ${CMAKE_SOURCE_DIR}/src/grammar_group.c
    )
add_library(execution ${CMAKE_SOURCE_DIR}/src/execution_ast.c)

add_library(grammar_check ${CMAKE_SOURCE_DIR}/src/grammar_check.c
    ${CMAKE_SOURCE_DIR}/src/grammar_command.c
    ${CMAKE_SOURCE_DIR}/src/grammar_rule.c
    ${CMAKE_SOURCE_DIR}/src/grammar_group.c
)
target_link_libraries(grammar_check PUBLIC lexer)

add_library(ast ${CMAKE_SOURCE_DIR}/src/ast.c
    ${CMAKE_SOURCE_DIR}/src/ast_print.c)

add_library(options ${CMAKE_SOURCE_DIR}/src/options.c)

add_executable(exec_main ${CMAKE_SOURCE_DIR}/src/tests/exec_main.c)
target_link_libraries(exec_main execution)
target_link_libraries(exec_main ast)
target_link_libraries(exec_main grammar_check)
target_link_libraries(exec_main lexer)
target_link_libraries(exec_main linked_list)

add_executable(lexer_main ${CMAKE_SOURCE_DIR}/src/tests/lexer_main.c)
target_link_libraries(lexer_main lexer)

add_executable(grammar_main ${CMAKE_SOURCE_DIR}/src/tests/grammar_main.c)
target_link_libraries(grammar_main ast)
target_link_libraries(grammar_main grammar)
target_link_libraries(grammar_main lexer)

add_custom_target(set_env
    COMMAND ${CMAKE_SOURCE_DIR}/src/tests/test_scripts/set_env.sh
    )

add_custom_target(check_lexer
    COMMENT "Running tests on lexer, building token linked_list\n"
    COMMAND ${CMAKE_SOURCE_DIR}/src/tests/test_scripts/lexer.sh
    DEPENDS lexer_main
)
add_custom_target(check_grammar
    COMMENT "Running tests on grammar,checking the token list\n"
    COMMAND ${CMAKE_SOURCE_DIR}/src/tests/test_scripts/grammar.sh
    DEPENDS lexer_main
)

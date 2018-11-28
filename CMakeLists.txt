cmake_minimum_required(VERSION 3.0)

project(42SH LANGUAGES C)

add_compile_options(-g -Wall -Wextra -Werror -pedantic -std=c99
    -D_DEFAULT_SOURCE)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/includes
)


##LIBRARIES


add_library(linked_list ${CMAKE_SOURCE_DIR}/src/utils/linked_list.c)

add_library(lexer ${CMAKE_SOURCE_DIR}/src/lexer/lexer.c)
target_link_libraries(lexer PRIVATE linked_list)

add_library(history ${CMAKE_SOURCE_DIR}/src/history/history.c)

add_library(tty ${CMAKE_SOURCE_DIR}/src/prompt/handlePS1.c)
target_link_libraries(tty readline)

add_library(grammar ${CMAKE_SOURCE_DIR}/src/grammar/grammar_check.c
    ${CMAKE_SOURCE_DIR}/src/grammar/grammar_command.c
    ${CMAKE_SOURCE_DIR}/src/grammar/grammar_rule.c
    ${CMAKE_SOURCE_DIR}/src/grammar/grammar_group.c
    )

target_link_libraries(grammar PRIVATE lexer)
target_link_libraries(grammar PRIVATE tty)

add_library(execution ${CMAKE_SOURCE_DIR}/src/ast/execution_ast.c)
target_link_libraries(execution ast)

add_library(ast ${CMAKE_SOURCE_DIR}/src/ast/ast.c
    ${CMAKE_SOURCE_DIR}/src/ast/ast_print.c)
target_link_libraries(ast grammar)

add_library(options ${CMAKE_SOURCE_DIR}/src/utils/options.c)

##EXECUTABLES

add_executable(42sh ${CMAKE_SOURCE_DIR}/src/42sh.c)
target_link_libraries(42sh execution)
target_link_libraries(42sh ast)
target_link_libraries(42sh options)
target_link_libraries(42sh tty)
target_link_libraries(42sh readline)
target_link_libraries(42sh history)

add_custom_target(set_env
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/set_env.sh
    )

add_custom_target(check
    COMMENT "Running all tests with timeout of 5sc.\n"
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/check.sh
    DEPENDS 42sh
    )

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

add_library(tty ${CMAKE_SOURCE_DIR}/src/prompt/handlePS1.c)


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


add_executable(lexer_main ${CMAKE_SOURCE_DIR}/tests/lexer_main.c)
target_link_libraries(lexer_main PRIVATE lexer)

add_executable(ast_main ${CMAKE_SOURCE_DIR}/tests/ast_main.c)
target_link_libraries(ast_main ast)

add_executable(grammar_main ${CMAKE_SOURCE_DIR}/tests/grammar_main.c)
target_link_libraries(grammar_main ast)

add_executable(42sh ${CMAKE_SOURCE_DIR}/src/42sh.c)
target_link_libraries(42sh execution)
target_link_libraries(42sh ast)
target_link_libraries(42sh options)
target_link_libraries(42sh tty)
target_link_libraries(42sh readline)

add_custom_target(set_env
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/set_env.sh
    )

add_custom_target(check_lexer
    COMMENT "Running tests on lexer, building token linked_list\n"
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/test_options.sh -c lexer
    DEPENDS lexer_main
)
add_custom_target(check_grammar
    COMMENT "Running tests on grammar,checking the token list\n"
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/test_options.sh -c grammar
    DEPENDS grammar_main
)
add_custom_target(check_ast
    COMMENT "Running tests on ast,checking that the dot file produced is
    correct\n"
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/test_options.sh -c ast-print
    DEPENDS ast_main
)
add_custom_target(check_42sh
    COMMENT "Running tests on 42sh, comparing with bash\n"
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/test_options.sh -c 42sh
    DEPENDS 42sh
)
add_custom_target(check
    COMMENT "Running all tests.\n"
    COMMAND ${CMAKE_SOURCE_DIR}/tests/test_scripts/test_options.sh -c all
    DEPENDS lexer_main
    DEPENDS grammar_main
    DEPENDS ast_main
    DEPENDS 42sh
    )

add_custom_target(check_sanity
    )

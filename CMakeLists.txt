cmake_minimum_required(VERSION 3.0)

project(42SH LANGUAGES C)

add_compile_options(-g -Wall -Wextra -Werror -pedantic -std=c99)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/includes
)

add_library(linked_list ${CMAKE_SOURCE_DIR}/src/linked_list.c)

add_library(lexer ${CMAKE_SOURCE_DIR}/src/lexer.c)
target_link_libraries(lexer PRIVATE linked_list)

add_library(grammar ${CMAKE_SOURCE_DIR}/src/grammar_check.c
    ${CMAKE_SOURCE_DIR}/src/grammar_command.c
    ${CMAKE_SOURCE_DIR}/src/grammar_rule.c
    ${CMAKE_SOURCE_DIR}/src/grammar_group.c
    )
add_executable(options ${CMAKE_SOURCE_DIR}/src/options.c)

add_executable(lexer_main ${CMAKE_SOURCE_DIR}/src/tests/lexer_main.c)
target_link_libraries(lexer_main lexer)

add_executable(grammar_main ${CMAKE_SOURCE_DIR}/src/tests/grammar_main.c)
target_link_libraries(grammar_main grammar)
target_link_libraries(grammar_main lexer)

add_custom_target(set_env
    COMMAND ${CMAKE_SOURCE_DIR}/build/set_env.sh
    )

add_custom_target(check_lexer
    COMMENT "Running tests on lexer, building token linked_list"
    COMMAND ${CMAKE_SOURCE_DIR}/build/activate.sh
    DEPENDS lexer_main
    COMMAND deactivate
)

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

add_executable(options ${CMAKE_SOURCE_DIR}/src/options.c)

add_executable(lexer_main ${CMAKE_SOURCE_DIR}/src/tests/lexer_main.c)
target_link_libraries(lexer_main lexer)

add_executable(grammar_main ${CMAKE_SOURCE_DIR}/src/tests/grammar_main.c)
target_link_libraries(grammar_main grammar_check)

add_custom_target(set_env
    COMMAND pip install -r ${CMAKE_SOURCE_DIR}/src/tests/test_suite/requirements.txt
    COMMAND python -m venv env
    )

execute_process(
    COMMAND source ${CMAKE_SOURCE_DIR}/build/env/bin/activate
    )

add_custom_target(check_lexer
    COMMAND pytest ${CMAKE_SOURCE_DIR}/src/tests/test_suite/.
    COMMENT "Running tests on lexer, building token linked_list"
    DEPENDS lexer_main
    COMMAND deactivate
)

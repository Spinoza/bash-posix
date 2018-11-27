sanity=0
lexer=0
grammar=0
ast_print=0
main=0
all=0
timeout=0

for (( i=1; i<=$#; i++))
do

    if [ "${!i}" = "--list" -o "${!i}" = "-l" ]; then
        echo "List of test categories to call with -c <category> : \n,
            'lexer','grammar','ast-print','42sh'"
    fi;
    if [ "${!i}" = "--timeout" -o "${!i}" = "-t" ]; then
        timeout=1
    fi;
    if [ "${!i}" = "--sanity" -o "${!i}" = "-s" ]; then
        sanity=1
    fi;
    if [ "${!i}" = "--category" -o "${!i}" = "-c" ]; then
        for (( j=$(($i + 1)); j <= $#; j++))
        do
            if [ "${!j}" = "all" ]; then
                all=1
            fi;

            if [ "${!j}" = "lexer" ]; then
                lexer=1
            fi;

            if [ "${!j}" = "grammar" ]; then
                grammar=1
            fi;

            if [ "${!j}" = "ast-print" ]; then
                ast_print=1
            fi;
            if [ "${!j}" = "42sh" ]; then
                main=1
            fi;
        done
    fi;
done

#PYTEST_TIMEOUT=10
source ./env/bin/activate
if [ $sanity -eq 1 ]; then
    echo "call with valgrind"
fi;

if [ $all -eq 1 ]; then
    pytest "../tests/test_suite/"
fi;

if [ $lexer -eq 1 ]; then
    pytest "--timeout=10" "../tests/test_suite/unit_tests/test_lexer.yml"

fi;

if [ $grammar -eq 1 ]; then
    pytest "--timeout=10" "../tests/test_suite/unit_tests/test_grammar.yml"
fi;


if [ $ast_print -eq 1 ]; then
    pytest "--timeout=10" "../tests/test_suite/unit_tests/test_ast.yml"
fi;


if [ $main -eq 1 ]; then
    if [ $sanity -eq 1 ]; then
        echo "calling with valgrind"
        pytest "-s" "--timeout=10" "../tests/test_suite/42sh_tests/" "--valgrind=1"
    else
        pytest "--timeout=10" "../tests/test_suite/42sh_tests/"
    fi;
fi;


deactivate

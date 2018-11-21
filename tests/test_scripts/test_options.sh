sanity=0
lexer=0
grammar=0
ast_print=0
main=0

for (( i=1; i<=$#; i++))
do

    if [ "${!i}" = "--list" -o "${!i}" = "-l" ]; then
        echo "List of test categories to call with -c <category> : \n,
            'lexer','grammar','ast-print','42sh'"
    fi;
    if [ "${!i}" = "--sanity" -o "${!i}" = "-s" ]; then
        sanity=1
    fi;
    if [ "${!i}" = "--category" -o "${!i}" = "-c" ]; then
        for (( j=$(($i + 1)); j <= $#; j++))
        do
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

echo "grammar is : $grammar"
echo "lexer is : $lexer"
echo "ast-print is : $ast_print"
echo "sanity is : $sanity"
echo "main is : $main"
source ./env/bin/activate
if [ $sanity -eq 1 ]; then
    echo "call with valgrind"
fi;

if [ $lexer -eq 1 ]; then
    pytest "../tests/test_suite/test_lexer.yml"

fi;

if [ $grammar -eq 1 ]; then
    echo "before grammar"
    pytest "../tests/test_suite/test_grammar.yml"
fi;


if [ $ast_print -eq 1 ]; then
    pytest "../tests/test_suite/test_ast.yml"
fi;


if [ $main -eq 1 ]; then
    pytest "../tests/test_suite/test_42sh.yml"
fi;


deactivate

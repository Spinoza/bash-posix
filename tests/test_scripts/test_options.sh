sanity=0
commands=0
cases=0
loops=0
ifs=0
pipes=0
func=0
all=0
builtins=0
assignments=0
arithmetic=0
timeout=5

PURPLE='\033[1;35m'
NC='\033[0m'

for (( i=1; i<=$#; i++))
do
    if [ "${!i}" = "--list" -o "${!i}" = "-l" ]; then
        echo -e "List of test categories to call with -c <category> : \n    commands\n    cases\n    loops\n    if\n    pipes\n    functions\n    builtins\n    arithmetic\n    assignments"
    fi;
    if [ "${!i}" = "--timeout" -o "${!i}" = "-t" ]; then
        x=$(($i + 1))
        timeout=${!x}
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

            if [ "${!j}" = "loops" ]; then
                loops=1
            fi;

            if [ "${!j}" = "cases" ]; then
                cases=1
            fi;

            if [ "${!j}" = "commands" ]; then
                commands=1
            fi;

            if [ "${!j}" = "if" ]; then
                ifs=1
            fi;

            if [ "${!j}" = "functions" ]; then
                func=1
            fi;

            if [ "${!j}" = "pipes" ]; then
                pipes=1
            fi;

            if [ "${!j}" = "builtins" ]; then
                builtins=1
            fi;

            if [ "${!j}" = "assignments" ]; then
                assignments=1
            fi;

            if [ "${!j}" = "arithmetic" ]; then
                arithmetic=1
            fi;
        done
    fi;
done

if [ $# -eq 0 ]; then
    all=1
fi;

sanity_string=""
timeout_string=""

source "env/bin/activate"

sanity_string="--valgrind=$sanity"

timeout_string="--timeout=$timeout"

if [ $all -eq 1 ]; then
    echo -e "${PURPLE}Starting all tests.\n${NC}"
    pytest "../test_suite/test_files/" $sanity_string $timeout_string
    echo
fi;

if [ $commands -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on simple commands.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_commands.yml" $sanity_string
    echo
fi;


if [ $loops -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on loops.\n${NC}"
    pytest -s $timeout_string "../test_suite/test_files/test_loops.yml" $sanity_string
    echo
fi;

if [ $cases -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on case statements.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_cases.yml" $sanity_string
    echo
fi;

if [ $ifs -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on if statements.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_if.yml" $sanity_string
    echo
fi;

if [ $func -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on functions.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_func.yml" $sanity_string
    echo
fi;

if [ $pipes -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on pipes.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_pipes.yml" $sanity_string
    echo
fi;

if [ $builtins -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on builtins.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_builtins.yml" $sanity_string
    echo
fi;

if [ $assignments -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on assigments.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_assignment.yml" $sanity_string
    echo
fi;


if [ $arithmetic -eq 1 ]; then
    echo -e "${PURPLE}Starting tests on arithmetic.\n${NC}"
    pytest $timeout_string "../test_suite/test_files/test_arithmetic.yml" $sanity_string
    echo
fi;



deactivate

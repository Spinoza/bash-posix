sanity=0
commands=0
cases=0
loops=0
ifs=0
pipes=0
func=0
all=0
timeout=0

for (( i=1; i<=$#; i++))
do

    if [ "${!i}" = "--list" -o "${!i}" = "-l" ]; then
        echo "List of test categories to call with -c <category> : \n,
            'simple_commands','cases','loops','if','pipes','functions'"
    fi;
    if [ "${!i}" = "--timeout" -o "${!i}" = "-t" ]; then
        x= $(($i + 1))
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

            if [ "${!j}" = "simple_commands" ]; then
                commands=1
            fi;
        done
    fi;
done

sanity_string=""
timeout_string=""
echo "here"
source ../../build/env/bin/activate
echo "after"
if [ $sanity -eq 1 ]; then
    sanity_string="--valgrind=1"
fi;

if [ $timeout -gt 0 ]; then
    echo $timeout
    timeout_string="--timeout=$timeout"
fi;

if [ $all -eq 1 ]; then
    pytest "../test_suite/"
fi;

if [ $commands -eq 1 ]; then
    echo "here"
    pytest $timeout_string "../test_suite/test_files/test_simple_commands.yml" $sanity_string
fi;


if [ $loops -eq 1 ]; then
    pytest $timeout_string "../tests/test_suite/test_files/test_loops.yml" $sanity_string
fi;

if [ $cases -eq 1 ]; then
    pytest $timeout_string "../tests/test_suite/test_files/test_cases.yml" $sanity_string
fi;


deactivate

source ./env/bin/activate

pytest "../tests/test_suite/test_files/" "--check=1"

deactivate
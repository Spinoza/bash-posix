cd ../tests/test_scripts
source env/bin/activate
pytest "../test_suite/test_files/" "--timeout=5" "--valgrind=0"
deactivate

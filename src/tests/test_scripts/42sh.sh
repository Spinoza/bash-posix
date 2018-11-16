echo "Testing prints of our 42sh"

echo -e "\ncommand : word\n"
./42sh word
echo -e "\ncommand : ls\n"
./42sh "ls"
echo -e "\ncommand : pwd\n"
./42sh "pwd"
echo -e "\ncommand : ls -la\n"
./42sh "ls" -la
echo -e "\ncommand : pwd && pwd\n"
./42sh "pwd" && "pwd"
echo -e "\ncommand : pwd || pwd\n"
./42sh "pwd" "||" "pwd"
echo -e "\ncommand : ls -la || pwd\n"
./42sh "ls" "-la" "||" "pwd"
echo -e "\ncommand : ls -la  || pwd\n"
./42sh  "ls" "-la" "||" "pwd"
echo -e "\ncommand : ls -la && pwd:\n"
./42sh "ls" "-la" "&&" "pwd"

echo -e "\ncommand : if pwd; then ls; else echo willneverprint fi\n"
./42sh "if" "pwd;" "then" "ls;" "else" "echo" "willneverprint;" "fi"

echo -e "\ncommand: while bonsoir || bonnenuit; do thx; if matin; then butwtf; elif jour; then butearly; else verymuch; fi done"
./42sh "while" "bonsoir" "||" "bonnenuit;" "do" "thx;" "if" "matin;" "then" "butwtf;" "elif" "jour;" "then" "butearly;" "else" "verymuch;" "fi" "done"

echo -e "\ncommand : while false; do echo neverprint; done; pwd;\n"
./42sh "while" "false;" "do" "echo" "neverprint;" "done;" "pwd;"

echo -e "\ncommand : until true; do pwd; done;\n"
./42sh "until" "true;" "do" "pwd;" "done"

echo -e "\ncommand : until true; do word=2; done; pwd;\n"
./42sh "until" "pwd;" "do" "word=2;" "done" "pwd;"

#!/bin/bash

MINISHELL="./minishell"
OUTPUT_FILE="minishell_output.txt"
EXPECTED_OUTPUT_FILE="bash_output.txt"

# --- Colors for output ---
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Check if minishell executable exists
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: minishell executable not found. Please run 'make'.${NC}"
    exit 1
fi

# Function to run a test and compare output
run_test() {
    TEST_NAME=$1
    COMMANDS=$2
    
    echo "--- Running test: $TEST_NAME ---"
    
    # Run minishell with the given commands and capture output, stripping the prompt
    (echo -e "$COMMANDS\nexit") | $MINISHELL | sed '/^minishell\$/d' > "$OUTPUT_FILE"

    # Run the same commands in a subshell for comparison
    (echo -e "$COMMANDS") | bash | sed '/^$/d' > "$EXPECTED_OUTPUT_FILE"

    # Compare the output
    if diff -w "$OUTPUT_FILE" "$EXPECTED_OUTPUT_FILE" >/dev/null; then
        echo -e "${GREEN}SUCCESS${NC}"
    else
        echo -e "${RED}FAILURE${NC}"
        echo "Command: $COMMANDS"
        echo "--- Minishell Output ---"
        cat "$OUTPUT_FILE"
        echo "--- Expected Output (from Bash) ---"
        cat "$EXPECTED_OUTPUT_FILE"
        echo "--------------------------"
        echo ""
    fi
}

# --- Specific Test Functions for better debugging ---
run_builtin_test() {
    TEST_NAME=$1
    COMMAND=$2
    
    echo "--- Running test: $TEST_NAME ---"
    
    # Run the command and capture output, stripping the prompt and empty lines
    (echo -e "$COMMAND\nexit") | $MINISHELL | sed '/^minishell\$/d' | sed '/^$/d' > "$OUTPUT_FILE"
    
    # Run the command in bash and capture output
    (echo -e "$COMMAND") | bash | sed '/^$/d' > "$EXPECTED_OUTPUT_FILE"
    
    # Compare the output
    if diff -w "$OUTPUT_FILE" "$EXPECTED_OUTPUT_FILE" >/dev/null; then
        echo -e "${GREEN}SUCCESS${NC}"
    else
        echo -e "${RED}FAILURE${NC}"
        echo "Command: $COMMAND"
        echo "--- Minishell Output ---"
        cat "$OUTPUT_FILE"
        echo "--- Expected Output (from Bash) ---"
        cat "$EXPECTED_OUTPUT_FILE"
        echo "--------------------------"
        echo ""
    fi
}

# --- Mandatory Part Tests ---

# 1. Simple command execution
run_test "Simple command" "ls"

# 2. Command with arguments
run_test "Echo with arguments" "echo hello world"

# 3. Relative path execution (if `bin` directory exists)
# Note: This test assumes you have a `bin` folder with `ls` in it for your project
run_test "Relative path" "bin/ls"

# 4. Absolute path execution
run_test "Absolute path" "/bin/echo minishell"

# 5. Redirection tests
echo "test for redirect" | run_test "Output redirection >" "cat > temp_redir_out"
run_test "Output redirection with file" "cat temp_redir_out" "test for redirect"

# >> (append output)
echo "first line" | run_test "Append redirection >> (1st line)" "cat >> temp_redir_app"
echo "second line" | run_test "Append redirection >> (2nd line)" "cat >> temp_redir_app"
run_test "Append redirection check" "cat temp_redir_app" "first line\nsecond line"

# < (redirect input)
echo "this is a file" > temp_file
run_test "Input redirection <" "cat < temp_file" "this is a file"

# 6. Built-in command tests
# echo -n
run_builtin_test "Built-in: echo -n" "echo -n hello"

# cd and pwd
run_builtin_test "Built-in: cd to a directory" "cd /tmp; pwd"
run_builtin_test "Built-in: cd to home" "cd; pwd"

# env
run_test "Built-in: env" "env"

# 7. Pipe tests
run_test "Simple pipe" "echo hello | cat"
run_test "Multiple pipes" "echo a b c | tr ' ' '\n' | sort | uniq"

# 8. Environment variable expansion
export TEST_VAR="hello"
run_test "Variable expansion" "echo \$TEST_VAR"
run_test "Exit status \$?" "ls nonexistent_file\necho \$?"

# 9. Quote tests
run_test "Single quotes" "echo 'hello \$USER'"
run_test "Double quotes" "echo \"hello \$USER\""

# 10. Heredoc test
echo "Heredoc test"
printf "line 1\nline 2\nEOF\n" | run_test "Heredoc with cat" "cat << EOF"

# Clean up temporary files
rm -f "$OUTPUT_FILE" "$EXPECTED_OUTPUT_FILE" temp_redir_out temp_redir_app temp_file

echo "-------------------"
echo "All tests finished."
#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

PASSED=0
FAILED=0
VERBOSE=0 # Initialize verbosity flag to 0 (off)

# Process command-line options
while getopts "v" opt; do
  case "$opt" in
    v)
      VERBOSE=1 # Set verbosity flag to 1 (on) if -v is passed
      ;;
    \?)
      echo "Invalid option: -$OPTARG"
      echo "Usage: $0 [-v]"
      exit 1
      ;;
  esac
done
shift $((OPTIND-1)) # Remove parsed options from positional parameters


PARACL_PATHS=(
    "./../../build/ParaCL"
    "./../../build-release/ParaCL"
    "./../../build-debug/ParaCL"
    "./../../build/release/ParaCL"
    "./../../build/debug/ParaCL"
)

PARACL_EXE=""
for path in "${PARACL_PATHS[@]}"; do
    if [ -f "$path" ]; then
        PARACL_EXE="$path"
        break
    fi
done

if [ -z "$PARACL_EXE" ]; then
    echo -e "${RED}Ошибка: исполняемый файл ParaCL не найден в следующих путях:${NC}"
    printf '  %s\n' "${PARACL_PATHS[@]}"
    echo "Вероятно, проект не собран"
    exit 1
fi

EXAMPLES_DIR="../../examples"
ANSWERS_DIR="answers"

echo "Найден исполняемый файл: $PARACL_EXE"
echo "Running END-TO-END tests"

if [ ! -d "$EXAMPLES_DIR" ]; then
    echo -e "${RED}Тесты не найдены по пути $EXAMPLES_DIR${NC}"
    exit 1
fi

for TEST_FILE in "$EXAMPLES_DIR"/*; do
    if [ -f "$TEST_FILE" ]; then
        base_name=$(basename "$TEST_FILE")
        expected_file="$ANSWERS_DIR/$base_name.out"

        if [ ! -f "$expected_file" ]; then
            echo -e "${RED}Ошибка: эталонный файл $base_name.out не найден${NC}"
            ((FAILED++))
            continue
        fi

        output=$("$PARACL_EXE" "$TEST_FILE" 2>&1)
        exit_code=$?

        if [ $exit_code -ne 0 ]; then
            echo -e "${RED}>>> Провален: $base_name (код ошибки $exit_code)${NC}"
            if [ $VERBOSE -eq 1 ]; then
                echo -e "${RED}Код выхода: $exit_code${NC}"
            fi
            ((FAILED++))
        elif diff -q <(echo "$output") "$expected_file" >/dev/null; then
            echo -e "${GREEN}Пройден: $base_name${NC}"
            ((PASSED++))
        else
            echo -e "${RED}>>> Провален: $base_name${NC}"
            if [ $VERBOSE -eq 1 ]; then
                echo -e "${RED}Отличия:${NC}"
                diff -y <(echo "$output") "$expected_file"
            fi
            ((FAILED++))
        fi
    fi
done

echo
echo "Результаты:"
echo -e "${GREEN}Пройдено: $PASSED${NC}"
echo -e "${RED}Провалено: $FAILED${NC}"

exit $((FAILED > 0 ? 1 : 0))

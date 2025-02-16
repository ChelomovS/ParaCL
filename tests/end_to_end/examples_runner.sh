#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

PASSED=0
FAILED=0

PARACL_EXE="./../../build/ParaCL"

EXAMPLES_DIR="../../examples"

ANSWERS_DIR="answers"
echo "Running END-TO-END tests"
if [ ! -f "$PARACL_EXE" ]; then
  echo "Исполняемый файл ParaCL не найден по пути $PARACL_EXE, вероятно, проект не собран!"
  exit 1
fi

if [ ! -d "$EXAMPLES_DIR" ]; then
  echo "Тесты не найдены по пути $EXAMPLES_DIR"
  exit 1
fi

for TEST_FILE in "$EXAMPLES_DIR"/*; do
  if [ -f "$TEST_FILE" ]; then
    base_name=$(basename "$TEST_FILE")
    expected_file="$ANSWERS_DIR/$base_name.out"
    
    if [ ! -f "$expected_file" ]; then
      echo -e "${RED}Ошибка: эталонный файл $base_name.out не найден${NC}"
      ((++failed))
      continue
    fi

    output=$("$PARACL_EXE" "$TEST_FILE" 2>&1)
    exit_code=$?

    if [ $exit_code -ne 0 ]; then
      echo -e "${RED}>>> Провален: $base_name (код ошибки $exit_code)${NC}"
      ((++failed))
    elif diff -q <(echo "$output") "$expected_file" >/dev/null; then
      echo -e "${GREEN}Пройден: $base_name${NC}"
      ((++passed))
    else
      echo -e "${RED}>>> Провален: $base_name${NC}"
      ((++failed))
    fi
  fi
done

echo
echo "Результаты:"
echo -e "${GREEN}Пройдено: $passed${NC}"
echo -e "${RED}Провалено: $failed${NC}"

exit $((failed > 0 ? 1 : 0))
#!/bin/bash

PARACL_EXE="./../../build/ParaCL"

EXAMPLES_DIR="../../examples"

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
    "$PARACL_EXE" "$TEST_FILE"
    TEST_FILE=$(basename "$TEST_FILE")
    echo "Тест исполнен: $TEST_FILE"
    printf '=%.0s' {1..100} 
echo
  fi
done

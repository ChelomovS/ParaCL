#!/usr/bin/env python3

import os
import sys
import subprocess
import argparse

GREEN       = '\033[0;32m'
RED         = '\033[0;31m'
CLEAR_COLOR = '\033[0m'

PASSED = 0
FAILED = 0

def find_paracl_executable(paths):
    """Searches for the ParaCL executable in the given paths."""
    for path in paths:
        if os.path.isfile(path):
            return path
    return None

def run_test(paracl_exe, test_file, answers_dir, verbose):
    """Runs a single test case."""
    global PASSED, FAILED
    base_name = os.path.basename(test_file)
    expected_file = os.path.join(answers_dir, f"{base_name}.out")

    if not os.path.isfile(expected_file):
        print(f"{RED}Ошибка: эталонный файл {base_name}.out не найден{CLEAR_COLOR}")
        FAILED += 1
        return

    try:
        result = subprocess.run([paracl_exe, test_file], capture_output=True, text=True, check=False)
        output = result.stdout
        exit_code = result.returncode

        if exit_code != 0:
            print(f"{RED}>>> Провален: {base_name} (код ошибки {exit_code}){CLEAR_COLOR}")
            if verbose:
                print(f"{RED}Код выхода: {exit_code}{CLEAR_COLOR}")
            FAILED += 1
        else:
            try:
                with open(expected_file, 'r') as f:
                    expected_output = f.read()
                if output.strip() == expected_output.strip():
                    print(f"{GREEN}Пройден: {base_name}{CLEAR_COLOR}")
                    PASSED += 1
                else:
                    print(f"{RED}>>> Провален: {base_name}{CLEAR_COLOR}")
                    if verbose:
                        print(f"{RED}Отличия:{CLEAR_COLOR}")
                        process = subprocess.run(['diff', '-y', '--from-file', expected_file, '-', ], input=output, text=True, capture_output=True)
                        print(process.stdout)
                    FAILED += 1
            except FileNotFoundError:
                print(f"{RED}Ошибка: не удалось открыть эталонный файл {expected_file}{CLEAR_COLOR}")
                FAILED += 1

    except FileNotFoundError:
        print(f"{RED}Ошибка: не удалось запустить {paracl_exe}{CLEAR_COLOR}")
        FAILED += 1
    except Exception as e:
        print(f"{RED}Произошла ошибка при выполнении теста {base_name}: {e}{CLEAR_COLOR}")
        FAILED += 1

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Runs end-to-end tests for ParaCL.")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose output.")
    args = parser.parse_args()

    VERBOSE = args.verbose

    paracl_paths = [
        "./../../build/ParaCL",
        "./../../build-release/ParaCL",
        "./../../build-debug/ParaCL",
        "./../../build/release/ParaCL",
        "./../../build/debug/ParaCL",
    ]

    paracl_exe = find_paracl_executable(paracl_paths)

    if not paracl_exe:
        print(f"{RED}Ошибка: исполняемый файл ParaCL не найден в следующих путях:{CLEAR_COLOR}")
        for path in paracl_paths:
            print(f"  {path}")
        print("Вероятно, проект не собран")
        sys.exit(1)

    examples_dir = "../../examples"
    answers_dir = "answers"

    print(f"Найден исполняемый файл: {paracl_exe}")
    print("Running END-TO-END tests")

    if not os.path.isdir(examples_dir):
        print(f"{RED}Тесты не найдены по пути {examples_dir}{CLEAR_COLOR}")
        sys.exit(1)

    for test_file in sorted(os.listdir(examples_dir)):
        test_file_path = os.path.join(examples_dir, test_file)
        if os.path.isfile(test_file_path):
            run_test(paracl_exe, test_file_path, answers_dir, VERBOSE)

    print()
    print("Результаты:")
    print(f"{GREEN}Пройдено: {PASSED}{CLEAR_COLOR}")
    print(f"{RED}Провалено: {FAILED}{CLEAR_COLOR}")

    sys.exit(1 if FAILED > 0 else 0)

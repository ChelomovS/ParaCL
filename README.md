# ParaCL

## Установка
```sh
git clone git@github.com:ChelomovS/ParaCL.git
cd ParaCL
```

## Сборка:
```sh
cmake -S . -B build [-DGRAPHVIZ=ON]
cmake --build build
```

## Использование 
```sh
./build/ParaCL <input_file>
```

## Запуск unit-тестов:
```sh
./build/tests/unit_tests
```

## Запуск end-to-end тестов:
```sh
cd tests/end-to-end
./examples_runner.sh 
```

# ParaCL

## Установка
```sh
git clone git@github.com:ChelomovS/ParaCL.git
cd ParaCL
```

Для сборки release версии:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Для сборки debug версии:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Для генерации изображения AST:
```sh
cmake -B build -DANALYSE=ON
```

## Использование 
```sh
cd build 
./ParaCL <input_file>
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

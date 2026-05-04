# lab5 — Графы

## Сборка и запуск
1. Сгенерировать билд:
```powershell
cd 2sem/lab5
cmake -S . -B build -G Ninja -DBUILD_TESTS=ON -DBUILD_BENCH=ON
```
Если у вас нет Ninja, опустите `-G Ninja` — CMake сам выберет генератор.

2. Собрать проект:
```powershell
cmake --build build --config Release --parallel
```

3. Запустить приложение (CLI):
```powershell
build\lab5_app.exe --input path\to\graph.txt --start 0 --out-result result.txt --out-graph dump.txt
```

4. Запустить unit‑тесты (всё в папке `build`):
```powershell
cd build
ctest -C Release -V
# или запустить исполняемый тест напрямую:
.\test_dijkstra.exe --gtest_filter="Dijkstra.SmallGraph"
```

5. Запустить бенчмарк и сохранить CSV результатов:
```powershell
.\build\lab5_bench.exe --runs 5 --warmups 1 --seed 12345 > bench.csv
```


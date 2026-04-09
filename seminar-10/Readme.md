# Seminar 10 - Turn-Based DnD Game Loop

Учебный проект по runtime-полиморфизму и двойной диспетчеризации в C++17.
Игра моделирует пошаговый цикл `step -> reaction -> repeat`, где сущности
обновляются, реагируют на столкновения без ручного приведения типов и затем
показываются как цветная ASCII-сцена в консоли.

## Структура

```
seminar-10/
├── CMakeLists.txt
├── Readme.md
├── inc/
│   ├── enemy.hpp
│   ├── console_view.hpp
│   ├── game_entity.hpp
│   ├── game_world.hpp
│   ├── player.hpp
│   └── power_up.hpp
├── src/
│   ├── console_view.cpp
│   ├── enemy.cpp
│   ├── game_entity.cpp
│   ├── game_world.cpp
│   ├── main.cpp
│   ├── player.cpp
│   └── power_up.cpp
└── tests/
    └── test_game_entities.cpp
```

## Возможности

- Базовый класс `GameEntity` с виртуальными `update`, `render`, `onCollision`
- Производные сущности `Player`, `Enemy`, `PowerUp`
- Двойная диспетчеризация для обработки столкновений без `dynamic_cast`
- Координатор `GameWorld`, возвращающий `TurnResult` после каждого хода
- Цветная ASCII-визуализация поля с ANSI-последовательностями
- Демонстрационный игровой цикл на 4 хода
- Optional unit-тесты на Catch2

## Сборка

```bash
cmake -S seminar-10 -B seminar-10/build
cmake --build seminar-10/build
./seminar-10/build/turn_based_dnd_game
```

## Тесты

```bash
cmake -S seminar-10 -B seminar-10/build -DBUILD_TESTS=ON
cmake --build seminar-10/build --target run-tests
```

# Seminar 7 - Dataset Analysis

Command-line utility for **statistical analysis of tabular data**. Each record
has the form `(id, category, value1, value2)`. The tool demonstrates modern
C++17 containers, simple statistics, and reusable library design with optional
Catch2-based tests.

## Features

- **Random dataset generation**: Creates synthetic datasets with configurable
  size and multiple categories
- **Filtering**:
  - by **category** (`filterByCategory`)
  - by **value range** on `value1` (`filterByRange`)
- **Column extraction**: `extractColumn` for `value1` and `value2`
- **Statistics**:
  - mean, median, standard deviation for numeric vectors
  - grouped statistics by category (mean, min, max)
- **Reporting**: Simple console report per category
- **Modular design**:
  - core logic in a library used by the app and tests
  - main program focuses on orchestration and I/O

## Project Structure

```
seminar-7/
├── CMakeLists.txt                 # CMake build configuration
├── Readme.md                      # This file
├── анализ набора данных.md        # Original task description (RU)
├── inc/
│   └── dataset_utils.hpp          # Declarations of dataset and statistics utils
├── src/
│   ├── main.cpp                   # Main application entry point
│   └── dataset_utils.cpp          # Implementation of dataset/statistics logic
└── tests/
    └── test_dataset_utils.cpp     # Unit tests using Catch2
```

## Requirements

- **CMake** ≥ 3.5
- **C++17** compatible compiler (GCC, Clang, MSVC)
- **Git** (for fetching Catch2 when tests are enabled)

## Build

From the `seminar-7` directory:

### Configure

```bash
cmake -S . -B build
```

### Compile

```bash
cmake --build build
```

This produces the executable:

- `build/dataset_analysis`

## Run

From the repository root or `seminar-7`:

```bash
./build/dataset_analysis [N]
```

- **`N`** (optional): number of generated records (default: `1000`)

### Program behaviour

1. Generates `N` random records:
   - `id` — integer
   - `category` — one of `"A"`, `"B"`, `"C"`, `"D"`, `"E"`
   - `value1`, `value2` — random doubles in `[0, 100]`
2. Computes and prints **grouped statistics** by category:
   - mean, min, max of `value1`
3. Applies and reports:
   - filter by category `"A"`
   - filter by `value1` range `[20, 80]`

Output example (format may vary slightly):

```text
Overall statistics by category:
Category    Mean    Min     Max
A           49.32   0.13    99.88
B           51.27   0.45    99.42
...
```

## Library API

Namespace: `dataset`

```cpp
using Record  = std::tuple<int, std::string, double, double>;
using Dataset = std::vector<Record>;
```

- **Data generation & filtering**
  - `Dataset generateRandomData(int n);`
  - `Dataset filterByCategory(const Dataset& data, const std::string& category);`
  - `Dataset filterByRange(const Dataset& data, double minVal, double maxVal);`
    - filters by `value1` in the inclusive range `[minVal, maxVal]`

- **Statistics helpers**
  - `std::vector<double> extractColumn(const Dataset& data, int columnIndex);`
    - supported `columnIndex`: `2` (value1), `3` (value2)
  - `double calculateMean(const std::vector<double>& values);`
  - `double calculateMedian(std::vector<double> values);`
  - `double calculateStdDev(const std::vector<double>& values, double mean);`

- **Grouped statistics**
  - `std::tuple<double, double, double> calculateStats(const std::vector<double>& values);`
    - returns `(mean, min, max)`
  - `std::map<std::string, std::tuple<double, double, double>> groupByCategory(const Dataset& data);`
    - groups by `category` and aggregates statistics for `value1`
  - `void printReport(const std::map<std::string, std::tuple<double, double, double>>& stats);`

## Unit Testing

Tests are **optional** and controlled via CMake option `BUILD_TESTS`.

### Configure with tests

```bash
cmake -S . -B build -DBUILD_TESTS=ON
```

### Build and run tests

```bash
cmake --build build --target run-tests
```

This will:

- build the test executable `unit_tests`
- run all Catch2 tests

You can also use `ctest` from the `build` directory:

```bash
cd build
ctest
```

Registered test name: **`DatasetUtilsTests`**.

## Architecture

The project follows a **library-first** architecture:

- **`dataset_utils_lib`**:
  - owns all core logic for data generation, filtering, statistics, and reporting
  - reusable in other tools or benchmarks
- **Main application**:
  - uses the library to generate a random dataset
  - demonstrates filtering and grouped statistics
  - prints a simple human-readable report
- **Test suite**:
  - links against the same library
  - validates key behaviours: filtering, statistics, grouping, and column extraction

This separation keeps the logic testable and encourages reuse in other
experiments or seminars.


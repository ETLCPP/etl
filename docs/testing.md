# Testing ETL

This document describes how to build and run the ETL test suite locally,
inside Dev Containers, and in CI.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Running Tests Locally (`test/run-tests.sh`)](#running-tests-locally)
3. [Syntax Checks (`test/run-syntax-checks.sh`)](#syntax-checks)
4. [Cross-Architecture Testing (`.devcontainer/run-tests.sh`)](#cross-architecture-testing)
5. [Dev Containers for Native Compilers](#dev-containers-for-native-compilers)
6. [CMake Options Reference](#cmake-options-reference)
7. [CI Checks (GitHub Actions)](#ci-checks-github-actions)
8. [Appveyor (Windows / MSVC)](#appveyor-windows--msvc)
9. [Code Coverage](#code-coverage)
10. [Generator Tests (`scripts/generator_test.py`)](#generator-tests)

---

## Prerequisites

* **CMake** ≥ 3.10
* **GCC** and/or **Clang** (any version supported by the project)
* **Make** or **Ninja** (build backend)
* **Docker** (only needed for cross-architecture testing via `.devcontainer/run-tests.sh`)
* **QEMU user-mode** (installed automatically inside the cross-arch Docker images)

The project is header-only, so there is no library to compile – the build
step compiles the test binary `etl_tests` which links against a bundled copy
of **UnitTest++**.

---

## Running Tests Locally

The main entry point for local testing is **`test/run-tests.sh`**.  It
iterates over a matrix of compiler / configuration combinations, creates a
temporary `build-make` directory for each one, runs CMake + Make + CTest,
and reports coloured pass/fail output (also appended to `log.txt`).

### Synopsis

```bash
cd test
./run-tests.sh <C++ Standard> [Optimisation] [Threads] [Sanitizer] [Compiler] [Verbose]
```

| Argument | Values | Default |
|---|---|---|
| C++ Standard | `11`, `14`, `17`, `20`, `23`, or `all` | *(required)* |
| Optimisation | `0`, `1`, `2`, `3` | `0` |
| Threads | any positive integer | `4` |
| Sanitizer | `s` (enable) / `n` (disable) | `n` (disabled) |
| Compiler | `gcc`, `clang` | all compilers |
| Verbose | `v` (enable) / `n` (disable) | `n` (disabled) |

### Examples

```bash
# Run all C++17 tests with GCC only, optimisation -O0, 8 threads
./run-tests.sh 17 0 8 n gcc

# Run every standard with both compilers, sanitizers enabled, verbose
./run-tests.sh all 0 4 s "" v
```

### What the script does

For every selected C++ standard the script loops over a built-in list of
configurations (STL / No STL / Force C++03 / Non-virtual messages / …) for
each selected compiler.  For every combination it:

1. Creates a fresh `build-make` directory inside the configuration's source
   subdirectory.
2. Invokes `cmake` with the appropriate `-D` flags (see
   [CMake Options Reference](#cmake-options-reference)).
3. Builds via `cmake --build .` (parallel level controlled by
   `CMAKE_BUILD_PARALLEL_LEVEL`).
4. Runs `ctest -V`.
5. Reports success or failure and removes the build directory.

The script exits immediately on the first compilation or test failure.

### Test configurations exercised

| Compiler | Configuration |
|---|---|
| GCC | STL |
| GCC | STL – Non-virtual messages |
| GCC | STL – Force C++03 |
| GCC | No STL |
| GCC | No STL – Force C++03 |
| GCC | No STL – Builtin mem functions |
| Clang | STL |
| Clang | STL – Force C++03 |
| Clang | No STL |
| Clang | No STL – Force C++03 |
| Clang | No STL – Builtin mem functions |
| GCC / Clang | Initializer list test |
| GCC / Clang | Error macros – log_errors, exceptions, log_errors_and_exceptions, assert_function |

---

## Syntax Checks

The script **`test/run-syntax-checks.sh`** performs compilation-only syntax
checks across multiple C++ standards and configurations.  Unlike
`run-tests.sh`, it **does not run the test binary** – it only verifies that
the code compiles successfully.  This is useful for quickly validating that
header changes do not introduce compilation errors across the supported
standard/configuration matrix.

### Synopsis

```bash
cd test
./run-syntax-checks.sh <C++ Standard> [Threads] [Compiler]
```

| Argument | Values | Default |
|---|---|---|
| C++ Standard | `03`, `11`, `14`, `17`, `20`, `23`, or `a` (all) | *(required)* |
| Threads | any positive integer | `4` |
| Compiler | `gcc`, `clang` | all compilers |

### Examples

```bash
# Check C++17 syntax with GCC only, using 8 threads
./run-syntax-checks.sh 17 8 gcc

# Check all standards with both compilers
./run-syntax-checks.sh a
```

### What the script does

The script operates from the `test/syntax_check` directory and iterates over
the selected C++ standard(s).  For each standard and compiler combination it:

1. Creates fresh build directories (`bgcc` / `bclang`).
2. Invokes `cmake` with the appropriate `-D` flags for the configuration.
3. Builds via `cmake --build`.
4. Reports compilation success or failure (logged to `log.txt`).

The script exits immediately on the first compilation failure.

### Configurations checked per standard

For each C++ standard the following configurations are compiled:

| Compiler | Configuration |
|---|---|
| GCC | STL |
| GCC | No STL |
| GCC | STL – Built-in traits |
| GCC | No STL – Built-in traits |
| Clang | STL |
| Clang | No STL |
| Clang | STL – Built-in traits |
| Clang | No STL – Built-in traits |

---

## Cross-Architecture Testing

**`.devcontainer/run-tests.sh`** builds and runs the test suite for
non-x86_64 architectures using Docker and QEMU user-mode emulation.  It is
designed to be run **from the project root**.

### Supported architectures

| Argument | Target | Endianness | QEMU binary |
|---|---|---|---|
| `armhf` | ARM hard-float (32-bit) | Little | `qemu-arm-static` |
| `i386` | x86 32-bit | Little | `qemu-i386-static` |
| `powerpc` | PowerPC 32-bit | Big | `qemu-ppc` |
| `riscv64` | RISC-V 64-bit | Little | `qemu-riscv64-static` |
| `s390x` | IBM Z (64-bit) | Big | `qemu-s390x-static` |

### Synopsis

```bash
# From the project root
.devcontainer/run-tests.sh <architecture>
```

### How it works

The script has two phases controlled by a second (internal) argument:

1. **Outside the container** (no second argument):
   * Builds a Docker image from `.devcontainer/<arch>/Dockerfile`.
   * Starts a container, bind-mounting the project at `/workspaces/etl`.
   * Re-invokes itself *inside* the container with the `inside_container`
     flag.

2. **Inside the container** (`inside_container`):
   * Creates `build-<arch>` and runs CMake with the appropriate cross-
     compilation toolchain file
     (`.devcontainer/<arch>/toolchain-<arch>.cmake`).
   * Builds with `cmake --build .` using all available cores.
   * Runs the test suite via `ctest --output-on-failure`.

The toolchain files set `CMAKE_CROSSCOMPILING_EMULATOR` so that CTest can
run the binary transparently through QEMU.

### Example

```bash
# Build & run the armhf test suite
.devcontainer/run-tests.sh armhf
```

The cross-arch containers build with the following fixed settings:

* C++23, No STL, sanitizer off, optimisation -O0.

---

## Dev Containers for Native Compilers

The `.devcontainer/` directory also provides Dev Container definitions for a
wide range of **native** (x86_64) compiler versions.  These are intended for
use with **VS Code Dev Containers** or **GitHub Codespaces**.

| Directory | Compiler |
|---|---|
| `gcc09` – `gcc15` | GCC 9 through 15 |
| `clang7` – `clang21` | Clang 7 through 21 |

Each subdirectory contains a `devcontainer.json` that references the shared
`Dockerfile` (`.devcontainer/Dockerfile`) and passes the appropriate base
Docker image via the `BASE_IMAGE_NAME` build argument (e.g. `gcc:15`).

The default Dev Container (`.devcontainer/devcontainer.json`) uses the
Microsoft C++ dev-container base image.

To use one of these containers:

1. Open the repository in VS Code.
2. **Ctrl+Shift+P → Dev Containers: Reopen in Container** and select the
   desired configuration (e.g. *Gcc 15*).
3. Use `test/run-tests.sh` inside the container as described above.

---

## CMake Options Reference

When invoking CMake for the test suite (source directory is `test/`), the
following `-D` options control the build:

| Option | Type | Description |
|---|---|---|
| `BUILD_TESTS` | `BOOL` | Must be `ON` to compile the test binary. |
| `NO_STL` | `BOOL` | Build without the C++ Standard Library. |
| `ETL_CXX_STANDARD` | `STRING` | C++ standard: `11`, `14`, `17`, `20`, `23`. |
| `ETL_OPTIMISATION` | `STRING` | Compiler optimisation flag, e.g. `-O0`. |
| `ETL_ENABLE_SANITIZER` | `BOOL` | Enable address / undefined-behaviour sanitizers. |
| `ETL_USE_TYPE_TRAITS_BUILTINS` | `BOOL` | Use compiler built-in type traits. |
| `ETL_USER_DEFINED_TYPE_TRAITS` | `BOOL` | Use user-defined type traits. |
| `ETL_FORCE_TEST_CPP03_IMPLEMENTATION` | `BOOL` | Force the C++03 code paths even on newer standards. |
| `ETL_MESSAGES_ARE_NOT_VIRTUAL` | `BOOL` | Use non-virtual message types. |
| `ETL_USE_BUILTIN_MEM_FUNCTIONS` | `BOOL` | Use built-in memory functions in No-STL mode. |
| `CMAKE_TOOLCHAIN_FILE` | `PATH` | Toolchain file for cross-compilation. |

### Minimal manual build example

```bash
cd test
mkdir build && cd build
cmake -DBUILD_TESTS=ON -DNO_STL=OFF -DETL_CXX_STANDARD=20 ..
cmake --build . -j$(nproc)
ctest -V
```

---

## CI Checks (GitHub Actions)

Every push or pull request to `master`, `development`, or `pull-request/*`
branches triggers a comprehensive set of GitHub Actions workflows defined in
`.github/workflows/`.

### Workflow matrix

| Workflow file | Compiler | Standard | Notes |
|---|---|---|---|
| `gcc-c++11.yml` | GCC | C++11 | STL, No STL, Force C++03 |
| `gcc-c++14.yml` | GCC | C++14 | STL, No STL, Force C++03 |
| `gcc-c++17.yml` | GCC | C++17 | STL, No STL, Force C++03 |
| `gcc-c++20.yml` | GCC | C++20 | STL, No STL, Force C++03 |
| `gcc-c++23.yml` | GCC | C++23 | STL, No STL, Force C++03 |
| `clang-c++11.yml` | Clang | C++11 | STL, No STL, Force C++03 |
| `clang-c++14.yml` | Clang | C++14 | STL, No STL, Force C++03 |
| `clang-c++17.yml` | Clang | C++17 | STL, No STL, Force C++03 |
| `clang-c++20.yml` | Clang | C++20 | STL, No STL, Force C++03 |
| `clang-c++23.yml` | Clang | C++23 | STL, No STL, Force C++03 |
| `gcc-syntax-checks.yml` | GCC | C++03 – C++23 | Compilation-only syntax checks (no tests run) |
| `clang-syntax-checks.yml` | Clang | C++03 – C++23 | Compilation-only syntax checks (no tests run) |
| `msvc.yml` | MSVC 2022 | C++17 | Windows, STL & No STL |
| `gcc-c++23-armhf.yml` | GCC cross | C++23 | armhf via QEMU |
| `gcc-c++23-i386.yml` | GCC cross | C++23 | i386 via QEMU |
| `gcc-c++23-powerpc.yml` | GCC cross | C++23 | powerpc via QEMU |
| `gcc-c++23-riscv64.yml` | GCC cross | C++23 | RISC-V 64 via QEMU |
| `gcc-c++23-s390x.yml` | GCC cross | C++23 | s390x via QEMU |
| `coverage.yml` | GCC | — | Generates lcov coverage report, deploys to GitHub Pages |
| `generator.yml` | — | — | Runs the code generator |
| `platformio-update.yml` | — | — | PlatformIO registry update |

### Typical CI job structure

Each compiler/standard workflow follows the same pattern:

1. **Checkout** – `actions/checkout@v4`.
2. **Build** – set `CC`/`CXX`, call `cmake` with the appropriate `-D` flags,
   then `make -j`.
3. **Run tests** – execute `./test/etl_tests -v` (or `ctest -V` for cross-
   arch jobs).

The cross-architecture CI jobs additionally install a cross-compiler
toolchain and QEMU inside a `debian:trixie` container, use the matching
toolchain file from `.devcontainer/<arch>/`, and run tests via CTest (which
delegates to QEMU through `CMAKE_CROSSCOMPILING_EMULATOR`).

### Branches tested

* `master`
* `development`
* `pull-request/*`

All workflows run on both `push` and `pull_request` events (types: opened,
synchronize, reopened).

---

## Appveyor (Windows / MSVC)

The `appveyor.yml` at the repository root provides additional Windows CI
using **Visual Studio 2022**.  It builds the `master` branch only.

Configurations tested:

* Debug MSVC C++14
* Debug MSVC C++14 – No STL
* Debug MSVC C++17
* Debug MSVC C++17 – No STL
* Debug MSVC C++20
* Debug MSVC C++20 – No STL

The build uses the VS 2022 solution file at `test/vs2022/etl.vcxproj`.

---

## Code Coverage

The `coverage.yml` GitHub Actions workflow generates an **lcov** coverage
report:

1. Runs `test/run-coverage.sh` which builds and tests with GCC coverage
   flags.
2. Uploads the HTML report as a build artifact (retained for 30 days).
3. On pushes to `master`, deploys the report to **GitHub Pages**.

To generate coverage locally:

```bash
cd test
./run-coverage.sh
# Open test/build-coverage/coverage/index.html
```

---

## Generator Tests

The script **`scripts/generator_test.py`** verifies that the code generators
in `include/etl/generators/` produce output matching the checked-in header
files in `include/etl/private/`.

ETL uses [Cog](https://nedbatchelder.com/code/cog/) to generate certain
repetitive header files (e.g. `delegate.h`, `message_packet.h`).  The
generator templates live in `include/etl/generators/*_generator.h` and the
generated output is committed to `include/etl/private/*.h`.  This test
ensures the two stay in sync.

### Prerequisites

* **Python 3**
* **cogapp** – install via `pip install cogapp`

### Synopsis

```bash
python3 scripts/generator_test.py
```

### What the script does

1. Iterates over every `*_generator.h` file in `include/etl/generators/`.
2. Runs Cog on each generator, outputting to `build/generator_tmp/`.
3. Compares each generated file against the corresponding file in
   `include/etl/private/`.
4. Reports success if all files match, or failure if any differ.

The script exits with code `0` on success and `1` on failure.

### CI integration

The `generator.yml` GitHub Actions workflow runs this script automatically
on pushes and pull requests to verify generator consistency.

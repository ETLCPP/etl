# Building ETL with Meson

## Prerequisites

- [Meson](https://mesonbuild.com/) >= 0.57.0
- A C++17 compiler (GCC, Clang, or MSVC)
- [Ninja](https://ninja-build.org/) (default Meson backend)

UnitTest++ is fetched automatically as a Meson subproject — no manual dependency installation is needed.

## Quick Start

```bash
# Configure (from the project root)
meson setup builddir

# Build
meson compile -C builddir

# Run tests
meson test -C builddir
```

## Build Options

### ETL project options

| Option              | Type | Default | Description                                                       |
|---------------------|------|---------|-------------------------------------------------------------------|
| `use_stl`           | bool | `true`  | Build with STL support. When `false`, defines `ETL_NO_STL`.       |
| `enable_sanitizer`  | bool | `false` | Enable AddressSanitizer and UndefinedBehaviorSanitizer (GCC/Clang only). |

### Meson built-in options

| Option      | Type   | Default  | Description                                              |
|-------------|--------|----------|----------------------------------------------------------|
| `cpp_std`   | string | `c++17`  | C++ standard to compile with (e.g. `c++20`, `c++23`).   |
| `buildtype` | string | `debug`  | Build type: `plain`, `debug`, `debugoptimized`, `release`, `minsize`. |
| `werror`    | bool   | `false`  | Treat compiler warnings as errors.                       |

These are handled by Meson directly — no `get_option()` call is needed in the build files.

### Examples

```bash
# No STL, C++23
meson setup builddir -Duse_stl=false -Dcpp_std=c++23

# Release build with sanitizers
meson setup builddir -Dbuildtype=release -Denable_sanitizer=true

# Override the C++ standard on an existing build directory
meson configure builddir -Dcpp_std=c++20
```

## Selecting a Compiler

The compiler is chosen at configure time via environment variables:

```bash
# GCC
CC=gcc CXX=g++ meson setup builddir

# Clang
CC=clang CXX=clang++ meson setup builddir

# Specific versions
CC=gcc-14 CXX=g++-14 meson setup builddir
CC=clang-18 CXX=clang++-18 meson setup builddir
```

To switch compilers on an existing build directory, wipe it first:

```bash
CC=clang CXX=clang++ meson setup --wipe builddir
```

Or use separate directories per compiler:

```bash
CC=gcc CXX=g++ meson setup build-gcc
CC=clang CXX=clang++ meson setup build-clang
```

## Running Tests

```bash
# Run all tests
meson test -C builddir

# Verbose output (shows individual test results)
meson test -C builddir -v

# Run the test binary directly
./builddir/test/etl_unit_tests
```

## Sanitizers

On GCC and Clang, AddressSanitizer and UndefinedBehaviorSanitizer can be enabled via the `enable_sanitizer` option:

```bash
meson setup builddir -Denable_sanitizer=true
```

Note: UBSan may prevent certain `constexpr` evaluations involving function pointers from compiling (e.g. in the closure tests). This matches the CMake build, where sanitizers are also opt-in via `ETL_ENABLE_SANITIZER=ON`.

## Using ETL as a Subproject

ETL can be consumed as a Meson subproject. In your project's `subprojects/` directory, create an `etl.wrap` file, then use:

```meson
etl_dep = dependency('etl', fallback: ['etl', 'etl_dep'])
```

When built as a subproject, the ETL test suite is not compiled.

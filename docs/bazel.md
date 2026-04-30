# Building ETL with Bazel

ETL provides first-class [Bazel](https://bazel.build/) support, both for developing ETL itself and for consuming it as a dependency in your own projects.

## Prerequisites

- [Bazelisk](https://github.com/bazelbuild/bazelisk) (recommended) or [Bazel](https://bazel.build/install) 7.0 or later (with Bzlmod support)

[Bazelisk](https://github.com/bazelbuild/bazelisk) is a launcher that automatically downloads and runs the Bazel version specified in the `.bazelversion` file at the project root. This ensures all contributors use a consistent Bazel version. Simply install Bazelisk and use `bazel` as usual — it transparently delegates to the correct version.

## Syntax Checks

To validate that every ETL header is well-formed and compiles on its own (equivalent to `test/run-syntax-checks.sh` for CMake):

```sh
bazel build //test/syntax_check:syntax_check
```

This compiles a set of minimal `.t.cpp` files, each of which includes a single ETL header, with strict warning flags enabled.

## Cleaning Build Artifacts

To remove all build outputs and symlinks (`bazel-bin`, `bazel-out`, `bazel-etl`, etc.):

```sh
bazel clean
```

For a full cleanup including the external dependency cache:

```sh
bazel clean --expunge
```

## Running Unit Tests

To run the full test suite:

```sh
bazel test //test:etl_tests
```

You can also pass standard Bazel flags:

```sh
# Run with verbose test output
bazel test //test:etl_tests --test_output=all

# Run tests matching a filter (UnitTest++ subset)
bazel test //test:etl_tests --test_arg=<suite_name>
```

## Using ETL in Your Project

### With Bzlmod (recommended, Bazel 7+)

Add ETL as a dependency in your project's `MODULE.bazel`:

```python
bazel_dep(name = "etl", version = "20.47.1")

git_override(
    module_name = "etl",
    remote = "https://github.com/ETLCPP/etl.git",
    tag = "20.47.1",  # or a specific commit
)
```

Then depend on it in your `BUILD.bazel`:

```python
cc_library(
    name = "my_library",
    srcs = ["my_library.cpp"],
    hdrs = ["my_library.h"],
    deps = ["@etl//:etl"],
)
```

### With WORKSPACE (legacy)

In your `WORKSPACE` file:

```python
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "etl",
    remote = "https://github.com/ETLCPP/etl.git",
    tag = "20.47.1",
)
```

Then use `deps = ["@etl//:etl"]` in your targets as shown above.

## Project Structure

| File | Purpose |
|---|---|
| `MODULE.bazel` | Module definition and dependencies |
| `BUILD.bazel` | Exposes ETL as a `cc_library` |
| `.bazelversion` | Bazel version for Bazelisk |
| `.bazelrc` | Default Bazel settings |
| `test/BUILD.bazel` | Unit test target |
| `test/syntax_check/BUILD.bazel` | Header syntax check target |
| `test/UnitTest++/BUILD.bazel` | Vendored UnitTest++ framework |

## Cross-Compilation

Bazel supports cross-compilation through its [platforms](https://bazel.build/extending/platforms) and [toolchains](https://bazel.build/extending/toolchains) system. Since ETL is a header-only library, there is nothing to cross-compile for the library itself. However, when building tests or consuming ETL in an application targeting a different architecture, you need to define a platform and register an appropriate C++ toolchain.

Example platform definition (e.g. in a `platforms/BUILD.bazel`):

```python
platform(
    name = "linux_arm64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)
```

Then build with:

```sh
bazel build //:etl --platforms=//platforms:linux_arm64
```

> **Note:** You must also have a C++ toolchain registered that supports the target platform.
> See the [Bazel toolchains documentation](https://bazel.build/extending/toolchains) for details.

### Running Cross-Compiled Tests under QEMU

Cross-compiled test binaries cannot run natively on the host. Pre-defined configurations in `.bazelrc` select the correct cross-compiler via `--repo_env=CC`, set the build flags to match `.devcontainer/run-tests.sh` (C++23, No-STL, `-O0`), and use `--run_under` to execute the resulting binary under the appropriate QEMU emulator:

```sh
# Cross-build and run tests for ARM (armhf)
bazel test //test:etl_tests --config=armhf

# Other architectures
bazel test //test:etl_tests --config=i386
bazel test //test:etl_tests --config=powerpc
bazel test //test:etl_tests --config=riscv64
bazel test //test:etl_tests --config=s390x
```

These configs are designed to run inside the Docker containers under `.devcontainer/`, which provide the cross-compiler toolchains and QEMU binaries. Each config sets `CC`, `AR`, `LD`, `NM`, `STRIP`, and `OBJDUMP` via `--repo_env` so that Bazel's auto-configured toolchain finds the complete prefixed cross-tool suite.

You can also use `--run_under` directly for custom setups:

```sh
bazel test //test:etl_tests --run_under=/usr/bin/qemu-arm-static
```

## Compiler and Build Configuration

Unlike CMake where options like `ETL_CXX_STANDARD` and `CMAKE_CXX_COMPILER` are set at configure time, Bazel uses command-line flags and `.bazelrc` configurations.

### C++ Standard Version

Use `--cxxopt` to pass the desired standard flag:

```sh
# C++17 (default in .bazelrc)
bazel test //test:etl_tests --cxxopt=-std=c++17

# C++20
bazel test //test:etl_tests --cxxopt=-std=c++20

# C++23
bazel test //test:etl_tests --cxxopt=-std=c++23

# C++14
bazel test //test:etl_tests --cxxopt=-std=c++14
```

### Optimization Level

Use `--compilation_mode` (shorthand `-c`) for standard profiles, or `--copt` for explicit flags:

```sh
# Debug (default) — no optimization, debug symbols
bazel test //test:etl_tests -c dbg

# Optimized — O2 with NDEBUG
bazel test //test:etl_tests -c opt

# Fast build — no optimization, no debug symbols
bazel test //test:etl_tests -c fastbuild

# Custom optimization level
bazel test //test:etl_tests --copt=-O3
bazel test //test:etl_tests --copt=-O1
bazel test //test:etl_tests --copt=-Os
```

### Selecting the Compiler (GCC vs Clang)

Bazel uses the system's default `CC` environment variable. Override it to switch compilers:

```sh
# Use Clang
bazel test //test:etl_tests --repo_env=CC=clang

# Use a specific GCC version
bazel test //test:etl_tests --repo_env=CC=gcc-13

# Use a specific Clang version
bazel test //test:etl_tests --repo_env=CC=clang-18
```

> **Note:** Bazel's auto-configured toolchain infers the C++ compiler from `CC` automatically
> (e.g. `CC=gcc-13` → `g++-13` for C++ compilation). There is no need to set `CXX` separately.

### Combining Options

Flags can be combined freely:

```sh
# Clang, C++20, optimized
bazel test //test:etl_tests --repo_env=CC=clang --cxxopt=-std=c++20 -c opt

# GCC 13, C++23, debug
bazel test //test:etl_tests --repo_env=CC=gcc-13 --cxxopt=-std=c++23 -c dbg
```

### STL vs. No-STL Mode

ETL can operate without the standard library, which is common on bare-metal embedded targets. Use `--copt` to define `ETL_NO_STL`:

```sh
# Build and test without STL
bazel test //test:etl_tests --copt=-DETL_NO_STL

# Build with STL (default, no flag needed)
bazel test //test:etl_tests
```

When `ETL_NO_STL` is defined, ETL provides its own implementations of containers, algorithms, and utilities instead of delegating to `<algorithm>`, `<type_traits>`, etc.

### Type Traits Configuration

ETL supports three type traits strategies, controlled via preprocessor defines:

| Mode | Define | Description |
|---|---|---|
| **STL type traits** | *(default)* | Uses `<type_traits>` from the standard library |
| **Compiler builtins** | `ETL_USE_TYPE_TRAITS_BUILTINS` | Uses compiler intrinsics (`__is_trivially_copyable`, etc.) — useful when STL headers are unavailable or incomplete |
| **User-defined** | `ETL_USER_DEFINED_TYPE_TRAITS` | Uses ETL's own type traits implementations |

```sh
# Use compiler built-in type traits
bazel test //test:etl_tests --copt=-DETL_USE_TYPE_TRAITS_BUILTINS

# Use ETL's own user-defined type traits
bazel test //test:etl_tests --copt=-DETL_USER_DEFINED_TYPE_TRAITS
```

These are mutually exclusive — define at most one. If neither is defined and STL is available, ETL uses `<type_traits>`.

### Other Configuration Defines

Additional defines can be passed the same way via `--copt=-D...`:

| Define | Description |
|---|---|
| `ETL_FORCE_TEST_CPP03_IMPLEMENTATION` | Force C++03 code paths even when a newer standard is available |
| `ETL_MESSAGES_ARE_NOT_VIRTUAL` | Use non-virtual message types |

```sh
# Force C++03 implementation paths
bazel test //test:etl_tests --copt=-DETL_FORCE_TEST_CPP03_IMPLEMENTATION
```

### Using `.bazelrc` Presets

To avoid retyping flags, add configurations to `.bazelrc`:

```
# .bazelrc

# Named configurations
build:clang    --repo_env=CC=clang
build:gcc13    --repo_env=CC=gcc-13
build:c++20    --cxxopt=-std=c++20
build:c++23    --cxxopt=-std=c++23
build:release  --compilation_mode=opt
```

Then use them with `--config`:

```sh
bazel test //test:etl_tests --config=clang --config=c++20 --config=release
```

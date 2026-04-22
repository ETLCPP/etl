# Docker for Development

## Overview

The ETL repository ships a set of Docker-based development environments under
`.devcontainer/`. They give every contributor an identical, reproducible toolchain
regardless of host operating system. Three flavours are provided:

| Flavour | Path | Purpose |
|---|---|---|
| **Default** | `.devcontainer/` | Day-to-day development (Microsoft C++ dev-container base image) |
| **Compiler-specific** | `.devcontainer/gcc09/` … `.devcontainer/gcc15/`, `.devcontainer/clang7/` … `.devcontainer/clang21/` | Test against a specific GCC or Clang version |
| **s390x big-endian** | `.devcontainer/s390x/` | Cross-compile and run tests on an s390x target via QEMU |

All containers include CMake, Make, Git, Python 3, cogapp (the code generator used
by ETL), clang-format 18, and treefmt.

## Prerequisites

- **Docker** (or Docker Desktop) – any recent version that supports `docker build`
  and `docker run`.
- **VS Code** with the
  [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)
  extension (optional, but recommended for the smoothest experience).
- **Git** – to clone the repository.

## Quick Start

### Using the helper script

The fastest way to get a shell inside the default container from the project root:

```bash
./scripts/run-docker.sh
```

This script performs two steps:

1. **Builds** the image from `.devcontainer/Dockerfile` and tags it `etl`.
2. **Runs** an interactive container that bind-mounts the repository at
   `/home/vscode/etl` so that edits made inside the container are visible on
   the host (and vice versa).

You are dropped into a Bash shell as the `vscode` user with the working
directory set to the repository root.

### Using VS Code Dev Containers

1. Open the repository folder in VS Code.
2. When prompted, click **Reopen in Container** – or run the command
   *Dev Containers: Reopen in Container* from the command palette.
3. VS Code reads `.devcontainer/devcontainer.json`, builds the image, and
   attaches to the running container automatically.

To open a **specific compiler variant** instead, run
*Dev Containers: Open Folder in Container…* and pick the sub-folder (e.g.
`.devcontainer/gcc14/`), or use the command palette action
*Dev Containers: Open Named Container Configuration…* and select the desired
name (e.g. "Gcc 14", "Clang 18").

## Default Development Container

The file `.devcontainer/Dockerfile` is a multi-purpose image used by the
default configuration **and** by every compiler-specific variant (they simply
override the `BASE_IMAGE_NAME` build argument).

### Base image

```text
mcr.microsoft.com/devcontainers/cpp:2
```

The exact digest is pinned in `devcontainer.json` so that builds are
reproducible even if the upstream tag is updated.

### Installed tools

The Dockerfile installs the following on top of the base image:

| Tool | Purpose |
|---|---|
| `python3`, `pip` | Runtime for helper scripts |
| `python3-cogapp` / `cogapp` | ETL code generator |
| `git` | Version control |
| `wget` | Downloading additional tooling |
| `cmake`, `make` | Build system |
| `clang-format` (v18) | Source formatting (see [source-formatting.md](source-formatting.md)) |
| `treefmt` (v2.4.1) | Single-command formatting wrapper |

### Reproducible builds with Debian snapshots

The default configuration sets the build argument
`DEBIAN_SNAPSHOT=20260223T000000Z`. When this value is not `"none"`, the
Dockerfile rewrites the APT sources to point at
`snapshot.debian.org/archive/debian/<timestamp>`, ensuring that every
contributor installs identical package versions. Compiler-specific variants
that are based on upstream `gcc:*` or `silkeh/clang:*` images set
`DEBIAN_SNAPSHOT=none` because those images manage their own package sources.

## Compiler-Specific Containers

Each sub-folder under `.devcontainer/` contains a `devcontainer.json` that
reuses the **same** `Dockerfile` (`../Dockerfile`) but overrides the
`BASE_IMAGE_NAME` build argument to select a different compiler.

### GCC variants

| Folder | Base image | Name |
|---|---|---|
| `gcc09/` | `gcc:9` | Gcc 09 |
| `gcc10/` | `gcc:10` | Gcc 10 |
| `gcc11/` | `gcc:11` | Gcc 11 |
| `gcc12/` | `gcc:12` | Gcc 12 |
| `gcc13/` | `gcc:13` | Gcc 13 |
| `gcc14/` | `gcc:14` | Gcc 14 |
| `gcc15/` | `gcc:15` | Gcc 15 |

### Clang variants

| Folder | Base image | Name |
|---|---|---|
| `clang7/` | `silkeh/clang:7` | Clang 7 |
| `clang8/` | `silkeh/clang:8` | Clang 8 |
| `clang9/` | `silkeh/clang:9` | Clang 9 |
| `clang10/` | `silkeh/clang:10` | Clang 10 |
| `clang11/` | `silkeh/clang:11` | Clang 11 |
| `clang12/` | `silkeh/clang:12` | Clang 12 |
| `clang13/` | `silkeh/clang:13` | Clang 13 |
| `clang14/` | `silkeh/clang:14` | Clang 14 |
| `clang15/` | `silkeh/clang:15` | Clang 15 |
| `clang16/` | `silkeh/clang:16` | Clang 16 |
| `clang17/` | `silkeh/clang:17` | Clang 17 |
| `clang18/` | `silkeh/clang:18` | Clang 18 |
| `clang19/` | `silkeh/clang:19` | Clang 19 |
| `clang20/` | `silkeh/clang:20` | Clang 20 |
| `clang21/` | `silkeh/clang:21` | Clang 21 |

All compiler-specific variants set `DEBIAN_SNAPSHOT` to `"none"` because they
rely on the upstream image's own package sources.

## s390x Big-Endian Cross-Compilation

The `s390x` container lives in `.devcontainer/s390x/` and has its **own**
Dockerfile (it does not reuse the default one). It is based on
`debian:trixie` and installs:

- QEMU user-mode emulation (`qemu-user-static`, `qemu-user`, `binfmt-support`)
- s390x cross-compilation toolchain (`gcc-s390x-linux-gnu`,
  `g++-s390x-linux-gnu`)
- CMake, Make, Ninja, Git, wget

### Container setup

Open `.devcontainer/s390x/` as a Dev Container in VS Code, or build manually:

```bash
docker build -t etl-s390x .devcontainer/s390x
docker run -it --rm -v .:/workspaces/etl -w /workspaces/etl etl-s390x
```

### CMake toolchain

A CMake toolchain file is provided at
`.devcontainer/s390x/toolchain-s390x.cmake`. It sets:

- `CMAKE_SYSTEM_PROCESSOR` to `s390x`
- Cross-compilers `s390x-linux-gnu-gcc` / `g++`
- `CMAKE_CROSSCOMPILING_EMULATOR` to `/usr/bin/qemu-s390x-static`

The VS Code Dev Container configuration already passes this toolchain file
via `cmake.configureArgs`, so CMake Tools picks it up automatically.

### Running tests under QEMU

Because the toolchain file sets `CMAKE_CROSSCOMPILING_EMULATOR`, CTest
automatically invokes `qemu-s390x-static` when running test binaries.
No extra flags are needed:

```bash
cmake -S test -B build-s390x \
  -DCMAKE_TOOLCHAIN_FILE=.devcontainer/s390x/toolchain-s390x.cmake \
  -DBUILD_TESTS=ON -DNO_STL=OFF -DETL_CXX_STANDARD=17 -G Ninja
cmake --build build-s390x
ctest --test-dir build-s390x
```

## Building and Running Tests

Once inside any container (default, compiler-specific, or s390x) you can
build and run the ETL test suite.

### Quick CMake workflow

```bash
# Configure – build tests with C++17
cmake -S test -B build -DBUILD_TESTS=ON -DETL_CXX_STANDARD=17

# Build
cmake --build build -j $(nproc)

# Run tests
ctest --test-dir build
```

Change `DETL_CXX_STANDARD` to `11`, `14`, `17`, `20`, or `23` as needed.
Add `-DNO_STL=ON` to build without the standard library.

### Using the run-tests script

The repository also provides a convenience script in `test/`:

```bash
cd test
./run-tests.sh <standard> [optimisation] [threads] [sanitizer] [compiler] [verbose]
```

| Argument | Values | Default |
|---|---|---|
| C++ standard | `11`, `14`, `17`, `20`, `23` | *(required)* |
| Optimisation | `0`, `1`, `2`, `3` | `0` |
| Threads | any integer | `4` |
| Sanitizer | `s` (enable) / `n` (disable) | `n` |
| Compiler | `gcc` / `clang` | all |
| Verbose | `v` (enable) / `n` (disable) | `n` |

Example – run C++17 tests at `-O2` with 8 threads using GCC:

```bash
./run-tests.sh 17 2 8 n gcc n
```

## Formatting Inside the Container

The default container ships with **clang-format 18** and **treefmt**.
See [source-formatting.md](source-formatting.md) for the full formatting guide.

Quick reference:

```bash
# Format all tracked C/C++ files with treefmt
treefmt

# Or use clang-format directly via the wrapper
./scripts/clang-format-wrapper -i include/etl/*.h
```

The wrapper script `scripts/clang-format-wrapper` resolves the correct
clang-format binary (prefers `clang-format-18`, falls back to `clang-format`
after checking the major version).

## Customisation

To add extra packages or tools to the default container, edit
`.devcontainer/Dockerfile`. The image follows a straightforward
`apt-get install` pattern, so adding a new package is as simple as appending
it to the existing `apt-get` line.

To create a new compiler variant:

1. Create a folder under `.devcontainer/` (e.g. `.devcontainer/gcc16/`).
2. Add a `devcontainer.json` that references `"../Dockerfile"` and sets
   `BASE_IMAGE_NAME` to the desired image (e.g. `gcc:16`).
3. Set `DEBIAN_SNAPSHOT` to `"none"` for upstream compiler images.

Example:

```jsonc
{
  "name": "Gcc 16",
  "build": {
    "dockerfile": "../Dockerfile",
    "args": {
      "BASE_IMAGE_NAME": "gcc:16",
      "DEBIAN_SNAPSHOT": "none"
    },
    "context": "../context"
  }
}
```

## Troubleshooting

| Symptom | Cause / Fix |
|---|---|
| `apt-get` fails with *"Release file … is not valid yet"* | The Debian snapshot timestamp is in the future relative to the build host clock. Either update `DEBIAN_SNAPSHOT` in `devcontainer.json` or set it to `"none"`. |
| `clang-format` reports the wrong version | The wrapper expects version **18**. Make sure the image installs `clang-format` (or `clang-format-18`) and that the binary is on `PATH`. |
| Permission errors on mounted files | The `run-docker.sh` script runs as user `vscode`. Ensure your host UID matches, or adjust the `--user` flag. |
| s390x tests crash immediately | Verify that `qemu-user-static` is installed and that `binfmt-support` is active. On some hosts you may need to register binfmt handlers with `docker run --privileged --rm tonistiigi/binfmt --install all`. |
| Build is very slow the first time | Docker is downloading and building the image from scratch. Subsequent builds use the layer cache and are much faster. |

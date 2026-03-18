# Using Docker for ETL development

## Overview

There are two separate Docker setups in this repository:

| Path | Purpose |
|---|---|
| `docker/Dockerfile` | Standalone local development image — described in this document. |
| `.devcontainer/*` | GitHub Codespaces / VS Code Dev Containers configuration. These Dockerfiles and `devcontainer.json` files are consumed automatically by GitHub Codespaces (and the VS Code *Dev Containers* extension) and should **not** be used for plain Docker builds. |

The rest of this page covers the **`docker/Dockerfile`** workflow only.

---

## Building the image

From the repository root, run:

```bash
docker build -t etl docker/
```

The default base image is `debian:trixie-20260316`. You can override it with the
`BASE_IMAGE_NAME` build argument:

```bash
docker build -t etl --build-arg BASE_IMAGE_NAME="ubuntu:24.04" docker/
```

### What is included in the image

The image installs the toolchain needed to build and test the ETL project:

- **Compilers:** GCC (`g++`) and Clang
- **Build tools:** CMake, Make
- **Utilities:** Git, wget, `python3-cogapp` (used for code generation)

A non-root user `developer` (UID 1000) is created and set as the default user.
If the base image already contains a user with UID 1000 (e.g. the `ubuntu` user
in Ubuntu-based images), that user is automatically removed and replaced by
`developer`.

## Running a container

The helper script `run-docker.sh` starts an interactive container with the
repository bind-mounted into `~/etl`:

```bash
docker/run-docker.sh
```

Or run it manually:

```bash
docker run -it --rm -v .:/home/developer/etl -u developer -w /home/developer/etl etl /bin/bash
```

Once inside the container, you can test the project in the usual way:

```bash
cd test
./run-tests.sh 17 0 $(nproc)
```

## `.devcontainer` — GitHub Codespaces

The `.devcontainer/` directory contains multiple configurations (e.g.
`gcc11`, `gcc14`, `clang16`, …) that target different compiler versions.
These are **exclusively** intended for use with
[GitHub Codespaces](https://github.com/features/codespaces) and the
[VS Code Dev Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers).

They rely on Microsoft-provided base images
(`mcr.microsoft.com/devcontainers/cpp`) and additional scripts that are not
suitable for standalone Docker usage. If you want to build and test ETL locally
with Docker, use `docker/Dockerfile` as described above.

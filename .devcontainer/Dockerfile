ARG BASE_IMAGE_NAME="mcr.microsoft.com/devcontainers/cpp:2@sha256:a5eb5a1e9109af88bf82ebb0f71903608a68144851ed1e4b852e31b251ac59c6"

FROM ${BASE_IMAGE_NAME}

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.31.7"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        git \
        wget \
        python3-cogapp \
    && rm -rf /var/lib/apt/lists/*

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

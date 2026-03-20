ARG BASE_IMAGE_NAME="mcr.microsoft.com/devcontainers/cpp:2@sha256:a5eb5a1e9109af88bf82ebb0f71903608a68144851ed1e4b852e31b251ac59c6"

FROM ${BASE_IMAGE_NAME}

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.31.7"
ARG DEBIAN_SNAPSHOT="20260223T000000Z"

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8 \
    TZ=UTC

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /usr/local/share/reinstall-cmake.sh

RUN set -eux \
    && export DEBIAN_FRONTEND=noninteractive \
    && if [[ "${DEBIAN_SNAPSHOT}" != "none" ]]; then \
        test -f /etc/apt/sources.list.d/debian.sources; \
        sed -ri "s|^URIs: https://deb.debian.org/debian$|URIs: http://snapshot.debian.org/archive/debian/${DEBIAN_SNAPSHOT}|g" /etc/apt/sources.list.d/debian.sources; \
        sed -ri "s|^URIs: https://deb.debian.org/debian-security$|URIs: http://snapshot.debian.org/archive/debian-security/${DEBIAN_SNAPSHOT}|g" /etc/apt/sources.list.d/debian.sources; \
        printf 'Acquire::Check-Valid-Until "false";\n' > /etc/apt/apt.conf.d/99snapshot; \
    fi \
    && apt-get update \
    && apt-get -y install --no-install-recommends \
        git \
        wget \
        python3-cogapp \
    && rm -rf /var/lib/apt/lists/*

RUN set -eux \
    && if [[ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]]; then \
        chmod +x /usr/local/share/reinstall-cmake.sh \
        && \
        /usr/local/share/reinstall-cmake.sh "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}"; \
    fi \
    && rm -f /usr/local/share/reinstall-cmake.sh

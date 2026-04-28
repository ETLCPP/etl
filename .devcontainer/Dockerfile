ARG BASE_IMAGE_NAME="mcr.microsoft.com/devcontainers/cpp:2@sha256:a5eb5a1e9109af88bf82ebb0f71903608a68144851ed1e4b852e31b251ac59c6"

FROM ${BASE_IMAGE_NAME}

ARG DEBIAN_SNAPSHOT="20260223T000000Z"

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

ENV LANG=C.UTF-8 \
    LC_ALL=C.UTF-8 \
    TZ=UTC

RUN set -eux \
    && export DEBIAN_FRONTEND=noninteractive \
    && if [[ "${DEBIAN_SNAPSHOT}" != "none" ]]; then \
        snapshot_main="http://snapshot.debian.org/archive/debian/${DEBIAN_SNAPSHOT}"; \
        snapshot_security="http://snapshot.debian.org/archive/debian-security/${DEBIAN_SNAPSHOT}"; \
        if [[ -f /etc/apt/sources.list.d/debian.sources ]]; then \
            sed -ri "s|^URIs: https?://deb.debian.org/debian$|URIs: ${snapshot_main}|g" /etc/apt/sources.list.d/debian.sources; \
            sed -ri "s|^URIs: https?://deb.debian.org/debian-security$|URIs: ${snapshot_security}|g" /etc/apt/sources.list.d/debian.sources; \
        elif [[ -f /etc/apt/sources.list ]] || compgen -G "/etc/apt/sources.list.d/*.list" > /dev/null; then \
            for list_file in /etc/apt/sources.list /etc/apt/sources.list.d/*.list; do \
                [[ -f "${list_file}" ]] || continue; \
                sed -ri "s|https?://deb.debian.org/debian|${snapshot_main}|g" "${list_file}"; \
                sed -ri "s|https?://security.debian.org/debian-security|${snapshot_security}|g" "${list_file}"; \
                sed -ri "s|https?://deb.debian.org/debian-security|${snapshot_security}|g" "${list_file}"; \
            done; \
        else \
            echo "ERROR: DEBIAN_SNAPSHOT='${DEBIAN_SNAPSHOT}' is set, but no supported apt source files were found."; \
            echo "Expected /etc/apt/sources.list.d/debian.sources or one or more .list files under /etc/apt/."; \
            exit 1; \
        fi; \
        printf 'Acquire::Check-Valid-Until "false";\n' > /etc/apt/apt.conf.d/99snapshot; \
    fi \
    && apt-get update \
    && apt-get -y install --no-install-recommends \
        python3-full \
        python3-pip \
        python3-cogapp \
        git \
        wget \
        cmake \
        clang-format \
        clang-format-18 \
        lcov \
    && rm -rf /var/lib/apt/lists/*

RUN set -eux; \
    VERSION="2.4.1"; \
    case "$(uname -m)" in \
        x86_64)  ARCH="amd64"; SHA256="bdaa2c0fbee03e5c2f99e605d9419386ce5d558440baac2017398faada839e04" ;; \
        aarch64) ARCH="arm64"; SHA256="0a09e1f04a0f8a86fd4e709552613f5d82adf6bc72f0a4b5e217670894e79fbf" ;; \
        *) echo "Unsupported architecture: $(uname -m)"; exit 1 ;; \
    esac; \
    wget -O treefmt.tar.gz "https://github.com/numtide/treefmt/releases/download/v${VERSION}/treefmt_${VERSION}_linux_${ARCH}.tar.gz" \
    && echo "${SHA256} treefmt.tar.gz" | sha256sum -c \
    && tar xzf treefmt.tar.gz treefmt \
    && install -m 755 treefmt /usr/bin/treefmt \
    && rm treefmt.tar.gz treefmt

RUN set -eux \
    && echo "Pip version: " \
    && pip --version \
    && echo "Cogapp version: " \
    && pip show cogapp \
    && echo "Git version: " \
    && git --version \
    && echo "Wget version: " \
    && wget --version \
    && echo "Cmake version: " \
    && cmake --version \
    && echo "Make version: " \
    && make --version

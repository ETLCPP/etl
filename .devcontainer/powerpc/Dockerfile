# powerpc Test Environment for ETL
# Uses QEMU user-mode emulation to run powerpc binaries on x64 host
#
# NOTE: floating tags like "sid" or "sid-slim" drift over time and cannot be
# pinned retroactively (Docker Hub keeps only the latest manifest per tag).
# debian:sid-20260202 is a dated, immutable tag (built and pushed once, not
# retagged later), pinned here by digest for full reproducibility. All apt
# sources are also switched to the matching snapshot.debian.org archive
# before the first "apt-get update" runs.
FROM debian@sha256:9ab6e6f3543eeb30510c516377bbb05f95ebb7507a956c0b772184deb7ecd340

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Pin apt to the snapshot.debian.org archive for the given date BEFORE any
# package installation, so every apt-get update/install in this file (not
# just the ones after this point) resolves to the exact same package set.
RUN rm -f /etc/apt/sources.list /etc/apt/sources.list.d/*.sources

RUN cat <<EOF > /etc/apt/sources.list.d/debian.sources
Types: deb
URIs: http://snapshot.debian.org/archive/debian/20260202T000000Z
Suites: sid
Components: main
Signed-By: /usr/share/keyrings/debian-archive-keyring.pgp
EOF

RUN echo 'Acquire::Check-Valid-Until "false";' > /etc/apt/apt.conf.d/99no-check-valid

# Install QEMU user-mode emulation and powerpc cross-compilation tools
RUN dpkg --add-architecture powerpc && \
    apt-get update && apt-get install -y --no-install-recommends \
    binfmt-support \
    gpg \
    ca-certificates \
    cmake \
    make \
    ninja-build \
    git \
    wget \
    file \
    debian-ports-archive-keyring \
    && rm -rf /var/lib/apt/lists/*

RUN cat <<EOF > /etc/apt/sources.list.d/powerpc.sources
Types: deb
URIs: http://snapshot.debian.org/archive/debian-ports/20260202T000000Z
Suites: sid
Components: main
Architectures: powerpc
Signed-By: /usr/share/keyrings/debian-ports-archive-keyring.gpg
EOF

RUN apt-get update && apt-get install -y --no-install-recommends \
    qemu-user-static \
    qemu-user \
    gcc-powerpc-linux-gnu \
    g++-powerpc-linux-gnu \
    libc6:powerpc \
    libstdc++6:powerpc \
    libatomic1:powerpc \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user with stable UID/GID
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=1000

RUN groupadd --gid ${USER_GID} ${USERNAME} && \
    useradd --uid ${USER_UID} --gid ${USER_GID} --shell /bin/bash --create-home ${USERNAME}

# Set working directory
WORKDIR /workspaces/etl

# Install Bazelisk as 'bazel'
RUN ARCH=$(uname -m | sed 's/x86_64/amd64/;s/aarch64/arm64/') && \
    wget -qO /usr/local/bin/bazel https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-${ARCH} && \
    chmod +x /usr/local/bin/bazel

# Verify QEMU and cross-compilation setup
RUN echo "=== Host Architecture ===" && \
    uname -m && \
    echo "" && \
    echo "=== powerpc Cross Compiler ===" && \
    powerpc-linux-gnu-gcc --version && \
    echo "" && \
    echo "=== QEMU powerpc ===" && \
    qemu-ppc-static --version | head -n1

# Ensure workspace directory ownership for non-root user
RUN mkdir -p /workspaces/etl && chown -R ${USERNAME}:${USERNAME} /workspaces

# Switch to non-root user
USER ${USERNAME}

# Default command
CMD ["/bin/bash"]

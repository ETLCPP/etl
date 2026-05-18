# armhf Test Environment for ETL
# Uses QEMU user-mode emulation to run armhf binaries on x64 host
FROM debian:trixie

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install QEMU user-mode emulation and armhf cross-compilation tools
RUN dpkg --add-architecture armhf && \
    apt-get update && apt-get install -y --no-install-recommends \
    qemu-user-static \
    qemu-user \
    binfmt-support \
    gcc-arm-linux-gnueabihf \
    g++-arm-linux-gnueabihf \
    cmake \
    make \
    ninja-build \
    git \
    wget \
    ca-certificates \
    file \
    libc6:armhf \
    libstdc++6:armhf \
    libatomic1:armhf \
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
    echo "=== armhf Cross Compiler ===" && \
    arm-linux-gnueabihf-gcc --version && \
    echo "" && \
    echo "=== QEMU arm ===" && \
    qemu-arm-static --version | head -n1

# Ensure workspace directory ownership for non-root user
RUN mkdir -p /workspaces/etl && chown -R ${USERNAME}:${USERNAME} /workspaces

# Switch to non-root user
USER ${USERNAME}

# Default command
CMD ["/bin/bash"]

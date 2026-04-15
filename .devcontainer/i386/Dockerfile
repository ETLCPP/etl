# i386 Test Environment for ETL
# Uses QEMU user-mode emulation to run i386 binaries on x64 host
FROM debian:trixie

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install QEMU user-mode emulation and i386 cross-compilation tools
RUN dpkg --add-architecture i386 && \
    apt-get update && apt-get install -y --no-install-recommends \
    qemu-user-static \
    qemu-user \
    binfmt-support \
    gcc-i686-linux-gnu \
    g++-i686-linux-gnu \
    cmake \
    make \
    ninja-build \
    git \
    wget \
    file \
    libc6:i386 \
    libstdc++6:i386 \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user with stable UID/GID
ARG USERNAME=devuser
ARG USER_UID=1000
ARG USER_GID=1000

RUN groupadd --gid ${USER_GID} ${USERNAME} && \
    useradd --uid ${USER_UID} --gid ${USER_GID} --shell /bin/bash --create-home ${USERNAME}

# Set working directory
WORKDIR /workspaces/etl

# Verify QEMU and cross-compilation setup
RUN echo "=== Host Architecture ===" && \
    uname -m && \
    echo "" && \
    echo "=== i386 Cross Compiler ===" && \
    i686-linux-gnu-gcc --version && \
    echo "" && \
    echo "=== QEMU i386 ===" && \
    qemu-i386-static --version | head -n1

# Ensure workspace directory ownership for non-root user
RUN mkdir -p /workspaces/etl && chown -R ${USERNAME}:${USERNAME} /workspaces

# Switch to non-root user
USER ${USERNAME}

# Default command
CMD ["/bin/bash"]

# s390x Big-Endian Test Environment for ETL
# Uses QEMU user-mode emulation to run s390x binaries on x64 host
FROM debian:bookworm

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install QEMU user-mode emulation and s390x cross-compilation tools
RUN dpkg --add-architecture s390x && \
    apt-get update && apt-get install -y \
    qemu-user-static \
    qemu-user \
    binfmt-support \
    gcc-s390x-linux-gnu \
    g++-s390x-linux-gnu \
    cmake \
    make \
    git \
    wget \
    file \
    libc6:s390x \
    libstdc++6:s390x \
    && rm -rf /var/lib/apt/lists/*

# Set up s390x as default cross-compilation target
ENV CROSS_COMPILE=s390x-linux-gnu-
ENV CC=s390x-linux-gnu-gcc
ENV CXX=s390x-linux-gnu-g++
ENV AR=s390x-linux-gnu-ar
ENV AS=s390x-linux-gnu-as
ENV LD=s390x-linux-gnu-ld
ENV STRIP=s390x-linux-gnu-strip

# Set working directory
WORKDIR /workspaces/etl

# Verify QEMU and cross-compilation setup
RUN echo "=== Host Architecture ===" && \
    uname -m && \
    echo "" && \
    echo "=== s390x Cross Compiler ===" && \
    s390x-linux-gnu-gcc --version && \
    echo "" && \
    echo "=== QEMU s390x ===" && \
    qemu-s390x-static --version | head -n1

# Default command
CMD ["/bin/bash"]

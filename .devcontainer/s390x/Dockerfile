# s390x Big-Endian Test Environment for ETL
# Based on Debian 12 (Bookworm) for s390x architecture
FROM s390x/debian:bookworm

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    make \
    git \
    wget \
    util-linux \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspaces/etl

# Verify architecture and endianness
RUN echo "=== System Information ===" && \
    uname -m && \
    lscpu | grep -E "Architecture|Byte Order" && \
    echo "=========================="

# Default command
CMD ["/bin/bash"]

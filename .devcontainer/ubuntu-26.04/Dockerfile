FROM ubuntu:26.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    ninja-build \
    python3 \
    python3-pip \
    clang \
    docker.io \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash vscode

WORKDIR /etl

CMD ["/bin/bash"]

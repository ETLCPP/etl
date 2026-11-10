# Pinned by digest for reproducible builds.
# To update: docker buildx imagetools inspect debian:sid-slim
FROM debian:sid-slim@sha256:ec3fa4e0b2987ae47be353f56854191e300261915470e40165b1c906d22a65db

# Pin the package archive to a snapshot.debian.org timestamp so that
# `apt-get install` resolves the same package versions on every build.
# This is the snapshot the base image itself was built from - see the comment
# in /etc/apt/sources.list.d/debian.sources of the pinned image above.
ARG DEBIAN_SNAPSHOT=20260918T000000Z

# snapshot.debian.org serves archived Release files whose Valid-Until has long
# since passed, so that check has to be disabled.
# http is used rather than https because the slim base image has no
# ca-certificates; integrity is guaranteed by the archive's GPG signature.
RUN printf '%s\n' \
    'Types: deb' \
    "URIs: http://snapshot.debian.org/archive/debian/${DEBIAN_SNAPSHOT}/" \
    'Suites: sid' \
    'Components: main' \
    'Check-Valid-Until: no' \
    'Signed-By: /usr/share/keyrings/debian-archive-keyring.pgp' \
    > /etc/apt/sources.list.d/debian.sources

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    ninja-build \
    python3 \
    python3-cogapp \
    gcc-16 \
    g++-16 \
    && rm -rf /var/lib/apt/lists/*

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-16 100 \
    --slave /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-16 \
    --slave /usr/bin/gcc-nm gcc-nm /usr/bin/gcc-nm-16 \
    --slave /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-16 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-16 100

# Create non-root user with stable UID/GID
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=1000

RUN groupadd --gid ${USER_GID} ${USERNAME} && \
    useradd --uid ${USER_UID} --gid ${USER_GID} --shell /bin/bash --create-home ${USERNAME}

# Ensure workspace directory ownership for non-root user
RUN mkdir -p /etl && chown -R ${USERNAME}:${USERNAME} /etl

WORKDIR /etl

# Switch to non-root user
USER ${USERNAME}

CMD ["/bin/bash"]

ARG BASE_IMAGE_NAME="clang:latest"

FROM ${BASE_IMAGE_NAME}

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.31.7"

# Optionally install the cmake for vcpkg
COPY ./reinstall-cmake.sh /tmp/

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        git \
        wget

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi \
    && rm -f /tmp/reinstall-cmake.sh

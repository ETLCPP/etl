ARG BASE_IMAGE_NAME="debian:trixie-20260316"

FROM ${BASE_IMAGE_NAME}

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        clang \
        cmake \
        g++ \
        git \
        make \
        python3-cogapp \
        wget \
    && rm -rf /var/lib/apt/lists/*

ARG USERNAME=developer
ARG USER_UID=1000
ARG USER_GID=${USER_UID}
RUN EXISTING_USER=$(getent passwd ${USER_UID} | cut -d: -f1) \
    && if [ -n "${EXISTING_USER}" ] && [ "${EXISTING_USER}" != "${USERNAME}" ]; then \
           userdel -r "${EXISTING_USER}" || true; \
       fi \
    && EXISTING_GROUP=$(getent group ${USER_GID} | cut -d: -f1) \
    && if [ -n "${EXISTING_GROUP}" ] && [ "${EXISTING_GROUP}" != "${USERNAME}" ]; then \
           groupdel "${EXISTING_GROUP}" || true; \
       fi \
    && groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME}

USER ${USERNAME}

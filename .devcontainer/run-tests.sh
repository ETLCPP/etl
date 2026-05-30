#!/bin/bash
#
# Run tests inside the separately created docker container for different hardware architecture
#
# Strategy:
# * Create docker image
# * Enter image
# * Cross build tests
# * Run tests via QEMU
#

set -e

usage()
{
  echo "Usage: run-tests.sh <architecture>"
  echo "Architecture: armhf|i386|powerpc|riscv64|s390x"
  echo "(run from project root)"
}

ARCHLIST="armhf i386 powerpc riscv64 s390x"

if [[ " $ARCHLIST " =~ " $1 " ]] ; then
  ARCH=$1
else
  echo "Unsupported architecture: $1"
  usage
  exit 1
fi

if [ "$2" = "" ] ; then
  echo "Creating docker image..."
  docker build -t $ARCH .devcontainer/$ARCH

  echo "Entering container..."
  docker run -it --rm -v "$PWD":/workspaces/etl -w /workspaces/etl $ARCH /bin/bash .devcontainer/run-tests.sh $ARCH inside_container

elif [ "$2" = "inside_container" ] ; then
  echo "Cross building tests..."
  mkdir -p build-$ARCH
  cd build-$ARCH
  cmake -DCMAKE_TOOLCHAIN_FILE=../.devcontainer/$ARCH/toolchain-$ARCH.cmake \
        -DBUILD_TESTS=ON -DNO_STL=ON -DETL_CXX_STANDARD=23 \
        -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=-O0 -DETL_ENABLE_SANITIZER=OFF -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF \
        ../test
  export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)
  cmake --build .

  echo "Running tests via CTest (using QEMU emulator from toolchain)..."
  ctest -V --output-on-failure
  echo "Tests successful."
else
  echo "Invalid second argument: $2"
  usage
  exit 1
fi

#!/bin/bash
#
# Execute from project root directory
#

set -euo pipefail

if [[ ! -f CMakeLists.txt || ! -d .git ]]; then
  echo "Error: must be run from the project root directory" >&2
  exit 1
fi

export CMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)
export CMAKE_BUILD_PARALLEL_LEVEL

cd test/syntax_check
rm -rf build-clang-tidy
mkdir -p build-clang-tidy
cd build-clang-tidy

CC=clang CXX=clang++ cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF \
      -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=17 ..
cmake --build .

cd ../../..

run-clang-tidy -j "$(nproc)" -p test/syntax_check/build-clang-tidy
# TODO: Add -warnings-as-errors='*' once the code is clean of warnings.

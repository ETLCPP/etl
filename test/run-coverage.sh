#!/bin/bash
#
# run-coverage.sh [gcc|clang]
#
# defaults:
#  compiler: clang
#

set -e

export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc)

# Choose gcc or clang via cmdline
if [ "$1" = "gcc" ] ; then
  COMPILER=gcc
else
  # default
  COMPILER=clang
fi

if [ "$COMPILER" = "gcc" ]; then
  C_COMPILER=gcc
  CXX_COMPILER=g++
  GCOV="$(command -v gcov)" || { echo "gcov not found in PATH"; exit 1; }
  GCOV_ADD=""
  EXTRA_LINK_OPTIONS=""
  EXTRA_LINK_LIBS="gcov"
elif [ "$COMPILER" = "clang" ]; then
  C_COMPILER=clang
  CXX_COMPILER=clang++
  GCOV="$(command -v llvm-cov)" || { echo "llvm-cov not found in PATH"; exit 1; }
  GCOV_ADD="--gcov-tool gcov"
  EXTRA_LINK_OPTIONS="--coverage"
  EXTRA_LINK_LIBS=""
else
  echo "Unsupported compiler: $COMPILER"
  exit 1
fi

BUILD=build-coverage

rm -rf -- "${BUILD:?}"
mkdir -p "$BUILD"
cd "$BUILD" || exit 1
touch total.info

for CXXSTD in 11 14 17 20 23 26; do
  for NOSTL in OFF ON; do
    rm -rf CMakeFiles
    cmake -DEXTRA_COMPILE_OPTIONS="--coverage" \
          -DEXTRA_LINK_OPTIONS="$EXTRA_LINK_OPTIONS" \
          -DEXTRA_LINK_LIBS="$EXTRA_LINK_LIBS" \
          -DCMAKE_C_COMPILER=$C_COMPILER \
          -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
          -DNO_STL=$NOSTL \
          -DETL_USE_TYPE_TRAITS_BUILTINS=OFF \
          -DETL_USER_DEFINED_TYPE_TRAITS=OFF \
          -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF \
          -DETL_OPTIMISATION=-O0 \
          -DETL_CXX_STANDARD=$CXXSTD \
          -DETL_ENABLE_SANITIZER=OFF \
          -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF \
          -DETL_USE_BUILTIN_MEM_FUNCTIONS=ON ..
    cmake --build .
    ./etl_tests
    lcov --gcov-tool "$GCOV" $GCOV_ADD --capture --directory CMakeFiles/etl_tests.dir \
         --rc "geninfo_unexecuted_blocks=1" --output-file coverage.info --include '*/include/etl/*' --rc "lcov_branch_coverage=1" \
         --ignore-errors inconsistent \
         --ignore-errors mismatch

    lcov -a total.info -a coverage.info -o total.info --rc "lcov_branch_coverage=1" \
         --ignore-errors inconsistent \
         --ignore-errors corrupt \
         --ignore-errors empty
  done
done

genhtml total.info --output-directory coverage --rc "genhtml_branch_coverage=1" --branch-coverage -t $COMPILER \
        --ignore-errors inconsistent \
        --ignore-errors category

cd ..

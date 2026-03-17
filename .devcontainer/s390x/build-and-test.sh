#!/bin/bash

# ETL s390x Big-Endian Build and Test Script
# This script verifies the environment is big-endian, builds the ETL tests, and runs them

set -e  # Exit on error

echo "=========================================="
echo "ETL s390x Big-Endian Test Environment"
echo "=========================================="
echo ""

# Verify architecture
echo "=== System Architecture ==="
uname -m
echo ""

# Verify endianness - this is critical for Manchester encoding tests
echo "=== Endianness Verification ==="
BYTE_ORDER=$(lscpu | grep "Byte Order" | awk '{print $3, $4}')
echo "Byte Order: $BYTE_ORDER"

if [[ "$BYTE_ORDER" != "Big Endian" ]]; then
    echo "ERROR: Expected Big Endian but found: $BYTE_ORDER"
    echo "Manchester encoding tests require big-endian architecture!"
    exit 1
fi
echo "✓ Big-endian confirmed"
echo ""

# Navigate to project root
cd /workspaces/etl

# Create build directory
echo "=== Creating Build Directory ==="
rm -rf build-s390x
mkdir -p build-s390x
cd build-s390x
echo ""

# Configure with CMake
echo "=== Configuring CMake ==="
cmake -DBUILD_TESTS=ON \
      -DNO_STL=OFF \
      -DETL_CXX_STANDARD=17 \
      /workspaces/etl/test
echo ""

# Build tests
echo "=== Building Tests ==="
make -j$(nproc)
echo ""

# Run all tests
echo "=== Running ETL Test Suite ==="
echo "This includes Manchester encoding tests which verify big-endian handling"
echo ""
./etl_tests

# Capture test result
TEST_RESULT=$?

echo ""
echo "=========================================="
if [ $TEST_RESULT -eq 0 ]; then
    echo "✓ All tests PASSED on s390x big-endian!"
else
    echo "✗ Tests FAILED with exit code: $TEST_RESULT"
fi
echo "=========================================="

exit $TEST_RESULT

#!/bin/bash

# ETL s390x Big-Endian Build and Test Script
# Cross-compiles for s390x and runs tests under QEMU emulation

set -e  # Exit on error

echo "=========================================="
echo "ETL s390x Big-Endian Test Environment"
echo "=========================================="
echo ""

# Verify host architecture
echo "=== Host Architecture ==="
echo "Host: $(uname -m)"
echo ""

# Verify cross-compilation tools
echo "=== Cross-Compilation Tools ==="
s390x-linux-gnu-gcc --version | head -n1
s390x-linux-gnu-g++ --version | head -n1
echo ""

# Verify QEMU availability
echo "=== QEMU s390x Emulator ==="
qemu-s390x-static --version | head -n1
echo ""

# Navigate to project root
cd /workspaces/etl

# Create build directory
echo "=== Creating Build Directory ==="
rm -rf build-s390x
mkdir -p build-s390x
cd build-s390x
echo ""

# Configure with CMake using s390x toolchain
echo "=== Configuring CMake for s390x Cross-Compilation ==="
cmake -DCMAKE_TOOLCHAIN_FILE=/workspaces/etl/.devcontainer/s390x/toolchain-s390x.cmake \
      -DBUILD_TESTS=ON \
      -DNO_STL=OFF \
      -DETL_CXX_STANDARD=17 \
      /workspaces/etl/test
echo ""

# Build tests
echo "=== Building Tests for s390x ==="
make -j$(nproc)
echo ""

# Verify the binary is s390x
echo "=== Verifying Binary Architecture ==="
TEST_BINARY="./etl_tests"
if [ -f "$TEST_BINARY" ]; then
    FILE_OUTPUT=$(file "$TEST_BINARY")
    echo "$FILE_OUTPUT"
    
    if echo "$FILE_OUTPUT" | grep -qi "S/390\|s390"; then
        echo "✓ Binary is s390x architecture"
    else
        echo "✗ ERROR: Binary is not s390x!"
        exit 1
    fi
    
    if echo "$FILE_OUTPUT" | grep -q "MSB"; then
        echo "✓ Binary is big-endian (MSB)"
    else
        echo "⚠ Warning: Could not confirm big-endian from file output"
    fi
else
    echo "✗ ERROR: Test binary not found!"
    exit 1
fi
echo ""

# Run all tests using QEMU
echo "=== Running ETL Test Suite under QEMU s390x Emulation ==="
echo ""
qemu-s390x-static "$TEST_BINARY"

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

exit $TEST_RESULT

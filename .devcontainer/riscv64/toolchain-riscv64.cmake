# CMake toolchain file for riscv64 cross-compilation
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

# Specify the cross compiler
set(CMAKE_C_COMPILER riscv64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER riscv64-linux-gnu-g++)
set(CMAKE_AR riscv64-linux-gnu-ar)
set(CMAKE_RANLIB riscv64-linux-gnu-ranlib)
set(CMAKE_STRIP riscv64-linux-gnu-strip)
# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set QEMU for running tests
set(CMAKE_CROSSCOMPILING_EMULATOR /usr/bin/qemu-riscv64-static CACHE FILEPATH "Path to the emulator for cross-compiled binaries")

# CMake toolchain file for powerpc cross-compilation
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR powerpc)

# Specify the cross compiler
set(CMAKE_C_COMPILER powerpc-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER powerpc-linux-gnu-g++)
set(CMAKE_AR powerpc-linux-gnu-ar)
set(CMAKE_RANLIB powerpc-linux-gnu-ranlib)
set(CMAKE_STRIP powerpc-linux-gnu-strip)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set QEMU for running tests
set(CMAKE_CROSSCOMPILING_EMULATOR /usr/bin/qemu-ppc CACHE FILEPATH "Path to the emulator for cross-compiled binaries")

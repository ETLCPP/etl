# CMake toolchain file for s390x cross-compilation
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR s390x)

# Specify the cross compiler
set(CMAKE_C_COMPILER s390x-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER s390x-linux-gnu-g++)
set(CMAKE_AR s390x-linux-gnu-ar)
set(CMAKE_RANLIB s390x-linux-gnu-ranlib)
set(CMAKE_STRIP s390x-linux-gnu-strip)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set QEMU for running tests
set(CMAKE_CROSSCOMPILING_EMULATOR /usr/bin/qemu-s390x-static CACHE FILEPATH "Path to the emulator for cross-compiled binaries")

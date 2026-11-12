"""Smoke-test optional CMake generation. Run with Python 3, CMake and a C++ compiler."""

from pathlib import Path
import shutil
import subprocess
import tempfile


root = Path(__file__).resolve().parent.parent


def run(*args, cwd=None):
    subprocess.run([str(arg) for arg in args], cwd=cwd, check=True)


with tempfile.TemporaryDirectory(prefix="etl cmake ") as temporary:
    work = Path(temporary)
    # Generation updates source headers, so test a disposable copy.
    checkout = work / "etl"
    checkout.mkdir()
    for directory in ("include", "cmake"):
        shutil.copytree(root / directory, checkout / directory)
    for filename in ("CMakeLists.txt", "version.txt"):
        shutil.copy2(root / filename, checkout / filename)
    build = work / "build"
    build.mkdir()
    # Ordinary users must not need Python or a virtual environment.
    run("cmake", checkout, "-DCMAKE_DISABLE_FIND_PACKAGE_Python3=TRUE",
        "-DCMAKE_DISABLE_FIND_PACKAGE_PythonInterp=TRUE", cwd=build)
    assert not (build / "generated").exists(), "Default configuration generated files"

    source = work / "consumer"
    source.mkdir()
    messages = ", ".join("Message<{}>".format(index) for index in range(32))
    (source / "main.cpp").write_text('''
#include <etl/message_packet.h>
#include <etl/message_router.h>
#include <etl/fsm.h>

template <int ID> struct Message : etl::message<ID> {};
struct Router;
struct Context;
struct State;
typedef etl::message_packet<@MESSAGES@ > Packet;
typedef etl::message_router<Router, @MESSAGES@ > RouterBase;
typedef etl::fsm_state<Context, State, 0, @MESSAGES@ > StateBase;
ETL_STATIC_ASSERT((ETL_USING_CPP11 == 0), "Test the generated C++03 implementation");
int main()
{
    Message<31> message;
    Packet packet(message);
    return (packet.is_valid() && sizeof(RouterBase) && sizeof(StateBase)) ? 0 : 1;
}
'''.replace("@MESSAGES@", messages), encoding="utf-8")
    (source / "CMakeLists.txt").write_text('''
cmake_minimum_required(VERSION 3.10)
project(generation_consumer LANGUAGES CXX)
set(ETL_NUMBER_OF_HANDLERS 32)
add_subdirectory("@ROOT@" etl-build)
add_executable(consumer main.cpp)
set_target_properties(consumer PROPERTIES CXX_STANDARD 98 CXX_STANDARD_REQUIRED YES)
if(MSVC)
    target_compile_definitions(consumer PRIVATE ETL_CPP11_SUPPORTED=0 ETL_CPP14_SUPPORTED=0
        ETL_CPP17_SUPPORTED=0 ETL_CPP20_SUPPORTED=0 ETL_CPP23_SUPPORTED=0 ETL_CPP26_SUPPORTED=0)
endif()
target_link_libraries(consumer PRIVATE etl::etl)
enable_testing()
add_test(NAME consumer COMMAND consumer)
'''.replace("@ROOT@", checkout.as_posix()), encoding="utf-8")
    consumer_build = work / "consumer build"
    consumer_build.mkdir()
    run("cmake", source, cwd=consumer_build)
    run("cmake", "--build", consumer_build, "--config", "Debug")
    run("ctest", "-C", "Debug", "--output-on-failure", cwd=consumer_build)

print("CMake generation smoke test passed")

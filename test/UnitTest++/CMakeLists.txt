cmake_minimum_required(VERSION 3.5.0)
project(UnitTestpp LANGUAGES CXX)

add_library(UnitTestpp
        AssertException.cpp
        Checks.cpp
        CompositeTestReporter.cpp
        CurrentTest.cpp
        DeferredTestReporter.cpp
        DeferredTestResult.cpp
        MemoryOutStream.cpp
        ReportAssert.cpp
        RequiredCheckException.cpp
        RequiredCheckTestReporter.cpp
        Test.cpp
        TestDetails.cpp
        TestList.cpp
        TestReporter.cpp
        TestReporterStdout.cpp
        TestResults.cpp
        TestRunner.cpp
        ThrowingTestReporter.cpp
        TimeConstraint.cpp
        XmlTestReporter.cpp
        )

target_include_directories(UnitTestpp SYSTEM INTERFACE ..)

if (WIN32)
    target_sources(UnitTestpp PRIVATE Win32/TimeHelpers.cpp)
else ()
    target_sources(UnitTestpp PRIVATE
            Posix/SignalTranslator.cpp
            Posix/TimeHelpers.cpp
            )
endif ()

if ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    target_compile_options(UnitTestpp PRIVATE -fexceptions)
endif ()

if (UNIX AND NOT APPLE)
    # atomic is need on Linux with Clang
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(UnitTestpp PRIVATE atomic Threads::Threads)
elseif (NOT UNIX AND APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(UnitTestpp PRIVATE Threads::Threads)
endif ()

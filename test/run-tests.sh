#!/bin/sh
clear

mkdir -p build-make || exit 1
cd build-make || exit 1

echo "ETL Tests" > log.txt

export ASAN_OPTIONS=symbol_line=1
export ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-14/bin//llvm-symbolizer

echo -e

configuration_name="Configuration Name Not Set"

FailColour='\033[38;2;255;128;128m'
PassColour='\033[38;2;128;255;128m'
TitleColour='\033[38;2;107;210;255m'
HelpColour='\033[38;2;250;180;250m'
NoColour='\033[0m'

ParseGitBranch() 
{
	git rev-parse --abbrev-ref HEAD
}

SetConfigurationName()
{
	configuration_name=$1
}

Bell()
{
	echo -n $'\a'
}

PrintHeader()
{
	echo "$TitleColour"
	echo "============================================================================" | tee -a log.txt
	echo " Configuration   : $configuration_name" | tee -a log.txt
	echo " Compiler        : $compiler          " | tee -a log.txt
	echo " Language        : C++$cxx_standard   " | tee -a log.txt
    echo " Optimisation    : $opt               " | tee -a log.txt
	echo " Sanitizer       : $sanitize          " | tee -a log.txt
	echo " Compiler select : $compiler_enabled  " | tee -a log.txt
	echo " ETL version     : $etl_version       " | tee -a log.txt
	echo " Git branch      : $(ParseGitBranch)  " | tee -a log.txt
	echo " Processes       : ${CMAKE_BUILD_PARALLEL_LEVEL}" | tee -a log.txt
	echo "============================================================================" | tee -a log.txt
	echo "$NoColour"
}

PrintHelp()
{
	echo "$HelpColour"
	echo "-------------------------------------------------------------------------------------"
	echo " Syntax          : ./runtests.sh <C++ Standard> <Optimisation> <Threads> <Sanitizer> "
	echo " C++ Standard    : 11, 14, 17 or 20                                                  "
	echo " Optimisation    : 0, 1, 2 or 3. Default = 0                                         "
	echo " Threads         : Number of threads to use. Default = 4                             "
	echo " Sanitizer       : s enables sanitizer checks, n disables. Default disabled         "
	echo " Compiler select : gcc or clang. Default All compilers                               "
	echo "-------------------------------------------------------------------------------------"
	echo "$NoColour"
}

PassedCompilation()
{
	echo "$PassColour"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " Compilation Success - $configuration_name" | tee -a log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "$NoColour"
}

PassedTests()
{
	echo "$PassColour"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " Passed Tests - $configuration_name" | tee -a log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "$NoColour"
}

FailedCompilation()
{
	echo "$FailColour"
	echo "****************************************************************************" | tee -a log.txt
	echo "**** Failed Compilation - $configuration_name" | tee -a log.txt
	echo "****************************************************************************" | tee -a log.txt
	echo "$NoColour"
	Bell
}

FailedTests()
{
	echo "$FailColour"
	echo "****************************************************************************" | tee -a log.txt
	echo "**** Failed Tests - $configuration_name" | tee -a log.txt
	echo "****************************************************************************" | tee -a log.txt
	echo "$NoColour"
	Bell
}

TestsCompleted()
{
	echo "$PassColour"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " All Tests Completed OK                        " | tee -a log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "$NoColour"
}

#******************************************************************************
# Set the language standard.
#******************************************************************************
if [ "$1" = "11" ]; then
  cxx_standard="11"
elif [ "$1" = "14" ]; then
  cxx_standard="14"
elif [ "$1" = "17" ]; then
  cxx_standard="17"
elif [ "$1" = "20" ]; then
  cxx_standard="20"
else
  PrintHelp
  exit
fi

#******************************************************************************
# Set the optimisation level. Default -O0
#******************************************************************************
if [ $# -eq 1 ]; then
  opt="-O0"
elif [ "$2" = "1" ]; then
  opt="-O1"
elif [ "$2" = "2" ]; then
  opt="-O2"
elif [ "$2" = "3" ]; then
  opt="-O3" 
else
  opt="-O0"
fi

#******************************************************************************
# Set the number of concurrent processes to use. Default 4
#******************************************************************************
if [ $# -le 2 ]; then
  export CMAKE_BUILD_PARALLEL_LEVEL=4
else
  export CMAKE_BUILD_PARALLEL_LEVEL=$3
fi

#******************************************************************************
# Set the sanitizer enable. Default OFF
#******************************************************************************
if [ "$4" = "s" ]; then
  sanitize="On"
elif [ "$4" = "n" ]; then
  sanitize="Off"
else
  sanitize="Off"
fi

#******************************************************************************
# Set the compiler enable. Default GCC and Clang
#******************************************************************************
if [ "$5" = "gcc" ]; then
  compiler_enabled="gcc"
elif [ "$5" = "clang" ]; then
  compiler_enabled="clang"
else
  compiler_enabled="All compilers"
fi

#******************************************************************************
# Get the ETL version
#******************************************************************************
etl_version_raw=$(cat ../../version.txt)
etl_version=$(echo $etl_version_raw | sed -e 's/\r//g') # Remove trailing \r

#******************************************************************************
# Get the compiler versions
#******************************************************************************
gcc_compiler=$(g++ --version | grep g++)
clang_compiler=$(clang++ --version | grep clang)

#******************************************************************************
# GCC
#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "STL"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "STL - Non-virtual messages"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=ON ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "STL - Force C++03"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "No STL"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "No STL - Force C++03"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
# CLANG
#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "STL"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "STL - Force C++03"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "No STL"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "No STL - Force C++03"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "Initializer list test"
PrintHeader
cd ../etl_initializer_list/
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "Initializer list test"
PrintHeader
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "Error macros 'log_errors' test"
PrintHeader
cd ../../etl_error_handler/log_errors
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "Error macros 'exceptions' test"
PrintHeader
cd ../../../etl_error_handler/exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "Error macros 'log_errors and exceptions' test"
PrintHeader
cd ../../../etl_error_handler/log_errors_and_exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$gcc_compiler
SetConfigurationName "Error macros 'assert function' test"
PrintHeader
cd ../../../etl_error_handler/assert_function
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="gcc" -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "Error macros 'log_errors' test"
PrintHeader
cd ../../../etl_error_handler/log_errors
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "Error macros 'exceptions' test"
PrintHeader
cd ../../../etl_error_handler/exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "Error macros 'log_errors and exceptions' test"
PrintHeader
cd ../../../etl_error_handler/log_errors_and_exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

#******************************************************************************
if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
compiler=$clang_compiler
SetConfigurationName "Error macros 'assert function' test"
PrintHeader
cd ../../../etl_error_handler/assert_function
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
cmake -DCMAKE_C_COMPILER="clang" -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize ..
cmake --build .
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  PassedTests
else
  FailedTests
  exit $?
fi
fi

cd ../..

TestsCompleted

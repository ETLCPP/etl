#!/bin/bash

shopt -s xpg_echo

set -e

clear

export ASAN_OPTIONS=symbol_line=1
export ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-14/bin//llvm-symbolizer

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
	echo " Running tests                        " | tee -a log.txt
	echo " Configuration   : $configuration_name" | tee -a log.txt
	echo " Compiler        : $compiler          " | tee -a log.txt
	echo " Language        : C++$cxx_standard   " | tee -a log.txt
	echo " Optimisation    : $opt               " | tee -a log.txt
	echo " Sanitizer       : $sanitize          " | tee -a log.txt
	echo " Compiler select : $compiler_enabled  " | tee -a log.txt
	echo " Verbose         : $verbose           " | tee -a log.txt
	echo " ETL version     : $etl_version       " | tee -a log.txt
	echo " Git branch      : $(ParseGitBranch)  " | tee -a log.txt
	echo " Processes       : ${CMAKE_BUILD_PARALLEL_LEVEL}" | tee -a log.txt
	echo "============================================================================" | tee -a log.txt
	echo "$NoColour"
}

PrintHelp()
{
	echo "$HelpColour"
	echo "----------------------------------------------------------------------------------------------------------"
	echo " Syntax          : ./run-tests.sh <C++ Standard> <Optimisation> <Threads> <Sanitizer> <Compiler> <Verbose>"
	echo " C++ Standard    : 11, 14, 17, 20, 23, 26 or all                                                          "
	echo " Optimisation    : 0, 1, 2 or 3. Default = 0                                                              "
	echo " Threads         : Number of threads to use. Default = 4                                                  "
	echo " Sanitizer       : s enables sanitizer checks, n disables. Default disabled                               "
	echo " Compiler select : gcc or clang. Default All compilers                                                    "
	echo " Verbose         : v enables verbose log, n disables. Default disabled                                    "
	echo "----------------------------------------------------------------------------------------------------------"
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
  cxx_standards="11"
elif [ "$1" = "14" ]; then
  cxx_standards="14"
elif [ "$1" = "17" ]; then
  cxx_standards="17"
elif [ "$1" = "20" ]; then
  cxx_standards="20"
elif [ "$1" = "23" ]; then
  cxx_standards="23"
elif [ "$1" = "26" ]; then
  cxx_standards="26"
elif [ "$1" = "all" ]; then
  cxx_standards="11 14 17 20 23 26"
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
  sanitize="ON"
elif [ "$4" = "n" ]; then
  sanitize="OFF"
else
  sanitize="OFF"
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
# Set the verbose enable. Default OFF
#******************************************************************************
if [ "$6" = "v" ]; then
  verbose="On"
  verbose_cmake_flag="-DEXTRA_TESTING_FLAGS=-v"
else
  verbose="Off"
  verbose_cmake_flag=""
fi

#******************************************************************************
# Get the ETL version
#******************************************************************************
etl_version_raw=$(cat ../version.txt)
etl_version=$(echo $etl_version_raw | sed -e 's/\r//g') # Remove trailing \r

#******************************************************************************
# Get the compiler versions
#******************************************************************************

for cxx_standard in $cxx_standards ; do
  while read i ; do
    CC=`echo $i | cut -d, -f1 | sed -e 's/ *$//'`
    MSG=`echo $i | cut -d, -f2 | sed -e 's/ *$//'`
    DIR=`echo $i | cut -d, -f3 | sed -e 's/ *$//'`
    CMD=`echo $i | cut -d, -f4 | sed -e 's/ *$//'`

    if [ "$compiler_enabled" = "$CC" ] || [ "$compiler_enabled" = "All compilers" ]; then
      if [ "$CC" = "gcc" ] ; then
        compiler=$(g++ --version | grep g++)
      else
        compiler=$(clang++ --version | grep clang)
      fi
      OLD_DIR=`pwd`
      cd $DIR
      mkdir -p build-make || exit 1
      cd build-make || exit 1
      echo "ETL Tests" > log.txt
      SetConfigurationName "$MSG"
      PrintHeader
      $CMD
      if cmake --build .; then
        PassedCompilation
      else
        FailedCompilation
        exit 1
      fi
      if ctest -V; then
        PassedTests
      else
        FailedTests
        exit 1
      fi
      cd ..
      rm -rf build-make
      cd $OLD_DIR
    fi
  done <<-EOF
gcc  ,STL                       ,.,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
gcc  ,STL - Non-virtual messages,.,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=ON $verbose_cmake_flag ..
gcc  ,STL - Force C++03         ,.,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON  -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
gcc  ,No STL                    ,.,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DNO_STL=ON  -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
gcc  ,No STL - Force C++03      ,.,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DNO_STL=ON  -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON  -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
gcc  ,No STL - Builtin mem functions ,.,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DNO_STL=ON  -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF  -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF -DETL_USE_BUILTIN_MEM_FUNCTIONS=ON $verbose_cmake_flag ..
clang,STL                       ,.,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
clang,STL - Force C++03         ,.,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON  -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
clang,No STL                    ,.,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DNO_STL=ON  -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
clang,No STL - Force C++03      ,.,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DNO_STL=ON  -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON  -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF $verbose_cmake_flag ..
clang,No STL - Builtin mem functions ,.,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DNO_STL=ON  -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF  -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize -DETL_MESSAGES_ARE_NOT_VIRTUAL=OFF -DETL_USE_BUILTIN_MEM_FUNCTIONS=ON $verbose_cmake_flag ..
gcc  ,Initializer list test     ,etl_initializer_list,cmake -DCMAKE_C_COMPILER=gcc   -DCMAKE_CXX_COMPILER=g++     -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
clang,Initializer list test     ,etl_initializer_list,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
gcc  ,Error macros 'log_errors' test,etl_error_handler/log_errors                              ,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
gcc  ,Error macros 'exceptions' test,etl_error_handler/exceptions                              ,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
gcc  ,Error macros 'log_errors and exceptions' test,etl_error_handler/log_errors_and_exceptions,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
gcc  ,Error macros 'assert function' test,etl_error_handler/assert_function                    ,cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
clang,Error macros 'log_errors' test,etl_error_handler/log_errors                              ,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
clang,Error macros 'exceptions' test,etl_error_handler/exceptions                              ,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
clang,Error macros 'log_errors and exceptions' test,etl_error_handler/log_errors_and_exceptions,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
clang,Error macros 'assert function' test,etl_error_handler/assert_function                    ,cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard -DETL_ENABLE_SANITIZER=$sanitize $verbose_cmake_flag ..
EOF
done

TestsCompleted

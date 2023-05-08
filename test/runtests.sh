#!/bin/sh
clear

mkdir -p build-make || exit 1
cd build-make || exit 1

echo "ETL Tests" > log.txt

export ASAN_OPTIONS=symbol_line=1
export ASAN_SYMBOLIZER_PATH=/usr/lib/llvm-14/bin//llvm-symbolizer

echo -e

testname="Test Name Not Set"

FailColour='\033[38;2;255;128;128m'
PassColour='\033[38;2;128;255;128m'
TitleColour='\033[38;2;100;173;254m'
HelpColour='\033[38;2;250;180;250m'
NoColour='\033[0m'

SetTestName()
{
	testname=$1
}

Bell()
{
	echo -n $'\a'
}

PrintHeader()
{
	echo "${TitleColour}"
	echo "----------------------------------------------------------------------------" | tee -a log.txt
	echo " $testname" | tee -a log.txt
	echo " Language standard : C++${cxx_standard}" | tee -a log.txt
    echo " Optimisation      : ${opt}" | tee -a log.txt
	echo "----------------------------------------------------------------------------" | tee -a log.txt
	echo "${NoColour}"
}

PrintHelp()
{
	echo "${HelpColour}"
	echo "----------------------------------------------------------------------------"
	echo " Syntax       : ./runtests.sh <C++ Standard> <Optimisation>"
	echo " C++ Standard : 11, 14, 17 or 20"
	echo " Optimisation : 0, 1, 2 or 3. Default = 0"
	echo "----------------------------------------------------------------------------"
	echo "${NoColour}"
}

PassedCompilation()
{
	echo "${PassColour}"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " Passed Compilation - $testname" | tee -a ../log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "${NoColour}"
}

PassedTests()
{
	echo "${PassColour}"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " Passed Tests - $testname" | tee -a ../log.txt
	echo "-----------------------------------------------" | tee -a log.txt
}

FailedCompilation()
{
	echo "${FailColour}"
	echo "****************************************************************************" | tee -a log.txt
    echo "**** Failed Compilation $testname" | tee -a log.txt
	echo "****************************************************************************" | tee -a ../log.txt
	echo "${NoColour}"
	Bell
}

FailedTests()
{
	echo "${FailColour}"
	echo "****************************************************************************" | tee -a log.txt
    echo "**** Failed Tests $testname" | tee -a log.txt
	echo "****************************************************************************" | tee -a ../log.txt
	echo "${NoColour}"
	Bell
}

TestsCompleted()
{
	echo "${PassColour}"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " All Tests Completed OK" | tee -a log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "${NoColour}"
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
if [ "$2" = "1" ]; then
  opt="-O1"
elif [ "$2" = "2" ]; then
  opt="-O2"
elif [ "$2" = "3" ]; then
  opt="-O3" 
else
  opt="-O0"
fi

#******************************************************************************
# GCC
#******************************************************************************
SetTestName "GCC - STL"
PrintHeader
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "GCC - STL - Force C++03"
PrintHeader
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "GCC - No STL"
PrintHeader
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "GCC - No STL - Force C++03"
PrintHeader
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
# CLANG
#******************************************************************************
SetTestName "Clang - STL"
PrintHeader
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "Clang - STL - Force C++03"
PrintHeader
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "Clang - No STL"
PrintHeader
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "Clang - No STL - Force C++03"
PrintHeader
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "GCC - Initializer list test"
PrintHeader
cd ../etl_initializer_list/
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "Clang - Initializer list test"
PrintHeader
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "GCC - Error macros 'log_errors' test"
PrintHeader
cd ../../etl_error_handler/log_errors
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "GCC - Error macros 'exceptions' test"
PrintHeader
cd ../../../etl_error_handler/exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "GCC - Error macros 'log_errors and exceptions' test"
PrintHeader
cd ../../../etl_error_handler/log_errors_and_exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
gcc --version | grep gcc | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="g++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "Clang - Error macros 'log_errors' test"
PrintHeader
cd ../../../etl_error_handler/log_errors
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "Clang - Error macros 'exceptions' test"
PrintHeader
cd ../../../etl_error_handler/exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

#******************************************************************************
SetTestName "Clang - Error macros 'log_errors and exceptions' test"
PrintHeader
cd ../../../etl_error_handler/log_errors_and_exceptions
mkdir -p build-make || exit 1
cd build-make || exit 1
rm * -rf
clang --version | grep clang | tee -a log.txt
cmake -DCMAKE_CXX_COMPILER="clang++" -DETL_OPTIMISATION=$opt -DETL_CXX_STANDARD=$cxx_standard ..
make -j4
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

cd ../..

TestsCompleted

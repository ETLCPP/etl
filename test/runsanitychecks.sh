#!/bin/sh
clear

echo -e

testname="Test Name Not Set"

FailColour='\033[38;2;255;128;128m'
PassColour='\033[38;2;128;255;128m'
TitleColour='\033[38;2;100;173;254m'
NoColour='\033[0m'

SetCxxStandard()
{
	cxx_standard=$1
}

SetTestName()
{
	testname=$1
}

PrintHeader()
{
	echo "${TitleColour}"
	echo "----------------------------------------------------------------------------" | tee -a log.txt
	echo " $testname" | tee -a log.txt
	echo " Language standard : C++${cxx_standard}" | tee -a log.txt
	echo "----------------------------------------------------------------------------" | tee -a log.txt
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

FailedCompilation()
{
	echo "${FailColour}"
	echo "****************************************************************************" | tee -a log.txt
    echo "**** Failed Compilation $testname" | tee -a log.txt
	echo "****************************************************************************" | tee -a ../log.txt
	echo "${NoColour}"
}

ChecksCompleted()
{
	echo "${PassColour}"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " All Checks Completed OK" | tee -a log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "${NoColour}"
}

cd sanity-check || exit 1
echo "" > log.txt

export CMAKE_BUILD_PARALLEL_LEVEL=4

echo ""
echo "Using "${CMAKE_BUILD_PARALLEL_LEVEL}" jobs"

###############################################################################
cd c++03 || exit 1

SetCxxStandard "03 (98)"

SetTestName "GCC - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

##SetTestName "GCC - No STL - User defined traits"
# PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetTestName "Clang - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetTestName "Clang - No STL - User defined traits"
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

###############################################################################
cd ../c++11 || exit 1

SetCxxStandard "11"

SetTestName "GCC - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - No STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetTestName "GCC - No STL - User defined traits"
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetTestName  "Clang - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - No STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetTestName "Clang - No STL - User defined traits"
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

###############################################################################
cd ../c++14 || exit 1

SetCxxStandard "14"

SetTestName "GCC - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - No STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetTestName "GCC - No STL - User defined traits"
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetTestName "Clang - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - No STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetTestName "Clang - No STL - User defined traits"
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

###############################################################################
cd ../c++17 || exit 1

SetCxxStandard "17"

SetTestName "GCC - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "GCC - No STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetTestName "GCC - No STL - User defined traits"
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetTestName "Clang - STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - No STL"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetTestName "Clang - No STL - Force C++03"
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetTestName "Clang - No STL - User defined traits"
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

ChecksCompleted

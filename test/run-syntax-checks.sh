#!/bin/sh
clear

echo -e

configuration_name="Configuration Name Not Set"

FailColour='\033[38;2;255;128;128m'
PassColour='\033[38;2;128;255;128m'
TitleColour='\033[38;2;107;210;255m'
NoColour='\033[0m'

ParseGitBranch() 
{
    git rev-parse --abbrev-ref HEAD
}

SetCxxStandard()
{
	cxx_standard=$1
}

SetConfigurationName()
{
	configuration_name=$1
}

PrintHeader()
{
	echo "$TitleColour"
	echo "============================================================================" | tee -a log.txt
	echo " Configuration     : $configuration_name" | tee -a log.txt
	echo " Compiler          : $compiler          " | tee -a log.txt
	echo " Language standard : C++$cxx_standard   " | tee -a log.txt
	echo " ETL version       : $etl_version       " | tee -a log.txt
	echo " Git branch        : $(ParseGitBranch)  " | tee -a log.txt
	echo " Processes         : ${CMAKE_BUILD_PARALLEL_LEVEL}" | tee -a log.txt
	echo "============================================================================" | tee -a log.txt
	echo "$NoColour"
}

PassedCompilation()
{
	echo "$PassColour"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " Passed Compilation - $configuration_name" | tee -a log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "$NoColour"
}

FailedCompilation()
{
	echo "$FailColour"
	echo "****************************************************************************" | tee -a log.txt
    echo "**** Failed Compilation $configuration_name" | tee -a log.txt
	echo "****************************************************************************" | tee -a log.txt
	echo "$NoColour"
}

ChecksCompleted()
{
	echo "$PassColour"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " All Checks Completed OK                       " | tee -a log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "$NoColour"
}

cd syntax_check || exit 1
echo "" > log.txt

#******************************************************************************
# Set the number of concurrent processes to use.
#******************************************************************************
if [ $# -eq 0 ]; then
  export CMAKE_BUILD_PARALLEL_LEVEL=4
else
  export CMAKE_BUILD_PARALLEL_LEVEL=$1
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

###############################################################################
cd c++03 || exit 1

SetCxxStandard "03 (98)"

SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

##SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
# PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

###############################################################################
cd ../c++11 || exit 1

SetCxxStandard "11"

SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName  "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
##CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

###############################################################################
cd ../c++14 || exit 1

SetCxxStandard "14"

SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
##CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

###############################################################################
cd ../c++17 || exit 1

SetCxxStandard "17"

SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

###############################################################################
cd ../c++20 || exit 1

SetCxxStandard "20"

SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi

SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi

ChecksCompleted

#!/bin/sh
clear

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
	echo " Configuration   : $configuration_name" | tee -a log.txt
	echo " Compiler        : $compiler          " | tee -a log.txt
	echo " Language        : C++$cxx_standard   " | tee -a log.txt
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
	echo "----------------------------------------------------------------------------------"
	echo " Syntax       : ./runtests.sh <C++ Standard> <Threads>                            "
	echo " C++ Standard : a, 03, 11, 14, 17 or 20 (a = All standards)                       "
	echo " Threads      : Number of threads to use. Default = 4                             "
	echo " Compiler select : gcc or clang. Default All compilers                            "
	echo "----------------------------------------------------------------------------------"
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
# Set the language standard.
#******************************************************************************
if [ "$1" = "3" ]; then
  requested_cxx_standard="03"
elif [ "$1" = "03" ]; then
  requested_cxx_standard="03"
elif [ "$1" = "11" ]; then
  requested_cxx_standard="11"
elif [ "$1" = "14" ]; then
  requested_cxx_standard="14"
elif [ "$1" = "17" ]; then
  requested_cxx_standard="17"
elif [ "$1" = "20" ]; then
  requested_cxx_standard="20"
elif [ "$1" = "A" ]; then
  requested_cxx_standard="All"
elif [ "$1" = "a" ]; then
  requested_cxx_standard="All"
else
  PrintHelp
fi

#******************************************************************************
# Set the number of concurrent processes to use. Default 4
#******************************************************************************
if [ $# -le 1 ]; then
  export CMAKE_BUILD_PARALLEL_LEVEL=4
else
  export CMAKE_BUILD_PARALLEL_LEVEL=$2
fi

#******************************************************************************
# Set the compiler enable. Default GCC and Clang
#******************************************************************************
if [ "$3" = "gcc" ]; then
  compiler_enabled="gcc"
elif [ "$3" = "clang" ]; then
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

###############################################################################
if [ "$requested_cxx_standard" = "03" ] || [ "$requested_cxx_standard" = "All" ]; then
SetCxxStandard "03 (98)"

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
# PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

fi

###############################################################################
if [ "$requested_cxx_standard" = "11" ] || [ "$requested_cxx_standard" = "All" ]; then

SetCxxStandard "11"

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName  "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
##CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

fi

###############################################################################
if [ "$requested_cxx_standard" =  "14" ] || [ "$requested_cxx_standard" = "All" ]; then
SetCxxStandard "14"

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
##CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

fi

###############################################################################
if [ "$requested_cxx_standard" =  "17" ] || [ "$requested_cxx_standard" = "All" ]; then
SetCxxStandard "17"

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

fi

###############################################################################
if [ "$requested_cxx_standard" =  "20" ] || [ "$requested_cxx_standard" = "All" ]; then
SetCxxStandard "20"

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$gcc_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "gcc" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$gcc_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Force C++03"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bclang
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

#if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
#SetConfigurationName "No STL - User defined traits"
#compiler=$clang_compiler
#PrintHeader
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  PassedCompilation
#else
#  FailedCompilation
#  exit $?
#fi
#fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

if [ "$compiler_enabled" = "clang" ] || [ "$compiler_enabled" = "All compilers" ]; then
SetConfigurationName "No STL - Built-in traits"
compiler=$clang_compiler
PrintHeader
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
CC=clang CXX=clang++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF -DETL_CXX_STANDARD=$cxx_standard ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  PassedCompilation
else
  FailedCompilation
  exit $?
fi
fi

fi

ChecksCompleted

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
TitleColour='\033[38;2;107;210;255m'
HelpColour='\033[38;2;255;128;128m'
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
	echo "$TitleColour"
	echo "============================================================================" | tee -a log.txt
	echo " $testname                                                                  " | tee -a log.txt
	echo " Language standard : C++$cxx_standard                                       " | tee -a log.txt
    echo " Optimisation      : $opt                                                   " | tee -a log.txt
	echo "============================================================================" | tee -a log.txt
	echo "$NoColour"
}

PrintHelp()
{
	echo "$HelpColour"
	echo "----------------------------------------------------------------------------"
	echo " Syntax       : ./run-tests.sh <options>                                    "
	echo " -c : C++ Standard : 11, 14, 17 or 20                                       "
	echo " -o : Optimisation : 0, 1, 2 or 3. Default = 0                              "
	echo " -p : Processes    : Default = 1                                            "
	echo " -s : Sanitizer    : off or on : Default = off                              "
	echo "                                                                            "
	echo " -c option is mandatory.                                                    "
	echo "----------------------------------------------------------------------------"
	echo "$NoColour"
}

PassedCompilation()
{
	echo "$PassColour"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " Passed Compilation - $testname                " | tee -a ../log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "$NoColour"
}

PassedTests()
{
	echo "$PassColour"
	echo "-----------------------------------------------" | tee -a log.txt
	echo " Passed Tests - $testname                      " | tee -a ../log.txt
	echo "-----------------------------------------------" | tee -a log.txt
	echo "$NoColour"
}

FailedCompilation()
{
	echo "$FailColour"
	echo "****************************************************************************" | tee -a log.txt
    echo "**** Failed Compilation $testname                                           " | tee -a log.txt
	echo "****************************************************************************" | tee -a ../log.txt
	echo "$NoColour"
	Bell
}

FailedTests()
{
	echo "$FailColour"
	echo "****************************************************************************" | tee -a log.txt
    echo "**** Failed Tests $testname                                                 " | tee -a log.txt
	echo "****************************************************************************" | tee -a ../log.txt
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
# Parse the options.
#******************************************************************************
while getopts ":c:o:p:s:" opt; do
  case $opt in
    c) c="$OPTARG" ;;
    o) o="$OPTARG" ;;
	p) p="$OPTARG" ;;
	s) s="$OPTARG" ;;
    [?]) echo "Invalid option -$OPTARG" >&2
    exit 1
    ;;
  esac
done

#******************************************************************************
# Set the language standard.
#******************************************************************************
if [ "$c" = "11" ]; then
  cxx_standard="11"
elif [ "$c" = "14" ]; then
  cxx_standard="14"
elif [ "$c" = "17" ]; then
  cxx_standard="17"
elif [ "$c" = "20" ]; then
  cxx_standard="20"
else
  PrintHelp
  exit
fi

#******************************************************************************
# Set the optimisation level. Default -O0
#******************************************************************************
if [ "$o" = "1" ]; then
  opt="-O1"
elif [ "$o" = "2" ]; then
  opt="-O2"
elif [ "$o" = "3" ]; then
  opt="-O3" 
else
  opt="-O0"
fi

#******************************************************************************
# Set the number of concurrent processes to use.
#******************************************************************************
if [ "$p" = "" ]; then
  export CMAKE_BUILD_PARALLEL_LEVEL=1
  processes="1"
else
  export CMAKE_BUILD_PARALLEL_LEVEL=$p
  processes=$p
fi

echo ""
echo "Using "${CMAKE_BUILD_PARALLEL_LEVEL}" concurrent processes"

#******************************************************************************
# Set the sanitizer enable. Default OFF
#******************************************************************************
if [ "$s" = "on" ]; then
  sanitizer="ON"
else
  sanitizer="OFF"
fi

echo cxx_standard = $cxx_standard
echo optimisation = $opt
echo processes    = $processes
echo sanitizer    = $sanitizer

PrintHeader

TestsCompleted

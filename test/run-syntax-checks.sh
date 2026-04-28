#!/bin/bash
clear

echo -e

configuration_name="Configuration Name Not Set"

FailColour=$'\033[38;2;255;128;128m'
PassColour=$'\033[38;2;128;255;128m'
TitleColour=$'\033[38;2;107;210;255m'
HelpColour=$'\033[38;2;250;180;250m'
NoColour=$'\033[0m'

ParseGitBranch() 
{
    git rev-parse --abbrev-ref HEAD
}

SetConfigurationName()
{
	configuration_name=$1 
}

PrintHeader()
{
	echo "$TitleColour"
	echo "============================================================================" | tee -a log.txt
	echo " Running syntax checks                " | tee -a log.txt
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
	echo " Syntax       : ./runtests.sh <C++ Standard> <Threads> <Compiler>                 "
	echo " C++ Standard : a, 03, 11, 14, 17, 20, 23 or 26 (a = All standards)               "
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
elif [ "$1" = "23" ]; then
  requested_cxx_standard="23"
elif [ "$1" = "26" ]; then
  requested_cxx_standard="26"
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

#******************************************************************************
# RunCheck - runs a single syntax check configuration
# Args: CC_NAME, MSG, NO_STL, BUILTINS, USER_DEFINED, FORCE_03, BUILD_DIR
#******************************************************************************
RunCheck()
{
	local cc_name=$1
	local msg=$2
	local no_stl=$3
	local builtins=$4
	local user_defined=$5
	local force_03=$6
	local build_dir=$7

	if [ "$compiler_enabled" != "$cc_name" ] && [ "$compiler_enabled" != "All compilers" ]; then
		return
	fi

	if [ "$cc_name" = "gcc" ]; then
		compiler=$gcc_compiler
		local cc=gcc
		local cxx=g++
	else
		compiler=$clang_compiler
		local cc=clang
		local cxx=clang++
	fi

	SetConfigurationName "$msg"
	PrintHeader
	rm -rdf bgcc
	rm -rdf bclang
	cmake -E make_directory bgcc bclang
	CC=$cc CXX=$cxx cmake -E chdir $build_dir cmake -DNO_STL=$no_stl -DETL_USE_TYPE_TRAITS_BUILTINS=$builtins -DETL_USER_DEFINED_TYPE_TRAITS=$user_defined -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=$force_03 -DETL_CXX_STANDARD=$cxx_standard ..
	cmake --build $build_dir
	if [ $? -eq 0 ]; then
		PassedCompilation
	else
		FailedCompilation
		exit $?
	fi
}

#******************************************************************************
# RunStandard - runs all configurations for a given C++ standard
# Args: standard, has_force_03 (1 = yes, 0 = no)
#******************************************************************************
RunStandard()
{
	local std=$1
	local has_force_03=$2

	cxx_standard=$std

	# GCC configurations
	RunCheck gcc "STL"                   OFF OFF OFF OFF bgcc
	RunCheck gcc "No STL"                ON  OFF OFF OFF bgcc
	RunCheck gcc "STL - Built-in traits" OFF ON  OFF OFF bgcc
	RunCheck gcc "No STL - Built-in traits" ON ON OFF OFF bgcc

	if [ "$has_force_03" = "1" ]; then
		RunCheck gcc "STL - Force C++03"        OFF OFF OFF ON bgcc
		RunCheck gcc "No STL - Force C++03"     ON  OFF OFF ON bgcc
	fi

	# Clang configurations
	RunCheck clang "STL"                   OFF OFF OFF OFF bclang
	RunCheck clang "No STL"                ON  OFF OFF OFF bclang
	RunCheck clang "STL - Built-in traits" OFF ON  OFF OFF bgcc
	RunCheck clang "No STL - Built-in traits" ON ON OFF OFF bgcc

	if [ "$has_force_03" = "1" ]; then
		RunCheck clang "STL - Force C++03"        OFF OFF OFF ON bclang
		RunCheck clang "No STL - Force C++03"     ON  OFF OFF ON bclang
	fi
}

###############################################################################
# Run checks for each requested standard
###############################################################################

for std in 03 11 14 17 20 23 26; do
	if [ "$requested_cxx_standard" = "$std" ] || [ "$requested_cxx_standard" = "All" ]; then
		if [ "$std" = "03" ]; then
			RunStandard "03" 0
		else
			RunStandard "$std" 1
		fi
	fi
done

ChecksCompleted

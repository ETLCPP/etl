#!/bin/sh
clear

cd sanity-check || exit 1
echo "" > log.txt

export CMAKE_BUILD_PARALLEL_LEVEL=4

###############################################################################
cd c++03 || exit 1
{ echo ""
echo "Using "${CMAKE_BUILD_PARALLEL_LEVEL}" jobs"
echo "-----------------------------------------------"
echo " C++03"
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

###############################################################################
cd ../c++11 || exit 1
{ echo ""
echo "-----------------------------------------------"
echo " C++11"
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - STL - Force C++03"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "Clang - STL - Force C++03"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

###############################################################################
cd ../c++14 || exit 1
{ echo ""
echo "-----------------------------------------------"
echo " C++14"
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - STL - Force C++03"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "Clang - STL - Force C++03"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

###############################################################################
cd ../c++17 || exit 1
{ echo ""
echo "-----------------------------------------------"
echo " C++17 "
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - STL - Force C++03"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "GCC - No STL - Force C++03"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bgcc
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bgcc
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "Clang - STL - Force C++03"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
rm -rdf bgcc
rm -rdf bclang
cmake -E make_directory bgcc bclang
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
cmake --build bclang
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#rm -rdf bgcc
#rm -rdf bclang
#cmake -E make_directory bgcc bclang
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
#cmake --build bclang
#if [ $? -eq 0 ]; then
#  echo "Passed"
#else
#  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
#  exit $?
#fi

{ echo ""
echo "-----------------------------------------------"
echo " All Checks Completed OK"
echo "-----------------------------------------------"; } | tee -a ../log.txt

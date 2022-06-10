#!/bin/sh
clear

mkdir -p build-make || exit 1
cd build-make || exit 1

echo "ETL Tests" > log.txt

#******************************************************************************
# GCC
#******************************************************************************
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC - STL" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
gcc --version | grep gcc | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USE_MEM_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC - STL - Force C++03" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
gcc --version | grep gcc | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USE_MEM_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC - No STL" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
gcc --version | grep gcc | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USE_MEM_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC - No STL - Force Builtins" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
gcc --version | grep gcc | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USE_MEM_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

#******************************************************************************
# CLANG
#******************************************************************************
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang - STL" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USE_MEM_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang - STL - Force C++03" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USE_MEM_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=ON ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang - No STL" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USE_MEM_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang - No STL - Builtins" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USE_MEM_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF -DETL_FORCE_TEST_CPP03_IMPLEMENTATION=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed Compilation >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC - Initializer list test" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
cd ../etl_initializer_list/
mkdir -p build-make || exit 1
cd build-make || exit 1
gcc --version | grep gcc | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="g++" -DCMAKE_C_COMPILER="gcc" ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed initializer_list Compilation >>>>"
else
  echo "****************\n**** Failed initializer_list ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi

echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang - Initializer list test" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
cmake --cmake-clean-cache -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" ..
make -j8
if [ $? -eq 0 ]; then
  echo "<<<< Passed initializer_list Compilation >>>>"
else
  echo "****************\n**** Failed initializer_list ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests
if [ $? -eq 0 ]; then
  echo "<<<< Passed Tests >>>>"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
cd ../..

echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " All Tests Completed OK" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt

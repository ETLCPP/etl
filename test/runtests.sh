#!/bin/sh
#sudo ntpdate ntp.ubuntu.com
cd build || exit 1

echo "ETL Tests" > log.txt
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
gcc --version | grep gcc | tee -a log.txt
CC=gcc CXX=g++ cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC - No STL" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
gcc --version | grep gcc | tee -a log.txt
CC=gcc CXX=g++ cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " GCC - No STL - Builtins" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
gcc --version | grep gcc | tee -a log.txt
CC=gcc CXX=g++ cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
CC=clang CXX=clang++ cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang - No STL" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
CC=clang CXX=clang++ cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Clang - No STL - Builtins" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt
clang --version | grep clang | tee -a log.txt
CC=clang CXX=clang++ cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
make -j8
if [ $? -eq 0 ]; then
  echo "Passed"
else
  echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt
  exit $?
fi
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" | tee -a log.txt
echo " Tests Completed" | tee -a log.txt
echo "-----------------------------------------------" | tee -a log.txt

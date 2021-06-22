#!/bin/sh
#sudo ntpdate ntp.ubuntu.com
cd build || exit 1
echo "ETL Tests" > etl_test_log.txt
echo ""
echo "-----------------------------------------------" >> etl_test_log.txt
echo " GCC" >> etl_test_log.txt
echo "-----------------------------------------------" >> etl_test_log.txt
gcc --version | grep gcc >> etl_test_log.txt
CC=gcc CXX=g++ cmake --cmake-clean-cache -DNO_STL=OFF ..
make
./etl_tests | tee etl_test_log.txt
echo ""
echo "-----------------------------------------------" >> etl_test_log.txt
echo " GCC - No STL" >> etl_test_log.txt
echo "-----------------------------------------------" >> etl_test_log.txt
gcc --version | grep gcc >> etl_test_log.txt
CC=gcc CXX=g++ cmake --cmake-clean-cache -DNO_STL=ON ..
make
./etl_tests | tee etl_test_log.txt
echo ""
echo "-----------------------------------------------" >> etl_test_log.txt
echo " Clang" >> etl_test_log.txt
echo "-----------------------------------------------" >> etl_test_log.txt
clang --version | grep clang >> etl_test_log.txt
CC=clang CXX=clang++ cmake --cmake-clean-cache -DNO_STL=OFF ..
make
./etl_tests | tee etl_test_log.txt
echo ""
echo "-----------------------------------------------" >> etl_test_log.txt
echo " Clang - No STL" >> etl_test_log.txt
echo "-----------------------------------------------" >> etl_test_log.txt
clang --version | grep clang >> etl_test_log.txt
CC=clang CXX=clang++ cmake --cmake-clean-cache -DNO_STL=ON ..
make
./etl_tests | tee etl_test_log.txt
echo ""
echo "-----------------------------------------------" >> etl_test_log.txt
echo " Tests Completed" >> etl_test_log.txt
echo "-----------------------------------------------" >> etl_test_log.txt


#!/bin/sh
#sudo ntpdate ntp.ubuntu.com
cd build || exit 1

echo "ETL Tests" > log.txt
echo ""
echo "-----------------------------------------------" >> log.txt
echo " GCC" >> log.txt
echo "-----------------------------------------------" >> log.txt
gcc --version | grep gcc >> log.txt
CC=gcc CXX=g++ cmake --cmake-clean-cache -DNO_STL=OFF ..
make -j8
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" >> log.txt
echo " GCC - No STL" >> log.txt
echo "-----------------------------------------------" >> log.txt
gcc --version | grep gcc >> log.txt
CC=gcc CXX=g++ cmake --cmake-clean-cache -DNO_STL=ON ..
make -j8
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" >> log.txt
echo " Clang" >> log.txt
echo "-----------------------------------------------" >> log.txt
clang --version | grep clang >> log.txt
CC=clang CXX=clang++ cmake --cmake-clean-cache -DNO_STL=OFF ..
make -j8
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" >> log.txt
echo " Clang - No STL" >> log.txt
echo "-----------------------------------------------" >> log.txt
clang --version | grep clang >> log.txt
CC=clang CXX=clang++ cmake --cmake-clean-cache -DNO_STL=ON ..
make -j8
./etl_tests | tee log.txt
echo ""
echo "-----------------------------------------------" >> log.txt
echo " Tests Completed" >> log.txt
echo "-----------------------------------------------" >> log.txt

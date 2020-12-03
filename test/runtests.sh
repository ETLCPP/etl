#sudo ntpdate time.windows.com
cd build
echo "ETL Tests" > etl_test_log.txt
echo "********** GCC **********" >> etl_test_log.txt
gcc --version | grep gcc >> etl_test_log.txt
cmake --cmake-clean-cache -DNO_STL=OFF ..
make
./etl_tests >> etl_test_log.txt
echo "********** GCC - No STL **********" >> etl_test_log.txt
gcc --version | grep gcc >> etl_test_log.txt
cmake --cmake-clean-cache -DNO_STL=ON ..
make
./etl_tests >> etl_test_log.txt
echo "********** Clang **********" >> etl_test_log.txt
clang --version | grep clang >> etl_test_log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF ..
make
./etl_tests >> etl_test_log.txt
echo "********** Clang - No STL **********" >> etl_test_log.txt
clang --version | grep clang >> etl_test_log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON ..
make
./etl_tests >> etl_test_log.txt

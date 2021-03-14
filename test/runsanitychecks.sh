
cd sanity-check
echo "" > ../log.txt

###############################################################################
cd c++03
echo "" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt
echo " C++03" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt

echo "GCC - STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "GCC - No STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - No STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

###############################################################################
cd ../c++11
echo "" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt
echo " C++11" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt

echo "GCC - STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "GCC - No STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - No STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

###############################################################################
cd ../c++14
echo "" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt
echo " C++14" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt

echo " GCC - STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "GCC - No STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - No STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

###############################################################################
cd ../c++17
echo "" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt
echo " C++17 " >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt

echo "GCC - STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "GCC - No STL" >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "Clang - No STL" >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo **** Failed **** >> ../log.txt

echo "" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt
echo " Completed" >> ../log.txt
echo "-----------------------------------------------" >> ../log.txt

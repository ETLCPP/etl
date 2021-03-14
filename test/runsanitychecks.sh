#!/bin/sh
cd sanity-check || exit 1
echo "" > ../log.txt

###############################################################################
cd c++03 || exit 1
{ echo ""
echo "-----------------------------------------------"
echo " C++03"
echo "-----------------------------------------------"

echo "GCC - STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "GCC - No STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - No STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

###############################################################################
cd ../c++11 || exit 1
{ echo ""
echo "-----------------------------------------------"
echo " C++11"
echo "-----------------------------------------------"

echo "GCC - STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "GCC - No STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - No STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

###############################################################################
cd ../c++14 || exit 1
{ echo ""
echo "-----------------------------------------------"
echo " C++14"
echo "-----------------------------------------------"

echo " GCC - STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "GCC - No STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - No STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

###############################################################################
cd ../c++17 || exit 1
{ echo ""
echo "-----------------------------------------------"
echo " C++17 "
echo "-----------------------------------------------"

echo "GCC - STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "GCC - No STL"; } >> ../log.txt
gcc --version | grep gcc >> ../log.txt
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=OFF .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""; echo "Clang - No STL"; } >> ../log.txt
clang --version | grep clang >> ../log.txt
export CC=clang
export CXX=clang++
cmake --cmake-clean-cache -DNO_STL=ON .
make || echo "**** Failed ****" >> ../log.txt

{ echo ""
echo "-----------------------------------------------"
echo " Completed"
echo "-----------------------------------------------"; } >> ../log.txt

#!/bin/sh
cd sanity-check || exit 1
echo "" > log.txt

export CMAKE_BUILD_PARALLEL_LEVEL=$(nproc --all)

###############################################################################
cd c++03 || exit 1
cmake -E make_directory bgcc bclang
{ echo ""
echo "Using "${CMAKE_BUILD_PARALLEL_LEVEL}" jobs"
echo "-----------------------------------------------"
echo " C++03"
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL - Builtins"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL - Builtins"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

###############################################################################
cd ../c++11 || exit 1
cmake -E make_directory bgcc bclang
{ echo ""
echo "-----------------------------------------------"
echo " C++11"
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL - Builtins"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL - Builtins"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

###############################################################################
cd ../c++14 || exit 1
cmake -E make_directory bgcc bclang
{ echo ""
echo "-----------------------------------------------"
echo " C++14"
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL - Builtins"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL - Builtins"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

###############################################################################
cd ../c++17 || exit 1
cmake -E make_directory bgcc bclang
{ echo ""
echo "-----------------------------------------------"
echo " C++17 "
echo "-----------------------------------------------"

echo "GCC - STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "GCC - No STL - Builtins"; } | tee -a ../log.txt
g++ --version | head --lines=1 | tee -a ../log.txt
CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "GCC - No STL - User defined traits"; } | tee -a ../log.txt
#g++ --version | head --lines=1 | tee -a ../log.txt
#CC=gcc CXX=g++ cmake -E chdir bgcc cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bgcc || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=OFF -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""; echo "Clang - No STL - Builtins"; } | tee -a ../log.txt
clang++ --version | head --lines=1 | tee -a ../log.txt
CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=ON -DETL_USER_DEFINED_TYPE_TRAITS=OFF ..
cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

##{ echo ""; echo "Clang - No STL - User defined traits"; } | tee -a ../log.txt
#clang++ --version | head --lines=1 | tee -a ../log.txt
#CC=clang CXX=clang++ cmake -E chdir bclang cmake --cmake-clean-cache -DNO_STL=ON -DETL_USE_TYPE_TRAITS_BUILTINS=OFF -DETL_USER_DEFINED_TYPE_TRAITS=ON ..
#cmake --build bclang || echo "****************\n**** Failed ****\n****************" | tee -a ../log.txt

{ echo ""
echo "-----------------------------------------------"
echo " Completed"
echo "-----------------------------------------------"; } | tee -a ../log.txt

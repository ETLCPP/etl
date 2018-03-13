cmake_minimum_required(VERSION 3.5.0)
include(ExternalProject)


function (add_unittest_cpp)

  set(UTPP_INSATLL_DIR "${CMAKE_CURRENT_BINARY_DIR}/unittest++/install")

  ExternalProject_Add(unittest-cpp
    GIT_REPOSITORY    https://github.com/unittest-cpp/unittest-cpp.git
    GIT_TAG           v2.0.0
    SOURCE_DIR  "${CMAKE_CURRENT_BINARY_DIR}/unittest++/src"
    INSTALL_DIR "${UTPP_INSATLL_DIR}"
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
  )
  
  set(UTPP_INCLUDE_DIRS "${UTPP_INSATLL_DIR}/include/UnitTest++" PARENT_SCOPE)
endfunction(add_unittest_cpp)

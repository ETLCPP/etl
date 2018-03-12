cmake_minimum_required(VERSION 3.5.0)
include(ExternalProject)


function (add_unittest_cpp)
  ExternalProject_Add(unittest-cpp
    GIT_REPOSITORY    https://github.com/unittest-cpp/unittest-cpp.git
    GIT_TAG           v2.0.0
  )
  
endfunction(add_unittest_cpp)

cmake_minimum_required(VERSION 3.5.0)
find_package(Git REQUIRED)
include(ExternalProject)


function (add_unittest_cpp)

  set(UTPP_INSATLL_DIR "${CMAKE_CURRENT_BINARY_DIR}/unittest++/install")
  if (WIN32)
    set(UTPP_LIB_PREFIX "")
    set(UTPP_LIB_SUFFIX ".lib")
  else()
    set(UTPP_LIB_PREFIX "lib")
    set(UTPP_LIB_SUFFIX ".a")
  endif()
  set(UTPP_LIB_NAME ${UTPP_LIB_PREFIX}Unittest++${UTPP_LIB_SUFFIX})


  ExternalProject_Add(unittest-cpp
    GIT_REPOSITORY    https://github.com/unittest-cpp/unittest-cpp.git
    GIT_TAG           v2.0.0
	GIT_SHALLOW		  TRUE # Don't require downloading the entire history
    SOURCE_DIR		  "${CMAKE_CURRENT_BINARY_DIR}/unittest++/src"
    INSTALL_DIR		  "${UTPP_INSATLL_DIR}"
    CMAKE_ARGS		  -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
  )
  
  set(UTPP_INCLUDE_DIRS "${UTPP_INSATLL_DIR}/include/UnitTest++" PARENT_SCOPE)
  add_library(UnitTest++ STATIC IMPORTED)
  # TODO this lib name is platform specific 
  set_target_properties(UnitTest++ PROPERTIES IMPORTED_LOCATION "${UTPP_INSATLL_DIR}/lib/${UTPP_LIB_NAME}" )
  add_dependencies(UnitTest++ unittest-cpp)

endfunction(add_unittest_cpp)

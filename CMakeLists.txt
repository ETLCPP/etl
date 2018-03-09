#######################################################################
# The Embedded Template Library (https://www.etlcpp.com/)
#######################################################################
cmake_minimum_required(VERSION 3.5.0)
project(etl)

add_library(etl
  src/binary.cpp
  src/crc16_ccitt.cpp
  src/crc16_kermit.cpp
  src/crc32.cpp
  src/crc64_ecma.cpp
  src/crc8_ccitt.cpp
  src/error_handler.cpp
  src/pearson.cpp
  src/random.cpp
  src/private
  src/private/pvoidvector.cpp
  )
target_include_directories(etl
  PUBLIC
  src/
  src/atomic
  src/profiles
  PRIVATE
  src/private
  )

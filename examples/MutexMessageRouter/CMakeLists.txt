cmake_minimum_required(VERSION 3.5.0)
project(etl_mutex_router LANGUAGES CXX)

add_executable(etl_mutex_router MutexMessageRouter.cpp)

target_compile_definitions(etl_mutex_router PRIVATE -DETL_DEBUG)

target_include_directories(etl_mutex_router
		PRIVATE
		${PROJECT_SOURCE_DIR}/../../include)

set_property(TARGET etl_mutex_router PROPERTY CXX_STANDARD 17)

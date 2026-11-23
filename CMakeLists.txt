cmake_minimum_required(VERSION 3.29.0)

project(rawbytes)

option(ENABLE_RAWBYTES_TEST "Generate test target" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rawbytes INTERFACE)

target_include_directories(rawbytes INTERFACE include)

if(ENABLE_RAWBYTES_TEST)
  add_subdirectory(tests)
endif()

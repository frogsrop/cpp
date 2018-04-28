cmake_minimum_required(VERSION 2.8)

project(debugList)

set(CMAKE_CXX_STANDARD 11)

include_directories(${debugList_SOURCE_DIR})

add_executable(main main.cpp
               debugList.h
               )
add_executable(debug_list_testing debug-list-test.cpp
               debugList.h
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc
               )


if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic ")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG ")
endif()

set(CMAKE_CXX_FLAGS -std=c++11)
target_link_libraries( debug_list_testing -lpthread)

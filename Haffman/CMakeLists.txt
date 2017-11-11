cmake_minimum_required(VERSION 2.8)

project(Haffman)

set(CMAKE_CXX_STANDARD 11)

include_directories(${Haffman_SOURCE_DIR})

add_executable( Main Main.cpp
                Haffman.cpp
                Haffman.h
                byte.cpp
                byte.h
                node.cpp
                node.h
               )
#add_executable(test test.cpp)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic ")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

set(CMAKE_CXX_FLAGS -std=c++11)
#target_link_libraries(Main -lpthread)
#target_link_libraries(test -lpthread)


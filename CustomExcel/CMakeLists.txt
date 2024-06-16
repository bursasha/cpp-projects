cmake_minimum_required(VERSION 3.22)
project(CustomExcel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wno-long-long -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

add_executable(CustomExcel
        test.cpp)

target_link_libraries(CustomExcel ${CMAKE_SOURCE_DIR}/arm64-darwin23-clang/libexpression_parser.a)


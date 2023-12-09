cmake_minimum_required(VERSION 3.2)

project(LR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_library(reader ./source/reader.h ./source/reader.cpp)
add_library(grammar ./source/grammar.h ./source/grammar.cpp)
add_library(parser ./source/parser.h ./source/parser.cpp)
add_library(lr ./source/lr.h ./source/lr.cpp)

add_executable(
    main
    ./source/main.cpp
    )

target_link_libraries(parser
                     PUBLIC reader
                     PUBLIC grammar)
target_link_libraries(lr PUBLIC parser)
target_link_libraries(main PUBLIC lr)
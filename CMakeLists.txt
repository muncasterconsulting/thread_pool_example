cmake_minimum_required(VERSION 3.0.0)

project(thread_pool_example)

# The CXX standard should be 17, even on Windows
set(CMAKE_CXX_STANDARD 17)

# Enforce CXX standard and export our compile commands to JSON
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(async libasync/src/group.cc libasync/src/pool.cc libasync/src/queue.cc)
target_include_directories(async PUBLIC ${CMAKE_SOURCE_DIR}/libasync/include)

add_executable(thread_pool_example main.cpp)
target_link_libraries(thread_pool_example async pthread)



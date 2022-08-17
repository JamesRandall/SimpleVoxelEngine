cmake_minimum_required(VERSION 3.10)

add_library(glad src/glad/glad.cpp)

target_include_directories(glad PRIVATE include/)



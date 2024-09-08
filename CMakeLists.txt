cmake_minimum_required(VERSION 3.23)
project(HLA LANGUAGES CXX)

include(CPack)
include(GoogleTest)

find_package(GTest REQUIRED) # apt install libgtest-dev

add_subdirectory(targets)

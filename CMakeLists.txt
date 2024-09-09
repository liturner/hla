cmake_minimum_required(VERSION 3.23)
project(HLA LANGUAGES CXX)

include(CPack)
include(GoogleTest)

find_package(GTest REQUIRED)  # libgtest-dev
find_package(Eigen3 REQUIRED NO_MODULE) # libeigen3-dev

add_subdirectory(targets)

# enable_testing needed on all levels to allow correct test discovery form project root
enable_testing()

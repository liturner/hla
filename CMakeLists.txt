cmake_minimum_required(VERSION 3.23)
project(HLA LANGUAGES CXX)

include(CPack)
include(GoogleTest)

find_package(GTest REQUIRED) # apt install libgtest-dev

#####
# HLA
#####

add_subdirectory(targets)

# Testing

enable_testing()
add_executable(tests
    targets/rti/src/RTI/VariableLengthData.tests.cpp
    targets/rti/src/RTI/encoding/BasicDataElements.tests.cpp
)
set_target_properties(tests PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 11
)
target_link_libraries(tests GTest::gtest_main mim)
gtest_discover_tests(tests)


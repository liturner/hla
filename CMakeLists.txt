cmake_minimum_required(VERSION 3.23)
project(
    HLA
    LANGUAGES CXX
)

# Executes during configure, not build
execute_process(
    COMMAND conan build --build=missing --output-folder ${CMAKE_BINARY_DIR} .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_VENDOR "TurnerTech")

include(CPack)
include(GoogleTest)

find_package(GTest REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
find_package(
    Doxygen
    REQUIRED dot
    OPTIONAL_COMPONENTS mscgen dia
)

add_subdirectory(targets)

enable_testing()

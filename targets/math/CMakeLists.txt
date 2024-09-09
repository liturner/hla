#####
# MATH
#####

add_library(math
    include/TT/math.h
    src/matrix.cpp
)

set_target_properties(math PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 98
)

target_include_directories(math
    PUBLIC include
    PRIVATE src
)

target_link_libraries(math Eigen3::Eigen)


add_executable(mathTests
    src/vector.tests.cpp
)

set_target_properties(mathTests PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 11
)

target_link_libraries(mathTests GTest::gtest_main math)

gtest_discover_tests(mathTests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

enable_testing()

install(TARGETS math
    LIBRARY
    PUBLIC_HEADER
)

install(DIRECTORY include/ DESTINATION include)
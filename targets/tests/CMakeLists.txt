add_executable(tests
    src/RTI/VariableLengthData.tests.cpp
    src/RTI/encoding/BasicDataElements.tests.cpp
)

set_target_properties(tests PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 11
)

target_link_libraries(tests GTest::gtest_main mim)

gtest_discover_tests(tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# enable_testing needed on all levels to allow correct test discovery form project root
enable_testing()

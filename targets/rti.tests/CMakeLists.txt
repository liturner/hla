add_executable(rti.tests
    src/RTI/VariableLengthData.tests.cpp
    src/RTI/encoding/BasicDataElements.tests.cpp
)

set_target_properties(rti.tests PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 11
)

target_link_libraries(rti.tests
    rti
    GTest::gtest_main
)

gtest_discover_tests(rti.tests
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
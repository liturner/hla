add_executable(tests
    src/RTI/VariableLengthData.tests.cpp
    src/RTI/encoding/BasicDataElements.tests.cpp
)

set_target_properties(tests PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 11
)

target_link_libraries(tests GTest::gtest_main mim)

enable_testing()
gtest_discover_tests(tests)

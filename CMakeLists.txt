cmake_minimum_required(VERSION 3.23)
project(HLA LANGUAGES CXX)

include(CPack)
include(GoogleTest)

find_package(GTest REQUIRED) # apt install libgtest-dev

add_library(hla
    # HLA Interfaces. Added here as often improves IDE experience.
    include/RTI/encoding/BasicDataElements.h
    include/RTI/encoding/DataElement.h
    include/RTI/encoding/EncodingConfig.h
    include/RTI/encoding/EncodingExceptions.h
    include/RTI/encoding/HLAfixedArray.h
    include/RTI/encoding/HLAfixedRecord.h
    include/RTI/encoding/HLAopaqueData.h
    include/RTI/encoding/HLAvariableArray.h
    include/RTI/encoding/HLAvariantRecord.h
    include/RTI/time/HLAfloat64Interval.h
    include/RTI/time/HLAfloat64Time.h
    include/RTI/time/HLAfloat64TimeFactory.h
    include/RTI/time/HLAinteger64Interval.h
    include/RTI/time/HLAinteger64Time.h
    include/RTI/time/HLAinteger64TimeFactory.h
    include/RTI/Enums.h
    include/RTI/Exception.h
    include/RTI/FederateAmbassador.h
    include/RTI/Handle.h
    include/RTI/LogicalTime.h
    include/RTI/LogicalTimeFactory.h
    include/RTI/LogicalTimeInterval.h
    include/RTI/NullFederateAmbassador.h
    include/RTI/RangeBounds.h
    include/RTI/RTI1516.h
    include/RTI/RTIambassador.h
    include/RTI/RTIambassadorFactory.h
    include/RTI/SpecificConfig.h
    include/RTI/Typedefs.h
    include/RTI/VariableLengthData.h

    # Implementation
    src/RTI/encoding/BasicDataElements.cpp
    src/RTI/encoding/DataElement.cpp
    src/RTI/encoding/HLAfixedArray.cpp
    src/RTI/VariableLengthData.cpp
    include/MIM/EnumeratedDataTypes.h
    include/MIM/HLAobjectRoot.h
    include/MIM/HLAtoken.h
    src/RTI/encoding/EncodingExceptions.cpp
    src/RTI/Exception.cpp
    src/RTI/encoding/Endianness.h

)

set_target_properties(hla PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 98
)

target_include_directories(hla
    PUBLIC include
    PRIVATE src
)

# Testing

enable_testing()
add_executable( runUnitTests
    src/RTI/VariableLengthData.tests.cpp
    src/RTI/encoding/BasicDataElements.tests.cpp
)
set_target_properties(runUnitTests PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 98
)
target_link_libraries(runUnitTests GTest::gtest_main hla)
gtest_discover_tests(runUnitTests)

# Deploy

install(TARGETS hla
    LIBRARY
    PUBLIC_HEADER
)
install(DIRECTORY include/ DESTINATION include)


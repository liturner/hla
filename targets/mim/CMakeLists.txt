#####
# MIM
#####

add_library(mim
    include/MIM/EnumeratedDataTypes.h
    include/MIM/HLAobjectRoot.h
    include/MIM/HLAtoken.h
)

set_target_properties(mim PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 98
)

target_include_directories(mim
    PUBLIC include
    PRIVATE src
)

target_link_libraries(mim PUBLIC
    rti
)

install(TARGETS mim
    LIBRARY
    PUBLIC_HEADER
)

install(DIRECTORY include/ DESTINATION include)
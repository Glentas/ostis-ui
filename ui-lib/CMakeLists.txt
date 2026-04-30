# ui-lib/CMakeLists.txt
cmake_minimum_required(VERSION 3.24)

add_library(ui-lib STATIC
    UILib.cpp
)

target_include_directories(ui-lib
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../translators/HTMLTranslatorModule
)

target_link_libraries(ui-lib
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
)

set_target_properties(ui-lib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)
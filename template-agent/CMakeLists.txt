set(SPECIFIED_TEMPLATE_AGENT_SOURCES
    "renderer/TemplateActionGenerator.cpp"
    "renderer/JsonToXMLAgent.cpp"
)
set(SPECIFIED_TEMPLATE_AGENT_HEADERS
    "renderer/TemplateActionGenerator.hpp"
    "renderer/JsonToXMLAgent.hpp"
    "keynodes/SpecifiedStringTemplateKeynodes.hpp"
)

add_library( 
    ostis-specified-template-agent 
    SHARED ${SPECIFIED_TEMPLATE_AGENT_SOURCES} ${SPECIFIED_TEMPLATE_AGENT_HEADERS}
)

target_include_directories(ostis-specified-template-agent
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(ostis-specified-template-agent
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PRIVATE nlohmann_json::nlohmann_json
)

if(${SC_BUILD_TESTS})
    add_subdirectory(tests)
endif()

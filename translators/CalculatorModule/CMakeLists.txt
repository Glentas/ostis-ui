project(ostis-ui-calculator)
set(CALCULATOR_SOURCES
	"CalculatorModule.cpp"
	"agents/CalculatorAgent.cpp"

)

set(CALCULATOR_HEADERS
    "CalculatorModule.hpp"
    "keynodes/CalculatorKeynodes.hpp"
    "agents/CalculatorAgent.hpp"
)

add_library(${PROJECT_NAME}
    SHARED ${CALCULATOR_SOURCES} ${CALCULATOR_HEADERS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

find_package(httplib REQUIRED)

target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PRIVATE httplib::httplib
)

if(${APPLE})
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CFNETWORK_FRAMEWORK CFNetwork)

    target_link_libraries(ostis-ui-html-translator
        LINK_PRIVATE ${COREFOUNDATION_FRAMEWORK}
        LINK_PRIVATE ${CFNETWORK_FRAMEWORK}
    )
endif()


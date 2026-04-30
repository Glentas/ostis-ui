set(HTML_TRANSLATOR_SOURCES
	"HTMLTranslatorModule.cpp"
	"agents/HTMLTranslatorAgent.cpp"
    "html-translator/HTMLTranslator.cpp"
    "parameter-retriever/ParameterRetriever.cpp"
    "agents/UIComponentsReorderAgent.cpp"
    "agents/AdaptationManagerAgent.cpp"
    "agents/VisualAdaptationAgent.cpp"
    "HTTPServer.cpp"
    "HTTPRequestHandler.cpp"
)

set(HTML_TRANSLATOR_HEADERS
    "HTMLTranslatorModule.hpp"
    "keynodes/HTMLTranslatorKeynodes.hpp"
    "agents/HTMLTranslatorAgent.hpp"
    "html-translator/HTMLTranslator.hpp"
    "parameter-retriever/ParameterRetriever.hpp"
    "agents/UIComponentsReorderAgent.hpp"
    "agents/AdaptationManagerAgent.hpp"
    "agents/VisualAdaptationAgent.hpp"
    "HTTPServer.hpp"
    "HTTPRequestHandler.hpp"
)

add_library(ostis-ui-html-translator
    SHARED ${HTML_TRANSLATOR_SOURCES} ${HTML_TRANSLATOR_HEADERS}
)

target_include_directories(ostis-ui-html-translator
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

find_package(httplib REQUIRED)

target_link_libraries(ostis-ui-html-translator
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PRIVATE httplib::httplib
    LINK_PRIVATE ui-lib
)

if(${APPLE})
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CFNETWORK_FRAMEWORK CFNetwork)

    target_link_libraries(ostis-ui-html-translator
        LINK_PRIVATE ${COREFOUNDATION_FRAMEWORK}
        LINK_PRIVATE ${CFNETWORK_FRAMEWORK}
    )
endif()

if(${SC_BUILD_TESTS})
    add_subdirectory(tests)
endif()

project(ostis-ui-html-translator-agent-test)
set(HTML_TRANSLATOR_AGENT_TEST_SOURCES
    "units/HTMLTranslationAgentTest.cpp"
    "units/HTMLTranslationTest.cpp"
)
add_executable(${PROJECT_NAME} ${HTML_TRANSLATOR_AGENT_TEST_SOURCES})

target_link_libraries(
    ${PROJECT_NAME}
    LINK_PRIVATE GTest::gtest_main
    LINK_PRIVATE ostis-ui-html-translator
    LINK_PRIVATE sc-machine::sc-builder-lib
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
gtest_discover_tests(${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/units)

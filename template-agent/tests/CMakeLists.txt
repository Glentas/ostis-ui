set(TEMPLATE_AGENT_TEST_SOURCES
    "units/SpecifiedStringTemplateAgentTest.cpp"
    "units/StringTemplateRendererTest.cpp"
    "units/TemplateActionGeneratorTest.cpp"
)

add_executable(template-agent-test ${TEMPLATE_AGENT_TEST_SOURCES})

target_link_libraries(template-agent-test
    LINK_PRIVATE GTest::gtest_main
    LINK_PRIVATE ostis-specified-template-agent 
    LINK_PRIVATE sc-machine::sc-builder-lib
)

target_include_directories(
    template-agent-test
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
gtest_discover_tests(template-agent-test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/units)

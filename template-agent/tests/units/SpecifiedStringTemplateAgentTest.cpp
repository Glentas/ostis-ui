/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-memory/test/sc_test.hpp>
#include <sc-builder/scs_loader.hpp>

#include <sc-memory/sc_agent.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "agent/SpecifiedStringTemplateAgent.hpp"
#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"

using namespace specifiedStringTemplateModule;
namespace templateAgentTest
{
std::string const TEST_FILES_DIR_PATH = "../test-structures/";
int const WAIT_TIME = 1000;
using TemplateAgentTest = ScMemoryTest;


void TestTemplateAgent(ScAgentContext & context, std::string const & scsTestFile)
{
  context.SubscribeAgent<SpecifiedStringTemplateAgent>();

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + scsTestFile);

  // Call the agent, get and validate result
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(context.IsElement(test_action_node));
  ScAction action = context.ConvertToAction(test_action_node);
  action.InitiateAndWait();
  ScStructure result = action.GetResult();
  EXPECT_TRUE(context.IsElement(result));
  ScAddr resultLink = utils::IteratorUtils::getAnyFromSet(&context, result);
  EXPECT_TRUE(context.IsElement(resultLink));

  std::string resultLinkContent;
  context.GetLinkContent(resultLink, resultLinkContent);
  EXPECT_NE(resultLinkContent, "");

  // Check if the result is correct
  ScAddr string_template_expected_result = context.SearchElementBySystemIdentifier("string_template_expected_result");
  std::string string_template_expected_result_content;
  context.GetLinkContent(string_template_expected_result, string_template_expected_result_content);
  EXPECT_EQ(string_template_expected_result_content, resultLinkContent);

  // Check if nrel_format is generated for the result sc-link
  ScIterator5Ptr formatIterator = context.CreateIterator5(
        resultLink,
        ScType::ConstCommonArc,
        context.SearchElementBySystemIdentifier("format_html"),
        ScType::ConstPermPosArc,
        SpecifiedStringTemplateKeynodes::nrel_format);
  EXPECT_TRUE(formatIterator->Next());

  context.UnsubscribeAgent<SpecifiedStringTemplateAgent>();
}

TEST_F(TemplateAgentTest, ZeroVariablesStringTemplate)
{
  TestTemplateAgent(*m_ctx, "zero_variables_string_template.scs");
}

TEST_F(TemplateAgentTest, OneVariableStringTemplate)
{
  TestTemplateAgent(*m_ctx, "one_variable_string_template.scs");
}

} // namespace templateAgentTest

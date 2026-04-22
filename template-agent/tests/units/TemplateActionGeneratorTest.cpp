/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-memory/test/sc_test.hpp>
#include <sc-builder/scs_loader.hpp>

#include <sc-memory/sc_agent.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "renderer/StringTemplateRenderer.hpp"
#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"

#include "renderer/TemplateActionGenerator.hpp"
#include "renderer/JsonToXMLAgent.hpp"

using namespace specifiedStringTemplateModule;

namespace rendererTest
{
std::string const TEST_FILES_DIR_PATH = "../test-structures/";

using RendererTest = ScMemoryTest;

void GenerateButtonAction(ScAgentContext & context, std::string const & scsTestFile)
{
context.SubscribeAgent<GenerateTemplateAgent>();

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + scsTestFile);

  // Call the agent, get and validate result
  ScAddr test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(context.IsElement(test_action_node));
  ScAction action = context.ConvertToAction(test_action_node);
  action.InitiateAndWait();


  // Check if the result is correct
  ScAddr string_template_expected_result = context.SearchElementBySystemIdentifier("string_template_expected_result");
  std::string string_template_expected_result_content;
  context.GetLinkContent(string_template_expected_result, string_template_expected_result_content);

  ScAddr string_result = context.SearchElementBySystemIdentifier("result");
  std::string string_result_content;
  context.GetLinkContent(string_result, string_result_content);

  EXPECT_EQ(string_template_expected_result_content, string_result_content);

  context.UnsubscribeAgent<GenerateTemplateAgent>();
}

TEST_F(RendererTest, ButtonActionGenerate)
{
  GenerateButtonAction(*m_ctx, "template_agent.scs");
}





} // namespace rendererTest

/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-memory/sc_agent.hpp>

#include "agents/HTMLTranslatorAgent.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"

#include "utils.hpp"

using namespace htmlTranslationModule;

namespace htmlTranslatorAgentTest
{
ScsLoader loader;

size_t const WAIT_TIME = 1000;
using HTMLTranslatorAgentTest = ScMemoryTest;

void TestHTMLTranslatorAgent(ScAgentContext & context)
{
  context.SubscribeAgent<HTMLTranslatorAgent>();

  loadKB(context, loader);

  // Call the agent, get and validate result
  ScAddr actionNode = context.SearchElementBySystemIdentifier("ui_mwhreyui_6");
  EXPECT_TRUE(context.IsElement(actionNode));

  ScAction action = context.GenerateAction(HTMLTranslatorKeynodes::action_translate_sc_to_html);
  action.SetArguments(actionNode);
  action.InitiateAndWait();

  ScStructure result = action.GetResult();
  EXPECT_TRUE(context.IsElement(result));

  ScAddr resultLink = utils::IteratorUtils::getAnyFromSet(&context, result);
  EXPECT_TRUE(context.IsElement(resultLink));

  std::string resultLinkContent;
  context.GetLinkContent(resultLink, resultLinkContent);
  EXPECT_EQ(resultLinkContent, "");

  context.UnsubscribeAgent<HTMLTranslatorAgent>();
}

TEST_F(HTMLTranslatorAgentTest, TranslateUi)
{
  TestHTMLTranslatorAgent(*m_ctx);
}

}  // namespace htmlTranslatorAgentTest

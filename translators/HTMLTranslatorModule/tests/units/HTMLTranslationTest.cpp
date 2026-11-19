/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <filesystem>

#include <fstream>
#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-memory/sc_agent.hpp>
#include <sc-memory/utils/sc_logger.hpp>

#include "agents/HTMLTranslatorAgent.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"

namespace fs = std::filesystem;

using namespace htmlTranslationModule;

namespace htmlTranslatorAgentTest
{
using HTMLTranslatorAgentTest = ScMemoryTest;
ScsLoader loader;
auto const rootPath = fs::path(__FILE__).parent_path().parent_path();
auto const kbFolder = "kb";
auto const generatedFolder = "generated";
auto const examplesFolder = "examples";

void loadKB(ScAgentContext & context, ScsLoader & loader, fs::path const & kbPath)
{
  for (auto const & entry : fs::recursive_directory_iterator(kbPath, fs::directory_options::skip_permission_denied))
  {
    if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".scs")
    {
      SC_LOG(utils::ScLogLevel::Info, std::format("Found KB entry: {}", entry.path().generic_string()));
      loader.loadScsFile(context, entry.path());
    }
  }
}

void TestHTMLTranslatorAgent(ScAgentContext & context, fs::path const & path)
{
  context.SubscribeAgent<HTMLTranslatorAgent>();

  // Call the agent, get and validate result
  ScAddr actionNode = context.SearchElementBySystemIdentifier(path.stem());
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
  EXPECT_NE(resultLinkContent, "");

  {
    auto htmlPath = rootPath / generatedFolder / (path.stem().string() + ".html");
    std::ofstream os(htmlPath);
    os << resultLinkContent;
  }

  context.UnsubscribeAgent<HTMLTranslatorAgent>();
}

TEST_F(HTMLTranslatorAgentTest, TranslateUi)
{
  loadKB(*m_ctx, loader, rootPath / kbFolder);
  SC_LOG(utils::ScLogLevel::Info, "Loaded test KB");

  for (auto const & entry :
       fs::recursive_directory_iterator(rootPath / examplesFolder, fs::directory_options::skip_permission_denied))
  {
    if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".scs")
    {
      SC_LOG(utils::ScLogLevel::Info, std::format("Found test entry: {}", entry.path().generic_string()));
      loader.loadScsFile(*m_ctx, entry.path());
      TestHTMLTranslatorAgent(*m_ctx, entry.path());
    }
  }
}

}  // namespace htmlTranslatorAgentTest

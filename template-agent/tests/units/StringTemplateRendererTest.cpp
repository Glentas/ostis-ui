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

using namespace specifiedStringTemplateModule;

namespace rendererTest
{
std::string const TEST_FILES_DIR_PATH = "../test-structures/";

using RendererTest = ScMemoryTest;

void TestRenderer(ScAgentContext & context, std::string const & scsTestFile)
{
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + scsTestFile);

  ScAddr string_template = context.SearchElementBySystemIdentifier("string_template");
  ScAddr replacement_values = context.SearchElementBySystemIdentifier("replacement_values");
  ScAddr formatHTML = context.SearchElementBySystemIdentifier("format_html");
  std::string result = specifiedStringTemplateModule::StringTemplateRenderer::RenderStringTemplate(context, string_template, replacement_values, formatHTML);

  std::string expectedResult;
  ScAddr string_template_expected_result = context.SearchElementBySystemIdentifier("string_template_expected_result");
  context.GetLinkContent(string_template_expected_result, expectedResult);

  EXPECT_EQ(result, expectedResult);
}

TEST_F(RendererTest, ZeroVariablesStringTemplate)
{
  TestRenderer(*m_ctx, "zero_variables_string_template.scs");
}

TEST_F(RendererTest, OneVariableStringTemplate)
{
  TestRenderer(*m_ctx, "one_variable_string_template.scs");
}

TEST_F(RendererTest, TwoVariablesStringTemplate)
{
  TestRenderer(*m_ctx, "two_variables_string_template.scs");
}

TEST_F(RendererTest, ThreeVariablesStringTemplate)
{
  TestRenderer(*m_ctx, "three_variables_string_template.scs");
}

TEST_F(RendererTest, NoVariablesStringTemplate)
{
  EXPECT_ANY_THROW(TestRenderer(*m_ctx, "no_variables_string_template.scs"));
}

TEST_F(RendererTest, NotAllVariablesStringTemplate)
{
  EXPECT_ANY_THROW(TestRenderer(*m_ctx, "not_all_variables_string_template.scs"));
}

} // namespace rendererTest

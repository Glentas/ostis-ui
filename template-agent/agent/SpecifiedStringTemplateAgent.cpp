/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/GenerationUtils.hpp>

#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"
#include "renderer/StringTemplateRenderer.hpp"

#include "SpecifiedStringTemplateAgent.hpp"

using namespace utils;

namespace specifiedStringTemplateModule
{
ScResult SpecifiedStringTemplateAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  // Get string template sc-link
  auto const [stringTemplateLink, stringTemplateLinkReplacements, generatedLinkFormatClass] = action.GetArguments<3>();

  if (!m_context.IsElement(stringTemplateLink))
  {
    SC_LOG_ERROR("Action doesn't have a string template link.");
    return action.FinishUnsuccessfully();
  }

  if (!m_context.IsElement(generatedLinkFormatClass))
  {
    SC_LOG_ERROR("Action doesn't have a format.");
    return action.FinishUnsuccessfully();
  }

  std::string result;
  try
  {
    result = StringTemplateRenderer::RenderStringTemplate(
        m_context, stringTemplateLink, stringTemplateLinkReplacements, generatedLinkFormatClass);
  }
  catch (ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
    return action.FinishUnsuccessfully();
  }

  // Create result sc-link and set result text as a link content
  ScAddr const resultLink = m_context.GenerateLink();
  m_context.SetLinkContent(resultLink, result);
  // Generate nrel_fromat to link if format is passed to the action parameter
  if (generatedLinkFormatClass.IsValid())
  {
    GenerationUtils::generateRelationBetween(
        &m_context, resultLink, generatedLinkFormatClass, SpecifiedStringTemplateKeynodes::nrel_format);
  }

  // Create agent answer and finish agent work
  ScStructure structAddr = m_context.GenerateStructure();
  structAddr << resultLink;
  action.SetResult(structAddr);
  SC_LOG_INFO("SpecifiedStringTemplateAgent finished");
  return action.FinishSuccessfully();
}

ScAddr SpecifiedStringTemplateAgent::GetActionClass() const
{
  return SpecifiedStringTemplateKeynodes::action_evaluate_specified_string_template;
}

}  // namespace specifiedStringTemplateModule

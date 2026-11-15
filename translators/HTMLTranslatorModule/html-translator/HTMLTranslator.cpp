/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "HTMLTranslator.hpp"
#include "utils/ParameterRetriever.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"

#include <cstddef>
#include <string>
#include <format>

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_debug.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/utils/sc_logger.hpp>

using namespace utils;

namespace htmlTranslationModule
{
ScAddr HTMLTranslator::TranslateSCToHTML(ScAgentContext & context, ScAddr const & interfaceNode)
{
  if (!context.IsElement(interfaceNode))
  {
    auto const message = std::format(
        "{}: given '{},{}' interface node is invalid.",
        className,
        interfaceNode.GetRealAddr().seg,
        interfaceNode.GetRealAddr().offset);
    SC_LOG_ERROR(message);
    throw utils::ScException(utils::ExceptionInvalidParams("", message));
  }

  // We are getting specific template for given ui component
  auto const componentHTMLTemplateLink = GetUIComponentHTMLTemplate(context, interfaceNode);

  // This link must contain fully translated html document
  auto const answerHTMLLink = GetAnswerLink(context, interfaceNode, componentHTMLTemplateLink);

  return answerHTMLLink;
}

ScAddr HTMLTranslator::GetAnswerLink(
    ScAgentContext & context,
    ScAddr const & uiComponent,
    ScAddr const & componentHTMLTemplateLink)
{
  ScAddr linkWithHTMLRepresentation = context.GenerateLink();

  std::string componentTemplateString;
  context.GetLinkContent(componentHTMLTemplateLink, componentTemplateString);

  // What nested components do we have
  auto params = ParameterRetriever::GetUIComponentParams(context, uiComponent);
  auto nestedComponents = StringScAddrMap();
  // Get theirs html representation
  StringStringMap IDsAndRepresentations = GetNestedComponentsHTMLRepresentation(context, nestedComponents);

  // Replace parameters with actual html code
  for (auto const & [ID, representation] : IDsAndRepresentations)
  {
    // Update template content by reference
    InsertParameterValue(componentTemplateString, ID, representation);
  }

  context.SetLinkContent(linkWithHTMLRepresentation, componentTemplateString);

  // Generating html representation
  ScAddr arcAddr = context.GenerateConnector(ScType::CommonArc, uiComponent, linkWithHTMLRepresentation);
  context.GenerateConnector(ScType::PermPosArc, HTMLTranslatorKeynodes::nrel_html_representation, arcAddr);

  if (!context.IsElement(linkWithHTMLRepresentation))
  {
    SC_LOG_ERROR("HTMLTranslator: Recursive answer link is invalid.");
    throw utils::ScException(utils::ExceptionInvalidState("HTMLTranslator: Recursive answer link is invalid.", ""));
  }
  return linkWithHTMLRepresentation;
}

StringStringMap HTMLTranslator::GetNestedComponentsHTMLRepresentation(
    ScAgentContext & context,
    StringScAddrMap const & nestedComponents)
{
  StringStringMap IDsAndRepresentations;
  for (auto const & [ID, parameterAddr] : nestedComponents)
  {
    // launch recursive agent for nested components
    ScAction action = context.GenerateAction(HTMLTranslatorKeynodes::action_translate_sc_to_html);
    action.SetArguments(parameterAddr);
    action.InitiateAndWait();
    ScStructure translationResult = action.GetResult();
    ScAddr translationResultLink = utils::IteratorUtils::getAnyFromSet(&context, translationResult);

    if (!context.IsElement(translationResultLink))
    {
      SC_LOG_ERROR("HTMLTranslator: translation of component with id=" + ID + " failed.");
      throw utils::ScException(
          utils::ExceptionItemNotFound("HTMLTranslator: translation of component with id=" + ID + " failed.", ""));
    }
    //
    std::string representation;
    context.GetLinkContent(translationResultLink, representation);
    IDsAndRepresentations[ID] = representation;
  }
  return IDsAndRepresentations;
}

void HTMLTranslator::InsertParameterValue(
    std::string & componentTemplateString,
    std::string const & parameterID,
    std::string const & parameterValue)
{
  if (componentTemplateString.empty() || parameterID.empty() || parameterValue.empty())
  {
    return;
  }

  std::string const openBrackets = "{{";
  std::string const closeBrackets = "}}";

  size_t pos = 0;

  while ((pos = componentTemplateString.find(openBrackets, pos)) != std::string::npos)
  {
    size_t closePos = componentTemplateString.find(closeBrackets, pos + openBrackets.size());

    if (closePos == std::string::npos)
    {
      SC_LOG_ERROR("HTMLTranslator: given html template is invalid.");
      throw utils::ScException(
          utils::ExceptionInvalidParams("HTMLTranslator: given html template is invalid.", "Unmatched '{{' bracket."));
    }

    std::string foundParameterID =
        componentTemplateString.substr(pos + openBrackets.size(), closePos - pos - openBrackets.size());

    size_t first_letter = foundParameterID.find_first_not_of(" \n\r\t");
    if (first_letter != std::string::npos)
    {
      size_t last_letter = foundParameterID.find_last_not_of(" \n\r\t");
      foundParameterID = foundParameterID.substr(first_letter, last_letter - first_letter + 1);
    }
    else
    {
      foundParameterID.clear();
    }

    if (foundParameterID == parameterID)
    {
      componentTemplateString.replace(pos, closePos - pos + closeBrackets.size(), parameterValue);
    }
    pos = closePos + closeBrackets.size();
  }
}
}  // namespace htmlTranslationModule
/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "HTMLTranslator.hpp"
#include "parameter-retriever/ParameterRetriever.hpp"

#include <cstddef>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include <keynodes/HTMLTranslatorKeynodes.hpp>
#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_debug.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/utils/sc_logger.hpp>
#include <string>

using namespace utils;

namespace htmlTranslationModule
{
ScAddr HTMLTranslator::TranslateScToHTML(ScAgentContext & context, ScAddr const & uiComponent)
{
  if (!context.IsElement(uiComponent))
  {
    SC_LOG_ERROR("HTMLTranslator: given UI element is invalid.");
    throw utils::ScException(utils::ExceptionInvalidParams("HTMLTranslator: given UI element is invalid.", ""));
  }

  // If element already has html translation - return it
  //
  // TODO: we need mechanism that will allow us to regenerate component (some
  // bool flag?)
  ScAddr answerHTMLLink =
      IteratorUtils::getAnyByOutRelation(&context, uiComponent, HTMLTranslatorKeynodes::nrel_html_representation);

  if (context.IsElement(answerHTMLLink))
  {
    return answerHTMLLink;
  }

  // We are getting specific template for given ui component
  ScAddr componentHTMLTemplateLink = GetUIComponentHTMLTemplate(context, uiComponent);

  // This link should contain fully translated html document
  answerHTMLLink = GetAnswerLink(context, uiComponent, componentHTMLTemplateLink);

  return answerHTMLLink;
}

ScAddr HTMLTranslator::GetUIComponentHTMLTemplate(ScAgentContext & context, ScAddr const & uiComponent)
{
  ScAddr componentHTMLTemplateLink;
  std::string const componentHTMLTemplateLinkAlias = "_component_html_template_link";

  ScTemplate componentHTMLTemplate;

  //
  // component = = = = = => template_link_for_component
  //                ^
  //                |
  //
  //                |
  //
  //                |
  //
  //        nrel_html_template
  //
  // Search for an HTML template for this component
  componentHTMLTemplate.Quintuple(
      uiComponent,
      ScType::VarCommonArc,
      ScType::VarNodeLink >> componentHTMLTemplateLinkAlias,
      ScType::VarPermPosArc,
      HTMLTranslatorKeynodes::nrel_html_template);

  // Search only the first template result
  context.SearchByTemplateInterruptibly(
      componentHTMLTemplate,
      [&componentHTMLTemplateLinkAlias, &componentHTMLTemplateLink](ScTemplateSearchResultItem const & item)
      {
        item.Get(componentHTMLTemplateLinkAlias, componentHTMLTemplateLink);
        return ScTemplateSearchRequest::STOP;
      });

  if (!context.IsElement(componentHTMLTemplateLink))
  {
    SC_LOG_ERROR("HTMLTranslator: nrel_html_template not found.");
    throw utils::ScException(
        utils::ExceptionItemNotFound("HTMLTranslator: html template for element is not found.", ""));
  }

  return componentHTMLTemplateLink;
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
  StringScAddrMap nestedComponents = ParameterRetriever::GetNestedUIComponents(context, uiComponent);

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
/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "ParameterRetriever.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_template.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <string>

using namespace utils;

namespace htmlTranslationModule
{

StringScAddrMap ParameterRetriever::GetNestedUIComponents(ScAgentContext & context, ScAddr const & uiComponent)
{
  StringScAddrMap nestedComponents;

  ScAddrSet unmappedParameters;

  ScTemplate scClassTemplate = GetComponentClassTemplate(context, uiComponent);

  // Getting parameters
  context.SearchByTemplateInterruptibly(
      scClassTemplate,
      [&unmappedParameters, &uiComponent](ScTemplateSearchResultItem const & item)
      {
        ScAddr const someRootComponent = item[HTMLTranslatorKeynodes::_root_ui_obj];
        if (someRootComponent == uiComponent)
        {
          unmappedParameters.insert(item[HTMLTranslatorKeynodes::_ui_obj]);
        }
        return ScTemplateSearchRequest::CONTINUE;
      });

  // We check here which link with ID corresponds to which component.
  for (auto const & param : unmappedParameters)
  {
    //
    // parameter = = = = = => _parameter_id_link
    //                ^
    //                |
    //
    //                |
    //
    //                |
    //
    //      nrel_html_parameter_id
    //
    ScAddr linkWithID =
        utils::IteratorUtils::getAnyByOutRelation(&context, param, HTMLTranslatorKeynodes::nrel_html_parameter_id);

    if (!context.IsElement(linkWithID))
    {
      SC_LOG_ERROR("ParameterRetriever: nested component id link is invalid.");
      throw utils::ScException(
          utils::ExceptionItemNotFound("ParameterRetriever: nested component id link is invalid.", ""));
    }

    std::string idLinkValue;
    context.GetLinkContent(linkWithID, idLinkValue);
    nestedComponents[idLinkValue] = param;
  }

  return nestedComponents;
}

ScTemplate ParameterRetriever::GetComponentClassTemplate(ScAgentContext & context, ScAddr const & uiComponent)
{
  ScAddr uiComponentClass;
  std::string const componentClassAlias = "_component_class";
  ScTemplate componentClassTemplate;

  //
  // concept_ui_object = = = = = => uiComponentClass
  //                        ^
  //                        |
  //
  //                        |
  //
  //                        |
  //
  //                 nrel_inclusion
  //
  componentClassTemplate.Quintuple(
      HTMLTranslatorKeynodes::concept_ui_object,
      ScType::VarCommonArc,
      ScType::VarNodeClass >> componentClassAlias,
      ScType::VarPermPosArc,
      HTMLTranslatorKeynodes::nrel_inclusion);

  //
  // uiComponentClass - - - -> uiComponent
  //
  componentClassTemplate.Triple(componentClassAlias, ScType::VarPermPosArc, uiComponent);

  context.SearchByTemplateInterruptibly(
      componentClassTemplate,
      [&componentClassAlias, &uiComponentClass](ScTemplateSearchResultItem const & item)
      {
        item.Get(componentClassAlias, uiComponentClass);
        return ScTemplateSearchRequest::STOP;
      });

  if (!context.IsElement(uiComponentClass))
  {
    SC_LOG_ERROR("ParameterRetriever: given component doesn't has it's class.");
    throw utils::ScException(
        utils::ExceptionItemNotFound("ParameterRetriever: given component doesn't has it's class.", ""));
  }

  //
  // uiComponentClass = = = = = => scClassTemplate
  //                       ^
  //                       |
  //
  //                       |
  //
  //                       |
  //
  //                nrel_sc_template
  //
  ScAddr scClassTemplateStructure =
      utils::IteratorUtils::getAnyByOutRelation(&context, uiComponentClass, HTMLTranslatorKeynodes::nrel_sc_template);

  if (!context.IsElement(uiComponentClass))
  {
    SC_LOG_ERROR(
        "ParameterRetriever: given component class doesn't has it's template / doesn't has it's template linked with "
        "nrel_sc_template relation.");
    throw utils::ScException(
        utils::ExceptionItemNotFound(
            "ParameterRetriever: given component class doesn't has it's template / doesn't has it's template linked "
            "with nrel_sc_template relation.",
            ""));
  }

  ScTemplate scClassTemplate;
  try
  {
    context.BuildTemplate(scClassTemplate, scClassTemplateStructure);
  }
  catch (utils::ExceptionInvalidState)
  {
    SC_LOG_ERROR("ParameterRetriever: given component class template is invalid.");
    throw utils::ScException(
        utils::ExceptionItemNotFound("ParameterRetriever: given component class template is invalid.", ""));
  }

  return scClassTemplate;
}
}  // namespace htmlTranslationModule
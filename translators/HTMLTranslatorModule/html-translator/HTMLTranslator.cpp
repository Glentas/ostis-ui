/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "HTMLTranslator.hpp"

#include "utils/AbstractUIModel.hpp"
#include <string>
#include <format>

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_debug.hpp>
#include <sc-memory/sc_template.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/utils/sc_logger.hpp>

#include "utils/ParameterRetriever.hpp"
#include "utils/HTMLTagMapper.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"

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
  auto uiComponents = GetUIComponents(context, interfaceNode);
  auto uiComponentLinks = GetUIComponentLinks(context, uiComponents);
  auto resolvedUIComponents = ResolveUIComponents(context, uiComponents);
  auto const answerHTMLLink = ScAddr();

  return answerHTMLLink;
}

ScAddrUnorderedSet HTMLTranslator::GetUIComponents(ScAgentContext & context, ScAddr const & interfaceNode)
{
  ScAddrUnorderedSet uniqueUIComponents;
  constexpr auto uiComponentAlias = "_ui_component";
  ScTemplate uiComponentTemplate;
  //
  // interfaceNode = = = = = => _ui_component
  //                ^
  //                |
  //
  //                |
  //
  //                |
  //
  //       nrel_ui_has_component
  //
  uiComponentTemplate.Quintuple(
      interfaceNode,
      ScType::VarCommonArc,
      uiComponentAlias,
      ScType::VarPermPosArc,
      HTMLTranslatorKeynodes::nrel_ui_has_component);

  try
  {
    // collect component nodes
    context.SearchByTemplate(
        uiComponentTemplate,
        [&](ScTemplateSearchResultItem const & item)
        {
          ScAddr uiComponent;
          item.Get(uiComponentAlias, uiComponent);
          uniqueUIComponents.insert(std::move(uiComponent));
        });
  }
  catch (utils::ExceptionInvalidState const & e)
  {
    auto const message = std::format("{}: search by template failed: {}.", className, e.what());
    SC_LOG_ERROR(message);
    throw e;
  }

  return uniqueUIComponents;
}

std::vector<std::pair<ScAddr, ScAddr>> HTMLTranslator::GetUIComponentLinks(
    ScAgentContext & context,
    ScAddrUnorderedSet const & uiComponents)
{
  // TODO: set instead of vector
  std::vector<std::pair<ScAddr, ScAddr>> uiComponentLinks;

  for (auto const & source : uiComponents)
  {
    constexpr auto targetAlias = "_target";
    ScTemplate uiLinkTemplate;
    //
    // source = = = = = => _target
    //                ^
    //                |
    //
    //                |
    //
    //                |
    //
    //         nrel_ui_contains
    //
    uiLinkTemplate.Quintuple(
        source, ScType::VarCommonArc, targetAlias, ScType::VarPermPosArc, HTMLTranslatorKeynodes::nrel_ui_contains);

    try
    {
      // collect component nodes
      context.SearchByTemplate(
          uiLinkTemplate,
          [&](ScTemplateSearchResultItem const & item)
          {
            ScAddr target;
            item.Get(targetAlias, target);
            uiComponentLinks.emplace_back(source, std::move(target));
          });
    }
    catch (utils::ExceptionInvalidState const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw e;
    }
  }
  return uiComponentLinks;
}

std::vector<std::pair<ScAddr, UIComponent>> HTMLTranslator::ResolveUIComponents(
    ScAgentContext & context,
    ScAddrUnorderedSet const & uiComponents)
{
  std::vector<std::pair<ScAddr, UIComponent>> resolvedUIComponents;

  for (auto const & uiComponent : uiComponents)
  {
    auto type = ResolveComponentType(context, uiComponent);
    auto params = ParameterRetriever::GetUIComponentParams(context, uiComponent);
    constexpr auto uiComponentClassAlias = "_ui_component_class", uiComponentIDAlias = "_ui_component_id";
    ScTemplate uiComponentTemplate;
    //
    // uiComponent = = = = = => _ui_component_id
    //                  ^
    //                  |
    //
    //                  |
    //
    //                  |
    //
    //             nrel_ui_id
    //
    uiComponentTemplate.Quintuple(
        uiComponent,
        ScType::VarCommonArc,
        uiComponentIDAlias,
        ScType::VarPermPosArc,
        HTMLTranslatorKeynodes::nrel_ui_id);
    try
    {
      context.SearchByTemplate(
          uiComponentTemplate,
          [&](ScTemplateSearchResultItem const & item)
          {
            ScAddr uiComponentID;
            item.Get(uiComponentIDAlias, uiComponentID);
            std::string idValue;
            context.GetLinkContent(uiComponentID, idValue);
            auto component = UIComponent(idValue, type, std::move(params));
            resolvedUIComponents.emplace_back(uiComponent, std::move(component));
          });
    }
    catch (utils::ExceptionInvalidState const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw e;
    }
  }
  return resolvedUIComponents;
}

UIComponentType HTMLTranslator::ResolveComponentType(ScAgentContext & context, ScAddr const & uiComponent)
{
  constexpr auto uiComponentClassAlias = "_candidate";
  ScTemplate uiComponentTemplate;
  ScAddrUnorderedSet candidates;
  UIComponentType type;
  //
  // _ui_component_class - - - - - -> uiComponent
  //
  uiComponentTemplate.Triple(uiComponentClassAlias, ScType::VarPermPosArc, uiComponent);
  try
  {
    context.SearchByTemplate(
        uiComponentTemplate,
        [&](ScTemplateSearchResultItem const & item)
        {
          ScAddr candidate;
          item.Get(uiComponentClassAlias, candidate);
          candidates.emplace(std::move(candidate));
        });
  }
  catch (utils::ExceptionInvalidState const & e)
  {
    auto const message = std::format("{}: search by template failed: {}.", className, e.what());
    SC_LOG_ERROR(message);
    throw e;
  }

  for (auto const & candidate : candidates)
  {
    bool gotItem = false;
    ScTemplate filter;
    filter.Quintuple(
        candidate, ScType::VarCommonArc, "_unused", ScType::VarPermPosArc, HTMLTranslatorKeynodes::nrel_ui_value);
    try
    {
      context.SearchByTemplate(
          filter,
          [&](ScTemplateSearchResultItem const & item)
          {
            gotItem = true;
          });
      if (!gotItem)
      {
        type = HTMLTagMapper::GetUIComponentTypeByKeynodeHash().at(candidate.Hash());
        break;
      }
    }
    catch (utils::ExceptionInvalidState const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw e;
    }
  }
  return type;
}
}  // namespace htmlTranslationModule
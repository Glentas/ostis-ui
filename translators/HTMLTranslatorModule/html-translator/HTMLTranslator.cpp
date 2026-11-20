/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "HTMLTranslator.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <format>
#include <unordered_map>
#include <utility>

#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_debug.hpp>
#include <sc-memory/sc_template.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-memory/utils/sc_logger.hpp>

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "utils/AbstractUIModel.hpp"
#include "utils/ParameterRetriever.hpp"
#include "utils/HTMLTagMapper.hpp"

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
  auto uiLinks = GetUILinks(context, uiComponents);
  auto mappedUIComponents = ResolveUIComponents(context, uiComponents);
  auto resolvedUILinks = ResolveUILinks(context, uiLinks, mappedUIComponents);
  auto resolvedUIComponents = std::vector<UIComponent>();
  resolvedUIComponents.resize(mappedUIComponents.size());
  std::transform(
      mappedUIComponents.begin(),
      mappedUIComponents.end(),
      resolvedUIComponents.begin(),
      [](std::pair<ScAddr, UIComponent> const & p)
      {
        return p.second;
      });
  auto resultBody = BuildHTML(std::move(resolvedUIComponents), std::move(resolvedUILinks));
  auto finalHtml = HTMLTagMapper::GetFullHTML(resultBody);
  auto answerHTMLLink = context.GenerateLink();
  context.SetLinkContent(answerHTMLLink, finalHtml);
  ScAddr arcAddr = context.GenerateConnector(ScType::ConstCommonArc, interfaceNode, answerHTMLLink);
  context.GenerateConnector(ScType::ConstPermPosArc, HTMLTranslatorKeynodes::nrel_html_representation, arcAddr);
  if (!context.IsElement(answerHTMLLink))
  {
    auto const message = std::format("{}: given result link is invalid.", className);
    SC_LOG_ERROR(message);
    throw utils::ScException(utils::ExceptionInvalidParams("", message));
  }
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

std::vector<std::pair<ScAddr, ScAddr>> HTMLTranslator::GetUILinks(
    ScAgentContext & context,
    ScAddrUnorderedSet const & uiComponents)
{
  // TODO: set instead of vector
  std::vector<std::pair<ScAddr, ScAddr>> uiLinks;

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
            uiLinks.emplace_back(source, std::move(target));
          });
    }
    catch (utils::ExceptionInvalidState const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw e;
    }
  }
  return uiLinks;
}

std::vector<std::pair<ScAddr, UIComponent>> HTMLTranslator::ResolveUIComponents(
    ScAgentContext & context,
    ScAddrUnorderedSet const & uiComponents)
{
  std::vector<std::pair<ScAddr, UIComponent>> resolvedUIComponents;

  for (auto const & uiComponent : uiComponents)
  {
    auto type = ResolveUIComponentType(context, uiComponent);
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

UIComponentType HTMLTranslator::ResolveUIComponentType(ScAgentContext & context, ScAddr const & uiComponent)
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

std::vector<UILink> HTMLTranslator::ResolveUILinks(
    ScAgentContext & context,
    std::vector<std::pair<ScAddr, ScAddr>> const & uiLinks,
    std::vector<std::pair<ScAddr, UIComponent>> const & resolvedUIComponents)
{
  std::vector<UILink> resolvedUILinks;
  auto getID = [&](ScAddr const & node)
  {
    auto searcher = [&](std::pair<ScAddr, UIComponent> const & p)
    {
      if (p.first == node)
      {
        return true;
      }
      return false;
    };
    auto res_it = std::find_if(resolvedUIComponents.cbegin(), resolvedUIComponents.cend(), searcher);
    if (res_it == resolvedUIComponents.cend())
    {
      throw std::runtime_error("Link's corresponding node is not found");
    }
    return res_it->second.ID;
  };
  for (auto const & l : uiLinks)
  {
    UILink uiLink;
    uiLink.Source = getID(l.first);
    uiLink.Target = getID(l.second);
    resolvedUILinks.emplace_back(std::move(uiLink));
  }
  return resolvedUILinks;
}

std::string HTMLTranslator::RecursiveBuild(
    std::string const & id,
    std::vector<UIComponent> const & uiComponents,
    std::vector<UILink> const & uiLinks)
{
  std::string result, placeholder;

  auto generateParams = [&placeholder](std::unordered_map<UIParamType, std::string> const & params)
  {
    std::string str_representation;
    for (auto const & [type, value] : params)
    {
      if (type == UIParamType::TextValue)
      {
        placeholder += "\n" + value;
        continue;
      }
      auto processedValue = value;
      if (type == UIParamType::Visible)
      {
        processedValue = (value == "true") ? "visible" : "hidden";
      }
      auto NeedsPx = [](UIParamType type)
      {
        switch (type)
        {
        case UIParamType::X:
        case UIParamType::Y:
        case UIParamType::Width:
        case UIParamType::Height:
        case UIParamType::MinWidth:
        case UIParamType::MinHeight:
        case UIParamType::Padding:
        case UIParamType::Margin:
        case UIParamType::BorderWidth:
        case UIParamType::CornerRadius:
        case UIParamType::FontSize:
          return true;
        default:
          return false;
        }
      };
      if (NeedsPx(type))
      {
        processedValue += "px";
      }
      if ((type == UIParamType::MaxWidth || type == UIParamType::MaxHeight) && value == "0")
      {
        processedValue += "none";
      }

      auto paramTemplateIT = HTMLTagMapper::HTMLTemplateByUIParamType.find(type);

      if (paramTemplateIT != HTMLTagMapper::HTMLTemplateByUIParamType.cend())
      {
        str_representation +=
            "  " + std::vformat(paramTemplateIT->second, std::make_format_args(processedValue)) + "\n";
        continue;
      }
      SC_LOG(utils::ScLogLevel::Info, "Encountered attribute that is unsupported in HTML, safely continue");
    }
    return str_representation;
  };
  auto getChildren = [&](std::string const & id)
  {
    std::vector<std::string> childrenIDs;
    for (auto const & l : uiLinks)
    {
      if (l.Source == id)
      {
        childrenIDs.emplace_back(l.Target);
      }
    }
    return childrenIDs;
  };

  auto children = getChildren(id);

  for (auto const & c : children)
  {
    placeholder += RecursiveBuild(c, uiComponents, uiLinks) + "\n";
  }
  auto component = std::find_if(
      uiComponents.begin(),
      uiComponents.end(),
      [id = id](UIComponent const & c)
      {
        return c.ID == id;
      });
  if (component->Type == UIComponentType::Select)
  {
    auto n = 2 + 2;
  }
  auto componentTemplate = HTMLTagMapper::HTMLTemplateByUIComponentType.at(component->Type);
  auto params = generateParams(component->Params);
  // TODO: check special templates for img, sound, etc
  result = std::vformat(componentTemplate, std::make_format_args(params, placeholder));
  return result;
}

std::string HTMLTranslator::BuildHTML(std::vector<UIComponent> && uiComponents, std::vector<UILink> && uiLinks)
{
  std::string result;
  for (auto const & c : uiComponents)
  {
    bool isRoot = true;
    for (auto const & l : uiLinks)
    {
      if (l.Target == c.ID)
      {
        isRoot = false;
        break;
      }
    }
    if (isRoot)
    {
      result += RecursiveBuild(c.ID, uiComponents, uiLinks) + "\n";
    }
  };
  return result;
}
}  // namespace htmlTranslationModule
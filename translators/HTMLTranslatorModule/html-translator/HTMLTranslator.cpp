/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "HTMLTranslator.hpp"

#include <algorithm>
#include <cstddef>
#include <exception>
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
    throw std::runtime_error(message);
  }
  auto uiComponents = GetUIComponents(context, interfaceNode);
  auto uiLinks = GetUILinks(context, uiComponents);
  auto mappedUIComponents = GetMappedUIComponents(context, uiComponents);
  auto resolvedUILinks = ResolveUILinks(context, uiLinks, mappedUIComponents);
  auto resolvedUIComponents = ResolveUIComponents(std::move(mappedUIComponents));
  auto interfaceAbstractTree = BuildInterfaceAbstractTree(resolvedUILinks, resolvedUIComponents);
  auto html = BuildHTML(std::move(interfaceAbstractTree), std::move(resolvedUIComponents), std::move(resolvedUILinks));
  auto answerHTMLLink = context.GenerateLink();
  context.SetLinkContent(answerHTMLLink, html);
  ScAddr arcAddr = context.GenerateConnector(ScType::ConstCommonArc, interfaceNode, answerHTMLLink);
  context.GenerateConnector(ScType::ConstPermPosArc, HTMLTranslatorKeynodes::nrel_html_representation, arcAddr);
  if (!context.IsElement(answerHTMLLink))
  {
    auto const message = std::format("{}: given result link is invalid.", className);
    SC_LOG_ERROR(message);
    throw std::runtime_error(message);
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
  catch (std::exception const & e)
  {
    auto const message = std::format("{}: search by template failed: {}.", className, e.what());
    SC_LOG_ERROR(message);
    throw std::runtime_error(message);
  }

  return uniqueUIComponents;
}

std::vector<std::pair<ScAddr, ScAddr>> HTMLTranslator::GetUILinks(
    ScAgentContext & context,
    ScAddrUnorderedSet const & uiComponents)
{
  std::vector<std::pair<ScAddr, ScAddr>> uiLinks;

  for (auto const & source : uiComponents)
  {
    constexpr auto targetAlias = "_target";
    ScTemplate uiLinkTemplate;
    //
    // source = = = = = => _target
    //             ^
    //             |
    //
    //             |
    //
    //             |
    //
    //      nrel_ui_contains
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
    catch (std::exception const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw std::runtime_error(message);
    }
  }
  return uiLinks;
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
  catch (std::exception const & e)
  {
    auto const message = std::format("{}: search by template failed: {}.", className, e.what());
    SC_LOG_ERROR(message);
    throw std::runtime_error(message);
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
    catch (std::exception const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw std::runtime_error(message);
    }
  }
  return type;
}

std::vector<std::pair<ScAddr, UIComponent>> HTMLTranslator::GetMappedUIComponents(
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
    catch (std::exception const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw std::runtime_error(message);
    }
  }
  return resolvedUIComponents;
}

std::vector<UILink> HTMLTranslator::ResolveUILinks(
    ScAgentContext & context,
    std::vector<std::pair<ScAddr, ScAddr>> const & uiLinks,
    std::vector<std::pair<ScAddr, UIComponent>> const & mappedUIComponents)
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
    auto res_it = std::find_if(mappedUIComponents.cbegin(), mappedUIComponents.cend(), searcher);
    if (res_it == mappedUIComponents.cend())
    {
      auto const message = std::format("{}: link's corresponding node is not found.", className);
      SC_LOG_ERROR(message);
      throw std::runtime_error(message);
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

std::vector<UIComponent> HTMLTranslator::ResolveUIComponents(
    std::vector<std::pair<ScAddr, UIComponent>> && mappedUIComponents)
{
  std::vector<UIComponent> resolvedUIComponents;
  resolvedUIComponents.resize(mappedUIComponents.size());
  std::transform(
      mappedUIComponents.begin(),
      mappedUIComponents.end(),
      resolvedUIComponents.begin(),
      [](std::pair<ScAddr, UIComponent> const & p)
      {
        return p.second;
      });
  return resolvedUIComponents;
}

std::unordered_map<std::string, std::vector<std::string>> HTMLTranslator::BuildInterfaceAbstractTree(
    std::vector<UILink> const & resolvedUILinks,
    std::vector<UIComponent> const & resolvedUIComponents)
{
  std::unordered_map<std::string, std::vector<std::string>> interfaceAbstractTree;
  for (auto const & uiLink : resolvedUILinks)
  {
    interfaceAbstractTree[uiLink.Source].push_back(uiLink.Target);
  }
  return interfaceAbstractTree;
}

UIComponent HTMLTranslator::GenerateFakeRootComponent(std::vector<UIComponent> const & uiComponents)
{
  UIComponent fakeRoot;
  fakeRoot.ID = "fake_root";
  fakeRoot.Type = UIComponentType::Spacer;
  std::unordered_map<UIParamType, std::string> params{
      {UIParamType::X, "0"}, {UIParamType::Y, "0"}, {UIParamType::Width, "800"}, {UIParamType::Height, "450"}};

  size_t maxWidth = 0, maxHeight = 0;
  for (auto const & uiComponent : uiComponents)
  {
    auto x_it = uiComponent.Params.find(UIParamType::X);
    auto y_it = uiComponent.Params.find(UIParamType::Y);
    auto width_it = uiComponent.Params.find(UIParamType::Width);
    auto height_it = uiComponent.Params.find(UIParamType::Height);
    auto margin_it = uiComponent.Params.find(UIParamType::Margin);
    if (x_it == uiComponent.Params.cend() || y_it == uiComponent.Params.cend() || width_it == uiComponent.Params.cend()
        || height_it == uiComponent.Params.cend())
    {
      auto const message =
          std::format("{}: component {} does not have X, Y, Width or Height attribute.", className, uiComponent.ID);
      SC_LOG_ERROR(message);
      throw std::runtime_error(message);
    }
    try
    {
      auto margin = 0ul;
      if (margin_it != uiComponent.Params.cend())
      {
        margin = std::stoul(margin_it->second);
      }
      auto x = std::stoul(x_it->second);
      auto y = std::stoul(y_it->second);
      auto width = std::stoul(width_it->second);
      auto height = std::stoul(height_it->second);
      maxWidth = std::max(maxWidth, x + width + margin);
      maxHeight = std::max(maxHeight, y + height + margin);
    }
    catch (std::exception const & e)
    {
      auto const message =
          std::format("{}: component {} has X, Y, Width or Height with invalid value.", className, uiComponent.ID);
      SC_LOG_ERROR(message);
      throw std::runtime_error(message);
    }
  }
  params[UIParamType::Width] = std::to_string(maxWidth);
  params[UIParamType::Height] = std::to_string(maxHeight);
  fakeRoot.Params = std::move(params);
  return fakeRoot;
}

std::string HTMLTranslator::GenerateParams(
    UIComponent const & parentComponent,
    UIComponent const & rootComponent,
    std::unordered_map<UIParamType, std::string> const & params,
    std::string & text,
    std::string & actionID)
{
  std::string generatedParams;
  for (auto const & [type, value] : params)
  {
    if (type == UIParamType::TextValue)
    {
      text = value;
      continue;
    }
    if (type == UIParamType::ActionId)
    {
      actionID = value;
      continue;
    }
    auto processedValue = value;
    if (type == UIParamType::Visible)
    {
      processedValue = (value == "true") ? "visible" : "hidden";
    }
    if ((type == UIParamType::MaxWidth || type == UIParamType::MaxHeight) && value == "0")
    {
      processedValue = "none";
    }
    if (type == UIParamType::BorderWidth || type == UIParamType::CornerRadius)
    {
      processedValue += "px";
    }
    if (type == UIParamType::X)
    {
      std::string suffix = "cqw";
      auto maxV = std::stod(rootComponent.Params.at(UIParamType::Width));
      auto parentV = std::stod(parentComponent.Params.at(UIParamType::X));
      auto currentV = std::stod(value);
      auto newV = 100.0 * (currentV - parentV) / maxV;
      processedValue = std::to_string(newV) + suffix;
    }
    switch (type)
    {
    case UIParamType::Width:
    {
      std::string suffix = "cqw";
      auto maxV = std::stod(rootComponent.Params.at(UIParamType::Width));
      auto currentV = std::stod(value);
      auto newV = 100.0 * currentV / maxV;
      processedValue = std::to_string(newV) + suffix;
      break;
    }
    case UIParamType::Y:
    {
      std::string suffix = "cqh";
      auto maxV = std::stod(rootComponent.Params.at(UIParamType::Height));
      auto parentV = std::stod(parentComponent.Params.at(UIParamType::Y));
      auto currentV = std::stod(value);
      auto newV = 100.0 * (currentV - parentV) / maxV;
      processedValue = std::to_string(newV) + suffix;
      break;
    }
    case UIParamType::Height:
    {
      std::string suffix = "cqh";
      auto maxV = std::stod(rootComponent.Params.at(UIParamType::Height));
      auto currentV = std::stod(value);
      auto newV = 100.0 * currentV / maxV;
      processedValue = std::to_string(newV) + suffix;
      break;
    }
    case UIParamType::FontSize:
    {
      std::string suffix = "cqmin";
      auto maxV = std::stod(rootComponent.Params.at(UIParamType::Width))
                  + std::stod(rootComponent.Params.at(UIParamType::Height));
      auto currentV = 2.0 * std::stod(value);
      auto newV = 100.0 * currentV / maxV;
      processedValue = std::to_string(newV) + suffix;
      break;
    }
    case UIParamType::Margin:
    {
      std::string suffix = "cqmin";
      auto maxV = std::stod(rootComponent.Params.at(UIParamType::Width))
                  + std::stod(rootComponent.Params.at(UIParamType::Height));
      auto currentV = std::stod(value);
      auto newV = 100.0 * currentV / maxV;
      processedValue = std::to_string(newV) + suffix;
      break;
    }
    case UIParamType::Padding:
    {
      std::string suffix = "cqmin";
      auto maxV = std::stod(rootComponent.Params.at(UIParamType::Width))
                  + std::stod(rootComponent.Params.at(UIParamType::Height));
      auto currentV = std::stod(value);
      auto newV = 100.0 * currentV / maxV;
      processedValue = std::to_string(newV) + suffix;
      break;
    }
    default:
    {
      break;
    }
    }
    auto paramTemplateIT = HTMLTagMapper::HTMLTemplateByUIParamType.find(type);

    if (paramTemplateIT != HTMLTagMapper::HTMLTemplateByUIParamType.cend())
    {
      generatedParams += "    " + std::vformat(paramTemplateIT->second, std::make_format_args(processedValue)) + "\n";
      continue;
    }
    SC_LOG(utils::ScLogLevel::Info, "Encountered attribute that is not supported in HTML, safely continue");
  }
  return generatedParams;
}

std::string HTMLTranslator::GenerateHTMLComponent(
    std::string const & componentID,
    std::string const & parentID,
    std::string const & rootID,
    std::unordered_map<std::string, std::vector<std::string>> const & interfaceAbstractTree,
    std::unordered_map<std::string, UIComponent> const & uiComponentByID,
    std::unordered_set<std::string> & actionIDs)
{
  std::string placeholder, text, actionID;
  auto const & children_it = interfaceAbstractTree.find(componentID);
  if (children_it != interfaceAbstractTree.cend())
  {
    for (auto const & childID : children_it->second)
    {
      auto childHTML =
          GenerateHTMLComponent(childID, componentID, rootID, interfaceAbstractTree, uiComponentByID, actionIDs);
      if (uiComponentByID.at(componentID).Type == UIComponentType::Select)
      {
        placeholder += HTMLTagMapper::WrapOptionComponent(
            childID, uiComponentByID.at(componentID).Params.at(UIParamType::ActionId), childHTML);
      }
      else
      {
        placeholder += childHTML;
      }
    }
  }
  auto component = uiComponentByID.at(componentID);
  auto componentTemplate = HTMLTagMapper::HTMLTemplateByUIComponentType.at(component.Type);
  auto params =
      GenerateParams(uiComponentByID.at(parentID), uiComponentByID.at(rootID), component.Params, text, actionID);
  actionIDs.insert(actionID);
  if (uiComponentByID.at(componentID).Type == UIComponentType::Button)
  {
    placeholder += HTMLTagMapper::CreateSenderScript(componentID, actionID, text) + "\n";
  }
  placeholder += text;
  return std::vformat(componentTemplate, std::make_format_args(componentID, params, placeholder));
}

std::string HTMLTranslator::BuildHTML(
    std::unordered_map<std::string, std::vector<std::string>> && interfaceAbstractTree,
    std::vector<UIComponent> && uiComponents,
    std::vector<UILink> && uiLinks)
{
  std::string result;
  std::unordered_set<std::string> rootComponents;
  std::unordered_map<std::string, UIComponent> uiComponentByID;
  std::unordered_set<std::string> actionIDs;
  auto fakeRoot = GenerateFakeRootComponent(uiComponents);
  uiComponentByID.emplace(fakeRoot.ID, fakeRoot);
  for (auto & uiComponent : uiComponents)
  {
    bool isRoot = true;
    for (auto const & uiLink : uiLinks)
    {
      if (uiLink.Target == uiComponent.ID)
      {
        isRoot = false;
        break;
      }
    }
    if (isRoot)
    {
      rootComponents.insert(uiComponent.ID);
    }
    auto ID = uiComponent.ID;
    uiComponentByID.emplace(std::move(ID), std::move(uiComponent));
  }
  if (rootComponents.empty())
  {
    auto const message = std::format("{}: no root components of interface were found.", className);
    SC_LOG_ERROR(message);
    throw std::runtime_error(message);
  }
  for (auto const & rootComponent : rootComponents)
  {
    result += GenerateHTMLComponent(
        rootComponent, fakeRoot.ID, fakeRoot.ID, interfaceAbstractTree, uiComponentByID, actionIDs);
  }
  for (auto const & actionID : actionIDs)
  {
    result += HTMLTagMapper::CreateRecieverScript(actionID) + "\n";
  }
  return HTMLTagMapper::GetFullHTML(result);
}
}  // namespace htmlTranslationModule
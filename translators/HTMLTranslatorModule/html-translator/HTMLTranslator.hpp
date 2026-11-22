/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>

#include "utils/AbstractUIModel.hpp"

namespace htmlTranslationModule
{
class HTMLTranslator
{
public:
  static ScAddr TranslateSCToHTML(ScAgentContext & context, ScAddr const & interfaceNode);

private:
  static ScAddrUnorderedSet GetUIComponents(ScAgentContext & context, ScAddr const & interfaceNode);
  static std::vector<std::pair<ScAddr, ScAddr>> GetUILinks(
      ScAgentContext & context,
      ScAddrUnorderedSet const & uiComponents);
  static UIComponentType ResolveUIComponentType(ScAgentContext & context, ScAddr const & uiComponent);
  static std::vector<std::pair<ScAddr, UIComponent>> GetMappedUIComponents(
      ScAgentContext & context,
      ScAddrUnorderedSet const & uiComponents);
  static std::vector<UILink> ResolveUILinks(
      ScAgentContext & context,
      std::vector<std::pair<ScAddr, ScAddr>> const & uiLinks,
      std::vector<std::pair<ScAddr, UIComponent>> const & mappedUIComponents);
  static std::vector<UIComponent> ResolveUIComponents(
      std::vector<std::pair<ScAddr, UIComponent>> && mappedUIComponents);
  static std::unordered_map<std::string, std::vector<std::string>> BuildInterfaceAbstractTree(
      std::vector<UILink> const & resolvedUILinks,
      std::vector<UIComponent> const & resolvedUIComponents);
  // ---------------------------
  static UIComponent GenerateFakeRootComponent(std::vector<UIComponent> const & uiComponents);
  static std::string GenerateParams(
      UIComponent const & parentComponent,
      UIComponent const & rootComponent,
      std::unordered_map<UIParamType, std::string> const & params,
      std::string & text,
      std::string & actionID);
  static std::string GenerateHTMLComponent(
      std::string const & componentID,
      std::string const & parentID,
      std::string const & rootID,
      std::unordered_map<std::string, std::vector<std::string>> const & interfaceAbstractTree,
      std::unordered_map<std::string, UIComponent> const & uiComponentByID,
      std::unordered_set<std::string> & actionIDs);
  static std::string BuildHTML(
      std::unordered_map<std::string, std::vector<std::string>> && interfaceAbstractTree,
      std::vector<UIComponent> && uiComponents,
      std::vector<UILink> && uiLinks);

private:
  constexpr inline static std::string_view className = "HTMLTranslator";
};

}  // namespace htmlTranslationModule

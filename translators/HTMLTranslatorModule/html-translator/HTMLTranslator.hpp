/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

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
  static std::vector<std::pair<ScAddr, UIComponent>> ResolveUIComponents(
      ScAgentContext & context,
      ScAddrUnorderedSet const & uiComponents);
  static UIComponentType ResolveUIComponentType(ScAgentContext & context, ScAddr const & uiComponent);
  static std::vector<UILink> ResolveUILinks(
      ScAgentContext & context,
      std::vector<std::pair<ScAddr, ScAddr>> const & uiLinks,
      std::vector<std::pair<ScAddr, UIComponent>> const & resolvedUIComponents);
  static std::string RecursiveBuild(
      std::string const & id,
      std::vector<UIComponent> const & uiComponents,
      std::vector<UILink> const & uiLinks);
  static std::string BuildHTML(std::vector<UIComponent> && uiComponents, std::vector<UILink> && uiLinks);

private:
  constexpr inline static std::string_view className = "HTMLTranslator";
};

}  // namespace htmlTranslationModule

/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>
#include <map>
#include <string>
#include <string_view>

namespace htmlTranslationModule
{
using StringStringMap = std::map<std::string, std::string>;
using StringScAddrMap = std::map<std::string, ScAddr>;

class HTMLTranslator
{
public:
  static ScAddr TranslateSCToHTML(ScAgentContext & context, ScAddr const & rootUiElement);
  static ScAddr GetUIComponentHTMLTemplate(ScAgentContext & context, ScAddr const & uiComponent);
  static ScAddr GetAnswerLink(
      ScAgentContext & context,
      ScAddr const & uiElement,
      ScAddr const & componentHTMLTemplateLink);
  static StringStringMap GetNestedComponentsHTMLRepresentation(
      ScAgentContext & context,
      StringScAddrMap const & nestedComponents);
  static void InsertParameterValue(
      std::string & componentTemplateString,
      std::string const & parameterID,
      std::string const & parameterValue);

private:
  constexpr inline static std::string_view className = "HTMLTranslator";
};

}  // namespace htmlTranslationModule

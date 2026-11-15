/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <map>
#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>
#include <string>

using namespace utils;

namespace htmlTranslationModule
{

using StringScAddrMap = std::map<std::string, ScAddr>;

class ParameterRetriever
{
public:
  static StringScAddrMap GetNestedUIComponents(ScAgentContext & context, ScAddr const & uiComponent);

  static ScTemplate GetComponentClassTemplate(ScAgentContext & context, ScAddr const & uiComponent);
};

}  // namespace htmlTranslationModule
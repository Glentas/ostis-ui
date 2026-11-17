/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <unordered_map>
#include <string>
#include <string_view>

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>

#include "utils/AbstractUIModel.hpp"

using namespace utils;

namespace htmlTranslationModule
{

class ParameterRetriever
{
public:
  static std::unordered_map<UIParamType, std::string> GetUIComponentParams(
      ScAgentContext & context,
      ScAddr const & uiComponent);

private:
  static ScAddrUnorderedSet GetUIComponentRawParams(ScAgentContext & context, ScAddr const & uiComponent);

private:
  static constexpr inline std::string_view className = "ParameterRetriever";
};

}  // namespace htmlTranslationModule
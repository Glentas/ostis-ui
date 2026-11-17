/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "ParameterRetriever.hpp"

#include "utils/AbstractUIModel.hpp"
#include "utils/HTMLTagMapper.hpp"
#include <format>
#include <string>

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_keynodes.hpp>
#include <sc-memory/sc_template.hpp>
#include <sc-memory/sc_type.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include <unordered_map>

using namespace utils;

namespace htmlTranslationModule
{

std::unordered_map<UIParamType, std::string> ParameterRetriever::GetUIComponentParams(
    ScAgentContext & context,
    ScAddr const & uiComponent)
{
  std::unordered_map<UIParamType, std::string> params;

  auto const rawUIParams = GetUIComponentRawParams(context, uiComponent);

  auto constexpr uiComponentParamLinkAlias = "_ui_param_link";
  auto constexpr uiComponentParamClassAlias = "_ui_param_class";

  for (auto const & uiParam : rawUIParams)
  {
    ScTemplate componentParamTemplate;
    //
    // uiParamClass - - - - - -> uiParam
    //
    componentParamTemplate.Triple(uiComponentParamClassAlias, ScType::VarPermPosArc, uiParam);

    //
    // uiParam = = = = = => _param_content_link
    //                ^
    //                |
    //
    //                |
    //
    //                |
    //
    //          nrel_ui_value
    //
    componentParamTemplate.Quintuple(
        uiParam,
        ScType::VarCommonArc,
        uiComponentParamLinkAlias,
        ScType::VarPermPosArc,
        HTMLTranslatorKeynodes::nrel_ui_value);

    try
    {
      context.SearchByTemplateInterruptibly(
          componentParamTemplate,
          [&](ScTemplateSearchResultItem const & item)
          {
            ScAddr uiParamValue;
            ScAddr uiParamType;
            item.Get(uiComponentParamLinkAlias, uiParamValue);
            item.Get(uiComponentParamClassAlias, uiParamType);
            auto type = HTMLTagMapper::GetUIParamTypeByKeynodeHash().at(uiParamType.Hash());
            std::string value;
            context.GetLinkContent(uiParamValue, value);
            params.emplace(type, std::move(value));
            return ScTemplateSearchRequest::STOP;
          });
    }
    catch (utils::ExceptionInvalidState const & e)
    {
      auto const message = std::format("{}: search by template failed: {}.", className, e.what());
      SC_LOG_ERROR(message);
      throw e;
    }
  }
  return params;
}

ScAddrUnorderedSet ParameterRetriever::GetUIComponentRawParams(ScAgentContext & context, ScAddr const & uiComponent)
{
  ScAddrUnorderedSet rawUIParams;

  ScTemplate rawUIParamTemplate;
  auto constexpr uiParamAlias = "_ui_param";

  //
  // uiParam - - - - - -> uiComponent
  //
  rawUIParamTemplate.Triple(uiParamAlias, ScType::VarPermPosArc, uiComponent);

  //
  // uiParam = = = = = => _param_content_link
  //                ^
  //                |
  //
  //                |
  //
  //                |
  //
  //          nrel_ui_value
  //
  rawUIParamTemplate.Quintuple(
      uiParamAlias,
      ScType::VarCommonArc,
      ScType::VarNodeLink,
      ScType::VarPermPosArc,
      HTMLTranslatorKeynodes::nrel_ui_value);

  try
  {
    context.SearchByTemplate(
        rawUIParamTemplate,
        [&](ScTemplateSearchResultItem const & item)
        {
          ScAddr uiParam;
          item.Get(uiParamAlias, uiParam);
          rawUIParams.insert(std::move(uiParam));
        });
  }
  catch (utils::ExceptionInvalidState const & e)
  {
    auto const message = std::format("{}: search by template failed: {}.", className, e.what());
    SC_LOG_ERROR(message);
    throw e;
  }
  return rawUIParams;
}
}  // namespace htmlTranslationModule
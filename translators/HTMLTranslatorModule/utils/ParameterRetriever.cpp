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

std::unordered_map<ParamType, std::string> ParameterRetriever::GetUIComponentParams(
    ScAgentContext & context,
    ScAddr const & uiComponent)
{
  std::unordered_map<ParamType, std::string> params;

  auto const rawParams = GetUIComponentRawParams(context, uiComponent);

  auto constexpr componentParamLinkAlias = "_ui_param_link";
  auto constexpr componentParamClassAlias = "_ui_param_class";

  for (auto const & param : rawParams)
  {
    ScTemplate componentParamTemplate;
    //
    // uiParamClass - - - - - -> uiParam
    //
    componentParamTemplate.Triple(componentParamClassAlias, ScType::VarPermPosArc, param);

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
        param,
        ScType::VarCommonArc,
        componentParamLinkAlias,
        ScType::VarPermPosArc,
        HTMLTranslatorKeynodes::nrel_ui_value);

    try
    {
      context.SearchByTemplateInterruptibly(
          componentParamTemplate,
          [&componentParamLinkAlias, &componentParamClassAlias, &params, &context](
              ScTemplateSearchResultItem const & item)
          {
            ScAddr paramValue;
            ScAddr paramType;
            item.Get(componentParamLinkAlias, paramValue);
            item.Get(componentParamClassAlias, paramType);
            auto type = HTMLTagMapper::GetParamTypeByKeynodeHash().at(paramType.Hash());
            std::string value;
            context.GetLinkContent(paramValue, value);
            params.emplace(type, value);
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

ScAddrSet ParameterRetriever::GetUIComponentRawParams(ScAgentContext & context, ScAddr const & uiComponent)
{
  ScAddrSet rawParams;

  ScTemplate componentRawParamTemplate;
  auto constexpr componentParamAlias = "_ui_param";

  //
  // uiParam - - - - - -> uiComponent
  //
  componentRawParamTemplate.Triple(componentParamAlias, ScType::VarPermPosArc, uiComponent);

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
  componentRawParamTemplate.Quintuple(
      componentParamAlias,
      ScType::VarCommonArc,
      ScType::VarNodeLink,
      ScType::VarPermPosArc,
      HTMLTranslatorKeynodes::nrel_ui_value);

  try
  {
    context.SearchByTemplate(
        componentRawParamTemplate,
        [&componentParamAlias, &rawParams](ScTemplateSearchResultItem const & item)
        {
          ScAddr uiParam;
          item.Get(componentParamAlias, uiParam);
          rawParams.insert(std::move(uiParam));
        });
  }
  catch (utils::ExceptionInvalidState const & e)
  {
    auto const message = std::format("{}: search by template failed: {}.", className, e.what());
    SC_LOG_ERROR(message);
    throw e;
  }
  return rawParams;
}
}  // namespace htmlTranslationModule
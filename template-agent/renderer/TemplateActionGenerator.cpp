/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "TemplateActionGenerator.hpp"

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include <sc-agents-common/utils/GenerationUtils.hpp>

#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"

using namespace utils;

namespace specifiedStringTemplateModule
{

ScResult GenerateTemplateAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [button] = action.GetArguments<1>();
  SC_LOG_INFO("ЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁЁ ");

  if (!m_context.IsElement(button))
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Button id not found");
  }

  std::string button_id;
  m_context.GetLinkContent(button, button_id);
  ScAddr buttonAddr = m_context.SearchElementBySystemIdentifier(button_id);

  if (!buttonAddr.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Button node not found: " + button_id);
  }

  // ScAddr const buttonAddr = IteratorUtils::getAnyByInRelation( // берем узел кнопки через айди
  // &m_context, button, SpecifiedStringTemplateKeynodes::nrel_system_identifier);

  // button =================> idtf
  //                 ^
  //                 |
  //                 |
  //        nrel_system_identifier

  ScAddr const variablesSetAddr = IteratorUtils::getAnyByOutRelation( // берем узел связывающий шаблон действия кнопки и параметры
        &m_context, buttonAddr, SpecifiedStringTemplateKeynodes::nrel_action_template);

  SC_LOG_INFO("variablesSetAddr valid: " + std::to_string(variablesSetAddr.IsValid()));

  if (!variablesSetAddr.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "nrel_action_template not found for button: " + button_id);
  }

  ScTemplate scTemplate;
  ScAddr templateAddr;
  ScTemplateParams params;

  ScIterator3Ptr it3 = m_context.CreateIterator3(variablesSetAddr, ScType::ConstPermPosArc, ScType::ConstNodeStructure);

  if (it3->Next())
  {
    templateAddr = it3->Get(2);  // берем структуру шаблона
    if (!m_context.IsElement(templateAddr))
    {
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Template for action not found");
    }
  }

  params = GetScTemplateParamFromTemplateReplacement(m_context, variablesSetAddr);

  m_context.BuildTemplate(scTemplate, templateAddr, params);

  SC_LOG_INFO("GenerateTemplateAgent finished");
  return action.FinishSuccessfully();
}

ScTemplateParams GenerateTemplateAgent::GetScTemplateParamFromTemplateReplacement(
    ScAgentContext & context,
    ScAddr const & variablesSetAddr)
{
  ScAddr const varSetInputParams = IteratorUtils::getAnyByOutRelation(  //получили узел с указанием параметров
        &context, variablesSetAddr, SpecifiedStringTemplateKeynodes::rrel_input_params);

  ScAddr const varSetComponentInputParams = IteratorUtils::getAnyByOutRelation( //получили узел с параметрами
        &context, variablesSetAddr, SpecifiedStringTemplateKeynodes::rrel_component_with_input);

  ScIterator5Ptr it5_for_param = context.CreateIterator5(
      ScType::VarNode, ScType::VarPermPosArc, ScType::Unknown, ScType::ConstPermPosArc, varSetInputParams);

  ScAddr replacementVariable;
  ScTemplateParams params;

  while (it5_for_param->Next())
  {
    replacementVariable = it5_for_param->Get(2);  // при помощи итератора для элемента получаем переменную из шаблона

    // итератор для поиска ролевого отношения в шаблоне, что найти соответствующее для него значение
    ScIterator5Ptr it5_for_relation_finding = context.CreateIterator5(
        ScType::VarNode, ScType::VarPermPosArc, replacementVariable, ScType::VarPermPosArc, ScType::ConstNodeRole);

    ScAddr relation_node;
    if (it5_for_relation_finding->Next())
    {
      relation_node = it5_for_relation_finding->Get(4);  // получили отношение
    }

    ScIterator5Ptr it5_for_param_value = context.CreateIterator5(  // итератор для поиска значений параметров через полученное отношение
        varSetComponentInputParams,
        ScType::ConstPermPosArc,
        ScType::Unknown,
        ScType::VarPermPosArc,
        relation_node);

    ScAddr param_node;  // для различных параметров

    if (it5_for_param_value->Next())
    {
      param_node = it5_for_param_value->Get(2);
      ScAddr const replacementValueLink = IteratorUtils::getAnyByOutRelation(  // получаем значение
            &context, param_node, SpecifiedStringTemplateKeynodes::nrel_value);

      if (context.IsElement(replacementVariable) && context.IsElement(replacementValueLink))  // подставляем значение
      {
        params.Add(replacementVariable, replacementValueLink);
      }
      else if (context.IsElement(param_node))
      {  // это если нету значения и надо просто ноду подставить УТОЧНИТЬ!!!
        params.Add(replacementVariable, param_node);
      }
      else
      {
        SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Param not found");
      }
    }
  }

  return params;
}

ScAddr GenerateTemplateAgent::GetActionClass() const
{
  return SpecifiedStringTemplateKeynodes::action_generate_template;
}

}  // namespace specifiedStringTemplateModule

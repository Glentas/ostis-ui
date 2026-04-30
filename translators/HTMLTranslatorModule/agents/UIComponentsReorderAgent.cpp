/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

/*
 * Назначение:
 *   Изменить порядок (позиции) двух дочерних UI-компонентов внутри
 *   родительского компонента путём переподключения их nrel_html_parameter_id
 *   в SC-памяти с последующей принудительной перегенерацией HTML родителя.
 *
 * Архитектурное обоснование:
 *   До свапа:
 *     comp_A ===[arc_A]===> link("slot1")
 *                  ^
 *                  |
 *       nrel_html_parameter_id
 *
 *     comp_B ===[arc_B]===> link("slot2")
 *                  ^
 *                  |
 *       nrel_html_parameter_id
 *
 *   После свапа:
 *     comp_A ===[new_arc_A]===> link("slot2")   <- была ссылка comp_B
 *                    ^
 *                    |
 *       nrel_html_parameter_id
 *
 *     comp_B ===[new_arc_B]===> link("slot1")   <- была ссылка comp_A
 *                    ^
 *                    |
 *       nrel_html_parameter_id
 *
 *   При EraseElement(arc_A) sc-machine автоматически удаляет все дуги,
 *   инцидентные arc_A — в том числе PermPosArc от nrel_html_parameter_id.
 */

#include "UIComponentsReorderAgent.hpp"

#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_event.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-memory/sc_structure.hpp>
#include <sc-memory/sc_template.hpp>

#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "html-translator/HTMLTranslator.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "parameter-retriever/ParameterRetriever.hpp"

using namespace utils;

namespace htmlTranslationModule
{

// ---------------------------------------------------------------------------
// Вспомогательная структура: адреса дуг для одного компонента
// ---------------------------------------------------------------------------
struct ParameterIdArcPair
{
  ScAddr commonArc;   // CommonArc: component ===> idLink
  ScAddr idLink;      // ScLink с ID (например "slot1")
};

// ---------------------------------------------------------------------------
// FindParameterIdArcs
//
// Ищет квинтет:
//   component ===[commonArc]===> idLink
//                    ^
//                    |
//         nrel_html_parameter_id
//
// Возвращает адреса commonArc и idLink.
// Если квинтет не найден — возвращает структуру с невалидными ScAddr.
// ---------------------------------------------------------------------------
static ParameterIdArcPair FindParameterIdArcs(
    ScAgentContext & ctx,
    ScAddr const & component,
    ScAddr const & nrelParamId)
{
  ParameterIdArcPair result;

  std::string const arcAlias  = "_param_id_arc";
  std::string const linkAlias = "_param_id_link";

  ScTemplate tmpl;
  tmpl.Quintuple(
      component,
      ScType::VarCommonArc  >> arcAlias,
      ScType::VarNodeLink   >> linkAlias,
      ScType::VarPermPosArc,
      nrelParamId);

  ctx.SearchByTemplateInterruptibly(
      tmpl,
      [&](ScTemplateSearchResultItem const & item)
      {
        item.Get(arcAlias,  result.commonArc);
        item.Get(linkAlias, result.idLink);
        return ScTemplateSearchRequest::STOP;
      });

  return result;
}

// ---------------------------------------------------------------------------
// ReconnectParameterId
//
// Удаляет старую дугу commonArc (вместе с ней sc-machine удалит
// PermPosArc от nrel_html_parameter_id) и создаёт новую пятёрку:
//   component ===[newArc]===> newIdLink
//                  ^
//                  |
//     nrel_html_parameter_id
//
// Возвращает адрес новой дуги (commonArc).
// ---------------------------------------------------------------------------
static ScAddr ReconnectParameterId(
    ScAgentContext & ctx,
    ScAddr const & component,
    ScAddr const & oldCommonArc,
    ScAddr const & newIdLink,
    ScAddr const & nrelParamId)
{
  // Отсоединяем старую дугу.
  // Все дуги, инцидентные oldCommonArc (в том числе PermPosArc от nrel),
  // удаляются sc-machine автоматически.
  ctx.EraseElement(oldCommonArc);

  // Подсоединяем к новой ссылке
  ScAddr newCommonArc = ctx.GenerateConnector(ScType::CommonArc, component, newIdLink);
  ctx.GenerateConnector(ScType::PermPosArc, nrelParamId, newCommonArc);

  return newCommonArc;
}

// ---------------------------------------------------------------------------
// DoProgram
// ---------------------------------------------------------------------------
ScResult UIComponentsReorderAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  // ------------------------------------------------------------------
  // 1. Получаем аргументы
  // ------------------------------------------------------------------
  auto const [parentComponent, firstIdLink, secondIdLink] = action.GetArguments<3>();

  if (!parentComponent.IsValid())
  {
    SC_LOG_ERROR("UIComponentsReorderAgent: parent component argument is invalid.");
    return action.FinishUnsuccessfully();
  }
  if (!firstIdLink.IsValid() || !secondIdLink.IsValid())
  {
    SC_LOG_ERROR("UIComponentsReorderAgent: parameter ID link arguments are invalid.");
    return action.FinishUnsuccessfully();
  }

  // ------------------------------------------------------------------
  // 2. Читаем строки ID из ссылок-аргументов
  // ------------------------------------------------------------------
  std::string firstId, secondId;
  m_context.GetLinkContent(firstIdLink, firstId);
  m_context.GetLinkContent(secondIdLink, secondId);

  if (firstId.empty())
  {
    SC_LOG_ERROR("UIComponentsReorderAgent: first parameter ID is empty.");
    return action.FinishUnsuccessfully();
  }
  if (secondId.empty())
  {
    SC_LOG_ERROR("UIComponentsReorderAgent: second parameter ID is empty.");
    return action.FinishUnsuccessfully();
  }
  if (firstId == secondId)
  {
    SC_LOG_WARNING("UIComponentsReorderAgent: both parameter IDs are equal — nothing to reorder.");
    ScAddr currentRepr = IteratorUtils::getAnyByOutRelation(
        &m_context, parentComponent, HTMLTranslatorKeynodes::nrel_html_representation);
    if (m_context.IsElement(currentRepr))
    {
      ScStructure result = m_context.GenerateStructure();
      result << currentRepr;
      action.SetResult(result);
    }
    return action.FinishSuccessfully();
  }

  // ------------------------------------------------------------------
  // 3. Получаем карту дочерних компонентов родителя: ID → ScAddr
  // ------------------------------------------------------------------
  StringScAddrMap nestedComponents;
  try
  {
    nestedComponents = ParameterRetriever::GetNestedUIComponents(m_context, parentComponent);
  }
  catch (utils::ScException const &)
  {
    SC_LOG_ERROR("UIComponentsReorderAgent: failed to retrieve nested components of parent.");
    return action.FinishUnsuccessfully();
  }

  // ------------------------------------------------------------------
  // 4. Находим оба компонента по их ID
  // ------------------------------------------------------------------
  auto it1 = nestedComponents.find(firstId);
  if (it1 == nestedComponents.end())
  {
    SC_LOG_ERROR(
        "UIComponentsReorderAgent: component with parameter_id='" + firstId + "' not found.");
    return action.FinishUnsuccessfully();
  }

  auto it2 = nestedComponents.find(secondId);
  if (it2 == nestedComponents.end())
  {
    SC_LOG_ERROR(
        "UIComponentsReorderAgent: component with parameter_id='" + secondId + "' not found.");
    return action.FinishUnsuccessfully();
  }

  ScAddr const comp1 = it1->second;
  ScAddr const comp2 = it2->second;

  // ------------------------------------------------------------------
  // 5. Находим адреса дуг и ссылок nrel_html_parameter_id
  //    для каждого компонента
  // ------------------------------------------------------------------
  ScAddr const nrelParamId = HTMLTranslatorKeynodes::nrel_html_parameter_id;

  ParameterIdArcPair pair1 = FindParameterIdArcs(m_context, comp1, nrelParamId);
  ParameterIdArcPair pair2 = FindParameterIdArcs(m_context, comp2, nrelParamId);

  if (!m_context.IsElement(pair1.commonArc) || !m_context.IsElement(pair1.idLink))
  {
    SC_LOG_ERROR("UIComponentsReorderAgent: nrel_html_parameter_id arc not found for first component.");
    return action.FinishUnsuccessfully();
  }
  if (!m_context.IsElement(pair2.commonArc) || !m_context.IsElement(pair2.idLink))
  {
    SC_LOG_ERROR("UIComponentsReorderAgent: nrel_html_parameter_id arc not found for second component.");
    return action.FinishUnsuccessfully();
  }

  // Сохраняем ссылки до удаления дуг — они понадобятся для переподключения и отката
  ScAddr const link1 = pair1.idLink;  // link("slot1") — сейчас у comp1
  ScAddr const link2 = pair2.idLink;  // link("slot2") — сейчас у comp2

  // ------------------------------------------------------------------
  // 6. Переподключаем: удаляем старые дуги, создаём новые
  //
  //   comp1 ===> link1  →  comp1 ===> link2
  //   comp2 ===> link2  →  comp2 ===> link1
  // ------------------------------------------------------------------

  // Сначала удаляем обе старые дуги
  m_context.EraseElement(pair1.commonArc);
  m_context.EraseElement(pair2.commonArc);

  // Подключаем comp1 к link2
  ScAddr const newArc1 = m_context.GenerateConnector(ScType::CommonArc, comp1, link2);
  m_context.GenerateConnector(ScType::PermPosArc, nrelParamId, newArc1);

  // Подключаем comp2 к link1
  ScAddr const newArc2 = m_context.GenerateConnector(ScType::CommonArc, comp2, link1);
  m_context.GenerateConnector(ScType::PermPosArc, nrelParamId, newArc2);

  SC_LOG_DEBUG(
      "UIComponentsReorderAgent: reconnected parameter IDs '"
      + firstId + "' <-> '" + secondId + "'.");

  // ------------------------------------------------------------------
  // 7. Принудительно перегенерируем HTML родителя (минуя кэш)
  // ------------------------------------------------------------------
  ScAddr updatedReprLink;
  try
  {
    updatedReprLink = HTMLTranslator::RegenerateHTMLRepresentation(m_context, parentComponent);
  }
  catch (utils::ScException const &)
  {
    // Откат: удаляем новые дуги и восстанавливаем старые
    m_context.EraseElement(newArc1);
    m_context.EraseElement(newArc2);

    ScAddr rollbackArc1 = m_context.GenerateConnector(ScType::CommonArc, comp1, link1);
    m_context.GenerateConnector(ScType::PermPosArc, nrelParamId, rollbackArc1);

    ScAddr rollbackArc2 = m_context.GenerateConnector(ScType::CommonArc, comp2, link2);
    m_context.GenerateConnector(ScType::PermPosArc, nrelParamId, rollbackArc2);

    SC_LOG_ERROR("UIComponentsReorderAgent: HTML regeneration failed. Reconnection rolled back.");
    return action.FinishUnsuccessfully();
  }

  if (!m_context.IsElement(updatedReprLink))
  {
    // Откат
    m_context.EraseElement(newArc1);
    m_context.EraseElement(newArc2);

    ScAddr rollbackArc1 = m_context.GenerateConnector(ScType::CommonArc, comp1, link1);
    m_context.GenerateConnector(ScType::PermPosArc, nrelParamId, rollbackArc1);

    ScAddr rollbackArc2 = m_context.GenerateConnector(ScType::CommonArc, comp2, link2);
    m_context.GenerateConnector(ScType::PermPosArc, nrelParamId, rollbackArc2);

    SC_LOG_ERROR("UIComponentsReorderAgent: regenerated HTML link is invalid. Rolled back.");
    return action.FinishUnsuccessfully();
  }

  // ------------------------------------------------------------------
  // 8. Формируем результат
  // ------------------------------------------------------------------
  ScStructure result = m_context.GenerateStructure();
  result << updatedReprLink;
  action.SetResult(result);

  SC_LOG_INFO(
      "UIComponentsReorderAgent: components '" + firstId + "' and '" + secondId
      + "' successfully reordered.");

  return action.FinishSuccessfully();
}

ScAddr UIComponentsReorderAgent::GetActionClass() const
{
  return HTMLTranslatorKeynodes::action_reorder_ui_components;
}

}  // namespace htmlTranslationModule
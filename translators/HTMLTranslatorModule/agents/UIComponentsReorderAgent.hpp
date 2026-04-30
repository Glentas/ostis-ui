/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_agent.hpp>

namespace htmlTranslationModule
{

/**
 * @brief Агент изменения порядка UI-компонентов внутри родительского элемента.
 *
 * Действие: action_reorder_ui_components
 *
 * Аргументы (через rrel_1 / rrel_2 / rrel_3):
 *   1. parentComponent — ScAddr родительского UI-компонента.
 *   2. firstIdLink     — ScLink, содержащий строку nrel_html_parameter_id первого  дочернего компонента.
 *   3. secondIdLink    — ScLink, содержащий строку nrel_html_parameter_id второго дочернего компонента.
 *
 * Поведение:
 *   - Находит два дочерних компонента по их nrel_html_parameter_id.
 *   - Меняет значения их ID-ссылок местами в SC-памяти.
 *   - Вызывает HTMLTranslator::RegenerateHTMLRepresentation для родителя,
 *     чтобы перестроить HTML без использования кэша.
 *   - При ошибке откатывает своп ID-ссылок.
 *
 * Результат: структура, содержащая обновлённую ScLink с HTML родительского компонента.
 */
class UIComponentsReorderAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
};

}  // namespace htmlTranslationModule

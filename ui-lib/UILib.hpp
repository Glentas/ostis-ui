/*
This source file is part of an OSTIS project.
Distributed under the MIT License.
*/
#pragma once

#include <sc-memory/sc_agent_context.hpp>
#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_structure.hpp>
#include <string>
#include <optional>

namespace htmlTranslationModule::UILib
{

/**
 * @brief Найти узел модели интерфейса по системному идентификатору или pretty-имени
 * @param context Контекст для работы с базой знаний
 * @param identifier Системный идентификатор (например, "concept_current_ostis_ui_model")
 * @return ScAddr узла или пустой адрес, если не найдено
 */
ScAddr FindUIModelByIdentifier(
    ScAgentContext & context,
    std::string const & identifier);

/**
 * @brief Запустить агент трансляции SC-модели в HTML
 * @param context Контекст для работы с агентами
 * @param uiComponent Адрес компонента интерфейса для трансляции
 * @param timeout_ms Таймаут ожидания завершения агента (по умолчанию 500 мс)
 * @return Структура результата агента
 * @throws std::runtime_error если агент завершился с ошибкой
 */
ScStructure InvokeTranslationAgent(
    ScAgentContext & context,
    ScAddr const & uiComponent,
    sc_uint32 timeout_ms = 500);

/**
 * @brief Извлечь ScLink с HTML-контентом из структуры результата агента
 * @param context Контекст для работы с базой знаний
 * @param resultStruct Структура результата агента
 * @return Адрес ссылки с HTML или пустой адрес, если не найдено
 */
ScAddr ExtractHTMLLinkFromResult(
    ScAgentContext & context,
    ScStructure const & resultStruct);

/**
 * @brief Полная цепочка: найти модель → запустить трансляцию → получить HTML
 * @param context Контекст для всех операций
 * @param modelIdentifier Идентификатор модели (например, "concept_current_ostis_ui_model")
 * @param timeout_ms Таймаут для агента
 * @return Строка с HTML-контентом или пустая строка при ошибке
 */
std::optional<std::string> GetHTMLForModel(
    ScAgentContext & context,
    std::string const & modelIdentifier,
    sc_uint32 timeout_ms = 500);

/**
 * @brief Запустить агент визуальной адаптации для компонента
 * @param context Контекст для работы с агентами
 * @param componentIdentifier Системный идентификатор компонента (например, "my_button_instance")
 * @param multiplier Множитель масштабирования (1.5 = увеличение на 50%)
 * @param timeout_ms Таймаут ожидания завершения агента (по умолчанию 500 мс)
 * @return Структура результата агента
 * @throws std::runtime_error если агент завершился с ошибкой
 */
ScStructure InvokeVisualAdaptation(
    ScAgentContext & context,
    std::string const & componentIdentifier,
    double multiplier,
    sc_uint32 timeout_ms = 500);

// 🔮 Заготовки для будущего расширения:
// ScStructure InvokeAdaptationAgent(ScAgentContext&, ScAddr const& component, ...);
// bool ValidateUIModel(ScAgentContext&, ScAddr const& model);
// ScStructure InvokeLayoutAgent(ScAgentContext&, ScAddr const& component, ...);

}  // namespace htmlTranslationModule::UILib
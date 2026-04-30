/*
This source file is part of an OSTIS project.
Distributed under the MIT License.
*/
#include "UILib.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-memory/sc_debug.hpp>
#include "html-translator/HTMLTranslator.hpp"

namespace htmlTranslationModule::UILib
{

ScAddr FindUIModelByIdentifier(
    ScAgentContext & context,
    std::string const & identifier)
{
    return context.ResolveElementSystemIdentifier(identifier);
}

ScStructure InvokeTranslationAgent(
    ScAgentContext & context,
    ScAddr const & uiComponent,
    sc_uint32 timeout_ms)
{
    if (!uiComponent.IsValid())
    {
        SC_LOG_ERROR("UILib: uiComponent address is invalid");
        throw std::runtime_error("Invalid UI component address");
    }

    ScAddr const actionClass = HTMLTranslatorKeynodes::action_translate_sc_to_html;
    if (!context.IsElement(actionClass))
    {
        SC_LOG_ERROR("UILib: action_translate_sc_to_html not found in KB");
        throw std::runtime_error("Translation agent class not found");
    }

    ScAction action = context.GenerateAction(actionClass);
    
    ScAddr const rrel_1 = context.ResolveElementSystemIdentifier("rrel_1");
    action.SetArgument(rrel_1, uiComponent);

    try
    {
        action.InitiateAndWait(timeout_ms);
    }
    catch (std::exception const & e)
    {
        SC_LOG_ERROR("UILib: Failed to initiate translation agent: " + std::string(e.what()));
        throw;
    }

    if (!action.IsFinishedSuccessfully())
    {
        SC_LOG_ERROR("UILib: Translation agent finished unsuccessfully");
        throw std::runtime_error("Translation agent failed");
    }

    return action.GetResult();
}

ScAddr ExtractHTMLLinkFromResult(
    ScAgentContext & context,
    ScStructure const & resultStruct)
{
    if (!context.IsElement(resultStruct))
    {
        return ScAddr::Empty;
    }

    ScAddr htmlLink;
    
    //Используем такой же паттерн, как в HTMLTranslator::GetNestedComponentsHTMLRepresentation
    ScIterator3Ptr it = context.CreateIterator3(
        resultStruct,               // источник: структура
        ScType::ConstPermPosArc,    // тип дуги
        ScType::Unknown);           // цель: элемент
    
    while (it->Next() && !htmlLink.IsValid())
    {
        ScAddr candidate = it->Get(2);  //get(2) — цель дуги (элемент)
        if (candidate.IsValid() && context.GetElementType(candidate).IsLink())
        {
            htmlLink = candidate;
            break;
        }
    }
    
    return htmlLink;
}

std::optional<std::string> GetHTMLForModel(
    ScAgentContext & context,
    std::string const & modelIdentifier,
    sc_uint32 timeout_ms)
{
    ScAddr uiComponent = FindUIModelByIdentifier(context, modelIdentifier);
    if (!uiComponent.IsValid())
    {
        SC_LOG_ERROR("UILib: Model not found: " + modelIdentifier);
        return std::nullopt;
    }

    // auto выведет тип как ScStructure (возвращаемый тип InvokeTranslationAgent)
    try
    {
        auto resultStruct = InvokeTranslationAgent(context, uiComponent, timeout_ms);
        
        ScAddr htmlLink = ExtractHTMLLinkFromResult(context, resultStruct);
        if (!htmlLink.IsValid())
        {
            SC_LOG_ERROR("UILib: No HTML link found in result structure");
            return std::nullopt;
        }

        std::string htmlContent;
        context.GetLinkContent(htmlLink, htmlContent);
        return htmlContent;
    }
    catch (std::exception const & e)
    {
        SC_LOG_ERROR("UILib: Translation failed: " + std::string(e.what()));
        return std::nullopt;
    }
}

ScStructure InvokeVisualAdaptation(
    ScAgentContext & context,
    std::string const & componentIdentifier,
    double multiplier,
    sc_uint32 timeout_ms)
{
    // 1. Найти компонент по идентификатору (как в GetHTMLForModel)
    ScAddr component = FindUIModelByIdentifier(context, componentIdentifier);
    if (!component.IsValid())
    {
        SC_LOG_ERROR("UILib: Component not found: " + componentIdentifier);
        throw std::runtime_error("Component '" + componentIdentifier + "' not found");
    }

    // 2. Создать ссылку с множителем (ScLink со строковым значением)
    ScAddr multiplierLink = context.GenerateLink();
    context.SetLinkContent(multiplierLink, std::to_string(multiplier));

    // 3. Запустить агент визуальной адаптации
    ScAddr const actionClass = HTMLTranslatorKeynodes::action_apply_visual_adaptation;
    if (!context.IsElement(actionClass))
    {
        SC_LOG_ERROR("UILib: action_apply_visual_adaptation not found in KB");
        throw std::runtime_error("Visual adaptation agent class not found");
    }

    ScAction action = context.GenerateAction(actionClass);
    
    // 4. Передать аргументы: компонент + множитель (через rrel_1 и rrel_2)
    ScAddr const rrel_1 = context.ResolveElementSystemIdentifier("rrel_1");
    ScAddr const rrel_2 = context.ResolveElementSystemIdentifier("rrel_2");
    
    action.SetArgument(rrel_1, component);       // Аргумент 1: компонент
    action.SetArgument(rrel_2, multiplierLink);  // Аргумент 2: множитель

    // 5. Инициировать и ждать завершения (как в InvokeTranslationAgent)
    try
    {
        action.InitiateAndWait(timeout_ms);
    }
    catch (std::exception const & e)
    {
        SC_LOG_ERROR("UILib: Failed to initiate visual adaptation: " + std::string(e.what()));
        throw;
    }

    // 6. Проверить результат (как в InvokeTranslationAgent)
    if (!action.IsFinishedSuccessfully())
    {
        SC_LOG_ERROR("UILib: Visual adaptation agent finished unsuccessfully");
        throw std::runtime_error("Visual adaptation failed");
    }

    try
    {
        HTMLTranslator::RegenerateHTMLRepresentationWithParents(context, component);
        SC_LOG_DEBUG("UILib: Cache regenerated for component and parents: " + componentIdentifier);
    }
    catch (std::exception const & e)
    {
        SC_LOG_WARNING("UILib: Failed to regenerate cache: " + std::string(e.what()));
        // Не выбрасываем исключение — адаптация уже прошла успешно
    }
    
    return action.GetResult();
}

}  // namespace htmlTranslationModule::UILib
#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_event.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-memory/sc_structure.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "AdaptationManagerAgent.hpp"
#include "parameter-retriever/ParameterRetriever.hpp"

using namespace utils;

namespace htmlTranslationModule
{

ScResult AdaptationManagerAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
    ScAddr userProfile = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, action, ScKeynodes::rrel_1);
    ScAddr rootComponent = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, action, ScKeynodes::rrel_2);

    SC_LOG_INFO("AdaptationManager: userProfile hash = " + std::to_string(userProfile.Hash()));
    SC_LOG_INFO("AdaptationManager: rootComponent hash = " + std::to_string(rootComponent.Hash()));

    if (!userProfile.IsValid() || !rootComponent.IsValid())
    {
        SC_LOG_ERROR("AdaptationManager: userProfile or rootComponent is invalid.");
        return action.FinishUnsuccessfully();
    }

    SC_LOG_INFO("AdaptationManager: Starting adaptation for user profile.");

    // 2. Получаем все адаптации пользователя
    ScAddrVector adaptations = GetUserAdaptations(m_context, userProfile);

    if (adaptations.empty())
    {
        SC_LOG_INFO("AdaptationManager: No adaptations found for user.");
        // Возвращаем оригинальный компонент без изменений
        ScStructure result = m_context.GenerateStructure();
        result << rootComponent;
        action.SetResult(result);
        return action.FinishSuccessfully();
    }

    SC_LOG_INFO("AdaptationManager: Found " + std::to_string(adaptations.size()) + " adaptations.");

    // 3. Рекурсивно адаптируем дерево компонентов
    AdaptComponentTree(m_context, userProfile, rootComponent, adaptations);

    // 4. Возвращаем адаптированный корневой компонент
    ScStructure result = m_context.GenerateStructure();
    result << rootComponent;
    action.SetResult(result);

    SC_LOG_INFO("AdaptationManager: Adaptation completed successfully.");
    return action.FinishSuccessfully();
}

ScAddr AdaptationManagerAgent::GetActionClass() const
{
    return HTMLTranslatorKeynodes::action_adapt_ui_for_user;
}

ScAddrVector AdaptationManagerAgent::GetUserAdaptations(
    ScAgentContext & context,
    ScAddr const & userProfile)
{
    ScAddrVector adaptations;

    // Шаблон: userProfile ==nrel_has_adaptation==> adaptation
    ScTemplate adaptationsTemplate;
    adaptationsTemplate.Quintuple(
        userProfile,
        ScType::VarCommonArc,
        ScType::VarNode >> "adaptation",
        ScType::VarPermPosArc,
        HTMLTranslatorKeynodes::nrel_has_adaptation);

    context.SearchByTemplate(adaptationsTemplate,
        [&adaptations](ScTemplateSearchResultItem const & item) {
            ScAddr adaptation;
            item.Get("adaptation", adaptation);
            adaptations.push_back(adaptation);
        });

    return adaptations;
}

ScAddr AdaptationManagerAgent::GetAdaptationAgent(
    ScAgentContext & context,
    ScAddr const & adaptation)
{
    ScAddr agent;

    // Шаблон: adaptation ==nrel_adaptation_agent==> agent
    ScTemplate agentTemplate;
    agentTemplate.Quintuple(
        adaptation,
        ScType::VarCommonArc,
        ScType::VarNode >> "agent",
        ScType::VarPermPosArc,
        HTMLTranslatorKeynodes::nrel_adaptation_agent);

    context.SearchByTemplateInterruptibly(agentTemplate,
        [&agent](ScTemplateSearchResultItem const & item) {
            item.Get("agent", agent);
            return ScTemplateSearchRequest::STOP;
        });

    return agent;
}

std::string AdaptationManagerAgent::GetAdaptationParameter(
    ScAgentContext & context,
    ScAddr const & adaptation,
    ScAddr const & parameterRelation)
{
    std::string value;

    // Шаблон: adaptation ==parameterRelation==> link
    ScTemplate parameterTemplate;
    parameterTemplate.Quintuple(
        adaptation,
        ScType::VarCommonArc,
        ScType::VarNodeLink >> "value_link",
        ScType::VarPermPosArc,
        parameterRelation);

    context.SearchByTemplateInterruptibly(parameterTemplate,
        [&context, &value](ScTemplateSearchResultItem const & item) {
            ScAddr valueLink;
            item.Get("value_link", valueLink);
            context.GetLinkContent(valueLink, value);
            return ScTemplateSearchRequest::STOP;
        });

    return value;
}

void AdaptationManagerAgent::AdaptComponentTree(
    ScAgentContext & context,
    ScAddr const & userProfile,
    ScAddr const & component,
    ScAddrVector const & adaptations)
{
    SC_LOG_INFO("AdaptationManager: Adapting component.");

    // 1. Применяем все адаптации к текущему компоненту
    for (ScAddr const & adaptation : adaptations)
    {
        ScAddr adaptationAgent = GetAdaptationAgent(context, adaptation);

        if (!context.IsElement(adaptationAgent))
        {
            SC_LOG_WARNING("AdaptationManager: Adaptation agent not found.");
            continue;
        }

        // 2. Получаем параметр multiplier (для визуальной адаптации)
        std::string multiplier = GetAdaptationParameter(
            context,
            adaptation,
            HTMLTranslatorKeynodes::nrel_adaptation_multiplier);

        // 3. Применяем адаптацию к компоненту
        ApplyAdaptationToComponent(context, component, adaptationAgent, multiplier);
    }

    // 4. Рекурсивно обрабатываем вложенные компоненты
    StringScAddrMap nestedComponents = ParameterRetriever::GetNestedUIComponents(context, component);

    for (auto const & [id, nestedComponent] : nestedComponents)
    {
        // Рекурсивный вызов для вложенного компонента
        AdaptComponentTree(context, userProfile, nestedComponent, adaptations);
    }
}

void AdaptationManagerAgent::ApplyAdaptationToComponent(
    ScAgentContext & context,
    ScAddr const & component,
    ScAddr const & adaptationAgent,
    std::string const & multiplier)
{
    SC_LOG_INFO("AdaptationManager: Applying adaptation agent.");

    // Запускаем агент адаптации для этого компонента
    ScAction adaptationAction = context.GenerateAction(adaptationAgent);

    // Если multiplier есть, передаём его как второй аргумент
    if (!multiplier.empty())
    {
        ScAddr multiplierLink = context.GenerateLink();
        context.SetLinkContent(multiplierLink, multiplier);
        adaptationAction.SetArguments(component, multiplierLink);
    }
    else
    {
        adaptationAction.SetArguments(component);
    }

    // Запускаем и ждём завершения
    adaptationAction.InitiateAndWait();

    if (adaptationAction.IsFinishedUnsuccessfully ())
    {
        SC_LOG_WARNING("AdaptationManager: Adaptation agent finished unsuccessfully.");
    }
}

}  // namespace htmlTranslationModule
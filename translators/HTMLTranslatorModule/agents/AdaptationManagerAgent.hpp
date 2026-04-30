#pragma once

#include <sc-memory/sc_addr.hpp>
#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>
#include <map>
#include <string>

namespace htmlTranslationModule
{
using StringScAddrMap = std::map<std::string, ScAddr>;

class AdaptationManagerAgent : public ScActionInitiatedAgent
{
public:
    ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
    ScAddr GetActionClass() const override;

private:
    // Получить все адаптации пользователя
    ScAddrVector GetUserAdaptations(ScAgentContext & context, ScAddr const & userProfile);

    // Получить агент адаптации из БЗ
    ScAddr GetAdaptationAgent(ScAgentContext & context, ScAddr const & adaptation);

    // Получить параметр адаптации (например, multiplier)
    std::string GetAdaptationParameter(
        ScAgentContext & context,
        ScAddr const & adaptation,
        ScAddr const & parameterRelation);

    // Рекурсивно адаптировать дерево компонентов
    void AdaptComponentTree(
        ScAgentContext & context,
        ScAddr const & userProfile,
        ScAddr const & component,
        ScAddrVector const & adaptations);

    // Применить одну адаптацию к компоненту
    void ApplyAdaptationToComponent(
        ScAgentContext & context,
        ScAddr const & component,
        ScAddr const & adaptationAgent,
        std::string const & multiplier);
};
}  // namespace htmlTranslationModule

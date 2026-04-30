#pragma once

#include <sc-memory/sc_agent.hpp>

namespace htmlTranslationModule
{
class VisualAdaptationAgent : public ScActionInitiatedAgent
{
public:
    ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;
    ScAddr GetActionClass() const override;
};
}  // namespace htmlTranslationModule

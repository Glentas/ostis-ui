/*
 * Generation teplate by button
 */

#pragma once

#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>

namespace specifiedStringTemplateModule
{
class GenerateTemplateAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

  static ScTemplateParams GetScTemplateParamFromTemplateReplacement(ScAgentContext & context, ScAddr const & variablesSetAddr);
};
}  // namespace specifiedStringTemplateModulebui

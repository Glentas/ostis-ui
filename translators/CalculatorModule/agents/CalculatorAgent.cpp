/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_event.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-memory/sc_structure.hpp>

#include "keynodes/CalculatorKeynodes.hpp"

#include "CalculatorAgent.hpp"

using namespace utils;

namespace calculatorModule
{
ScResult CalculatorAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto const [interfaceNode] = action.GetArguments<1>();

  if (!interfaceNode.IsValid())
  {
    SC_LOG_ERROR("Given UI element is invalid.");
    return action.FinishUnsuccessfully();
  }
  return action.FinishSuccessfully();
}

ScAddr CalculatorAgent::GetActionClass() const
{
  return CalculatorKeynodes::action_calculate_or_append;
}

}  // namespace calculatorModule

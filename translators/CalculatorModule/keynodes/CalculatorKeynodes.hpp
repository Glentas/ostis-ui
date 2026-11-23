/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>
#include <sc-memory/sc_type.hpp>

namespace calculatorModule
{
class CalculatorKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_calculate_or_append{"action_calculate_or_append", ScType::ConstNodeClass};
  static inline ScKeynode const concept_accumulator_link{"concept_accumulator_link", ScType::ConstNodeLink};
};

}  // namespace calculatorModule

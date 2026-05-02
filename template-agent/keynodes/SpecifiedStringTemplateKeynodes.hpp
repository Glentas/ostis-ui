/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>

namespace specifiedStringTemplateModule
{
class SpecifiedStringTemplateKeynodes : public ScKeynodes
{
  public:
  static inline ScKeynode const action_evaluate_specified_string_template{"action_evaluate_specified_string_template"};

  static inline ScKeynode const action_generate_template{"action_generate_template"};

  static inline ScKeynode const action_translate_XML_to_json{"action_translate_XML_to_json"};

  static inline ScKeynode const action_translate_json_to_XML{"action_translate_json_to_XML"};

  static inline ScKeynode const nrel_html_parameter_id{"nrel_html_parameter_id", ScType::ConstNodeNonRole};
  
  static inline ScKeynode const nrel_variable_template{"nrel_variable_template", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_html_template{"nrel_html_template", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_action_template{"nrel_action_template", ScType::ConstNodeNonRole};

  static inline ScKeynode const rrel_search_template{"rrel_search_template", ScType::ConstNodeRole};

  static inline ScKeynode const rrel_input_params{"rrel_input_params", ScType::ConstNodeRole};

  static inline ScKeynode const rrel_component_with_input{"rrel_component_with_input", ScType::ConstNodeRole};

  static inline ScKeynode const rrel_outputs{"rrel_outputs", ScType::ConstNodeRole};

  static inline ScKeynode const nrel_value{"nrel_value", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_format{"nrel_format", ScType::ConstNodeNonRole};

  static inline ScKeynode const rrel_variable{"rrel_variable", ScType::ConstNodeRole};

  static inline ScKeynode const rrel_value{"rrel_value", ScType::ConstNodeRole};

  static inline ScKeynode const rrel_template{"rrel_template", ScType::ConstNodeRole};

  static inline ScKeynode const nrel_replacements_variables{"nrel_replacements_variables", ScType::ConstNodeNonRole};

  static inline ScKeynode const nrel_translation_action{"nrel_translation_action", ScType::ConstNodeNonRole};
};

}  // namespace specifiedStringTemplateModule

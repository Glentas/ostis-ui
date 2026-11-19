/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_keynodes.hpp>
#include <sc-memory/sc_type.hpp>

namespace htmlTranslationModule
{
class HTMLTranslatorKeynodes : public ScKeynodes
{
public:
  static inline ScKeynode const action_translate_sc_to_html{"action_translate_sc_to_html", ScType::ConstNodeClass};
  static inline ScKeynode const concept_current_ostis_ui_model{
      "concept_current_ostis_ui_model",
      ScType::ConstNodeClass};

  // отношения
  static inline ScKeynode const nrel_inclusion{"nrel_inclusion", ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_html_representation{"nrel_html_representation", ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_ui_contains{"nrel_ui_contains", ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_ui_has_component{"nrel_ui_has_component", ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_ui_id{"nrel_ui_id", ScType::ConstNodeNonRole};
  static inline ScKeynode const nrel_ui_value{"nrel_ui_value", ScType::ConstNodeNonRole};

  // типы компонентов
  static inline ScKeynode const concept_ui_interface{"concept_ui_interface", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_object{"concept_ui_object", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_image{"concept_ui_image", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_sound{"concept_ui_sound", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_text{"concept_ui_text", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_divider{"concept_ui_divider", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_spacer{"concept_ui_spacer", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_menu{"concept_ui_menu", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_menu_bar{"concept_ui_menu_bar", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_list{"concept_ui_list", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_scroll{"concept_ui_scroll", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_window{"concept_ui_window", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_text_field{"concept_ui_text_field", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_text_area{"concept_ui_text_area", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_slider{"concept_ui_slider", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_select{"concept_ui_select", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_button{"concept_ui_button", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_menu_item{"concept_ui_menu_item", ScType::ConstNodeClass};

  // свойства
  static inline ScKeynode const concept_ui_name{"concept_ui_name", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_x{"concept_ui_x", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_y{"concept_ui_y", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_width{"concept_ui_width", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_height{"concept_ui_height", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_order{"concept_ui_order", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_visible{"concept_ui_visible", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_enabled{"concept_ui_enabled", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_opacity{"concept_ui_opacity", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_text_value{"concept_ui_text_value", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_tooltip{"concept_ui_tooltip", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_action_id{"concept_ui_action_id", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_background_color{"concept_ui_background_color", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_color{"concept_ui_color", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_border_color{"concept_ui_border_color", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_border_width{"concept_ui_border_width", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_border_style{"concept_ui_border_style", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_corner_radius{"concept_ui_corner_radius", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_font_size{"concept_ui_font_size", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_font_weight{"concept_ui_font_weight", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_text_align{"concept_ui_text_align", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_padding{"concept_ui_padding", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_margin{"concept_ui_margin", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_min_width{"concept_ui_min_width", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_min_height{"concept_ui_min_height", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_max_width{"concept_ui_max_width", ScType::ConstNodeClass};
  static inline ScKeynode const concept_ui_max_height{"concept_ui_max_height", ScType::ConstNodeClass};
};

}  // namespace htmlTranslationModule

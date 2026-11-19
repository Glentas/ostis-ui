/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <sc-memory/sc_keynodes.hpp>

#include "AbstractUIModel.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"

namespace htmlTranslationModule
{
class HTMLTagMapper
{
public:
  static std::unordered_map<ScKeynode::HashType, UIComponentType> const & GetUIComponentTypeByKeynodeHash()
  {
    static auto const UIComponentTypeByKeynodeHash = std::unordered_map<ScKeynode::HashType, UIComponentType>(
        {{HTMLTranslatorKeynodes::concept_ui_image.Hash(), UIComponentType::Image},
         {HTMLTranslatorKeynodes::concept_ui_sound.Hash(), UIComponentType::Sound},
         {HTMLTranslatorKeynodes::concept_ui_text.Hash(), UIComponentType::Text},
         {HTMLTranslatorKeynodes::concept_ui_divider.Hash(), UIComponentType::Divider},
         {HTMLTranslatorKeynodes::concept_ui_spacer.Hash(), UIComponentType::Spacer},
         {HTMLTranslatorKeynodes::concept_ui_menu.Hash(), UIComponentType::Menu},
         {HTMLTranslatorKeynodes::concept_ui_menu_bar.Hash(), UIComponentType::MenuBar},
         {HTMLTranslatorKeynodes::concept_ui_list.Hash(), UIComponentType::List},
         {HTMLTranslatorKeynodes::concept_ui_scroll.Hash(), UIComponentType::Scroll},
         {HTMLTranslatorKeynodes::concept_ui_window.Hash(), UIComponentType::Window},
         {HTMLTranslatorKeynodes::concept_ui_text_field.Hash(), UIComponentType::TextField},
         {HTMLTranslatorKeynodes::concept_ui_text_area.Hash(), UIComponentType::TextArea},
         {HTMLTranslatorKeynodes::concept_ui_slider.Hash(), UIComponentType::Slider},
         {HTMLTranslatorKeynodes::concept_ui_select.Hash(), UIComponentType::Select},
         {HTMLTranslatorKeynodes::concept_ui_button.Hash(), UIComponentType::Button},
         {HTMLTranslatorKeynodes::concept_ui_menu_item.Hash(), UIComponentType::MenuItem}});
    return UIComponentTypeByKeynodeHash;
  }

  static std::unordered_map<ScKeynode::HashType, UIParamType> const & GetUIParamTypeByKeynodeHash()
  {
    static auto const UIParamTypeByKeynodeHash = std::unordered_map<ScKeynode::HashType, UIParamType>(
        {{HTMLTranslatorKeynodes::concept_ui_name.Hash(), UIParamType::Name},
         {HTMLTranslatorKeynodes::concept_ui_x.Hash(), UIParamType::X},
         {HTMLTranslatorKeynodes::concept_ui_y.Hash(), UIParamType::Y},
         {HTMLTranslatorKeynodes::concept_ui_width.Hash(), UIParamType::Width},
         {HTMLTranslatorKeynodes::concept_ui_height.Hash(), UIParamType::Height},
         {HTMLTranslatorKeynodes::concept_ui_order.Hash(), UIParamType::Order},
         {HTMLTranslatorKeynodes::concept_ui_visible.Hash(), UIParamType::Visible},
         {HTMLTranslatorKeynodes::concept_ui_enabled.Hash(), UIParamType::Enabled},
         {HTMLTranslatorKeynodes::concept_ui_opacity.Hash(), UIParamType::Opacity},
         {HTMLTranslatorKeynodes::concept_ui_text_value.Hash(), UIParamType::TextValue},
         {HTMLTranslatorKeynodes::concept_ui_tooltip.Hash(), UIParamType::Tooltip},
         {HTMLTranslatorKeynodes::concept_ui_action_id.Hash(), UIParamType::ActionId},
         {HTMLTranslatorKeynodes::concept_ui_background_color.Hash(), UIParamType::BackgroundColor},
         {HTMLTranslatorKeynodes::concept_ui_color.Hash(), UIParamType::Color},
         {HTMLTranslatorKeynodes::concept_ui_border_color.Hash(), UIParamType::BorderColor},
         {HTMLTranslatorKeynodes::concept_ui_border_width.Hash(), UIParamType::BorderWidth},
         {HTMLTranslatorKeynodes::concept_ui_border_style.Hash(), UIParamType::BorderStyle},
         {HTMLTranslatorKeynodes::concept_ui_corner_radius.Hash(), UIParamType::CornerRadius},
         {HTMLTranslatorKeynodes::concept_ui_font_size.Hash(), UIParamType::FontSize},
         {HTMLTranslatorKeynodes::concept_ui_font_weight.Hash(), UIParamType::FontWeight},
         {HTMLTranslatorKeynodes::concept_ui_text_align.Hash(), UIParamType::TextAlign},
         {HTMLTranslatorKeynodes::concept_ui_padding.Hash(), UIParamType::Padding},
         {HTMLTranslatorKeynodes::concept_ui_margin.Hash(), UIParamType::Margin},
         {HTMLTranslatorKeynodes::concept_ui_min_width.Hash(), UIParamType::MinWidth},
         {HTMLTranslatorKeynodes::concept_ui_min_height.Hash(), UIParamType::MinHeight},
         {HTMLTranslatorKeynodes::concept_ui_max_width.Hash(), UIParamType::MaxWidth},
         {HTMLTranslatorKeynodes::concept_ui_max_height.Hash(), UIParamType::MaxHeight}});
    return UIParamTypeByKeynodeHash;
  };

  static std::unordered_map<ScKeynode::HashType, UILinkType> const & GetUILinkTypeByKeynodeClass()
  {
    static auto const UILinkTypeByKeynode = std::unordered_map<ScKeynode::HashType, UILinkType>(
        {{HTMLTranslatorKeynodes::nrel_ui_contains.Hash(), UILinkType::Contains}});
    return UILinkTypeByKeynode;
  };

  static inline std::unordered_map<UIComponentType, std::string_view> const HTMLTemplateByUIComponentType = {
      {UIComponentType::Image, "<img style=\"\n{}\" src=\"{}\" alt=\"\">"},
      {UIComponentType::Sound, "<audio style=\"\n{}\" src=\"{}\" controls></audio>"},
      {UIComponentType::Text, "<p style=\"\n{}\">{}</p>"},
      {UIComponentType::Divider, "<hr style=\"\n{}\">"},
      {UIComponentType::Spacer, "<div style=\"\n{}\"></div>"},
      {UIComponentType::Menu, "<nav style=\"\n{}\">{}</nav>"},
      {UIComponentType::MenuBar, "<div role=\"menubar\" style=\"\n{}\">{}</div>"},
      {UIComponentType::List, "<ul style=\"\n{}\">{}</ul>"},
      {UIComponentType::Scroll, "<div style=\"\noverflow:auto;{}\">{}</div>"},
      {UIComponentType::Window, "<section role=\"dialog\" style=\"\n{}\">{}</section>"},
      {UIComponentType::TextField, "<input type=\"text\" style=\"\n{}\" value=\"{}\">"},
      {UIComponentType::TextArea, "<textarea style=\"\n{}\">{}</textarea>"},
      {UIComponentType::Slider, "<input type=\"range\" style=\"\n{}\" value=\"{}\">"},
      {UIComponentType::Select, "<select style=\"\n{}\">{}</select>"},
      {UIComponentType::Button, "<button type=\"button\" style=\"\n{}\">{}</button>"},
      {UIComponentType::MenuItem, "<li role=\"menuitem\" style=\"\n{}\">{}</li>"}};

  static inline std::unordered_map<UIParamType, std::string_view> const HTMLTemplateByUIParamType = {
      {UIParamType::X, R"(left: {};)"},
      {UIParamType::Y, R"(top: {};)"},
      {UIParamType::Width, R"(width: {};)"},
      {UIParamType::Height, R"(height: {};)"},
      {UIParamType::Order, R"(z-index: {};)"},
      {UIParamType::Visible, R"(visibility: {};)"},
      {UIParamType::Opacity, R"(opacity: {};)"},
      {UIParamType::BackgroundColor, R"(background-color: {};)"},
      {UIParamType::Color, R"(color: {};)"},
      {UIParamType::BorderColor, R"(border-color: {};)"},
      {UIParamType::BorderWidth, R"(border-width: {};)"},
      {UIParamType::BorderStyle, R"(border-style: {};)"},
      {UIParamType::CornerRadius, R"(border-radius: {};)"},
      {UIParamType::FontSize, R"(font-size: {};)"},
      {UIParamType::FontWeight, R"(font-weight: {};)"},
      {UIParamType::TextAlign, R"(text-align: {};)"},
      {UIParamType::Padding, R"(padding: {};)"},
      {UIParamType::Margin, R"(margin: {};)"},
      {UIParamType::MinWidth, R"(min-width: {};)"},
      {UIParamType::MinHeight, R"(min-height: {};)"},
      {UIParamType::MaxWidth, R"(max-width: {};)"},
      {UIParamType::MaxHeight, R"(max-height: {};)"}};

  static inline std::string GetFullHTML(std::string const & body)
  {
    static auto script = R"(
<script>
  addEventListener('load', () => {
    let minX = Infinity, minY = Infinity, maxX = -Infinity, maxY = -Infinity;

    for (const el of document.querySelectorAll('body *')) {
      const r = el.getBoundingClientRect();
      if (!r.width && !r.height) continue;

      if (r.left   < minX) minX = r.left;
      if (r.top    < minY) minY = r.top;
      if (r.right  > maxX) maxX = r.right;
      if (r.bottom > maxY) maxY = r.bottom;
    }

    if (!isFinite(minX)) return;

    const w = maxX - minX;
    const h = maxY - minY;

    document.body.style.transform = `translate(
      ${(innerWidth  - w) / 2 - minX}px,
      ${(innerHeight - h) / 2 - minY}px
    )`;
  });
</script>
)";

    return std::string(R"(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8"/>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover"/>
  <title>Test interface</title>
  <style>
    html, body { margin: 0; height: 100%; }
    body { position: relative; }
    body * { position: fixed; resize: none; }
  </style>
)") + script
           + R"(
</head>
<body>
  <div>
)" + body + R"(
  </div>
</body>
</html>
)";
  }
};
}  // namespace htmlTranslationModule
/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

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
      {UIComponentType::Image, R"(<img style="{}" src="{}" alt="">)"},
      {UIComponentType::Sound, R"(<audio style="{}" src="{}" controls></audio>)"},
      {UIComponentType::Text, R"(<p style="{}">{}</p>)"},
      {UIComponentType::Divider, R"(<hr style="{}">)"},
      {UIComponentType::Spacer, R"(<div style="{}"></div>)"},
      {UIComponentType::Menu, R"(<nav style="{}">{}</nav>)"},
      {UIComponentType::MenuBar, R"(<div role="menubar" style="{}">{}</div>)"},
      {UIComponentType::List, R"(<ul style="{}">{}</ul>)"},
      {UIComponentType::Scroll, R"(<div style="{};overflow:auto">{}</div>)"},
      {UIComponentType::Window, R"(<section role="dialog" style="{}">{}</section>)"},
      {UIComponentType::TextField, R"(<input type="text" style="{}" value="{}">)"},
      {UIComponentType::TextArea, R"(<textarea style="{}">{}</textarea>)"},
      {UIComponentType::Slider, R"(<input type="range" style="{}" value="{}">)"},
      {UIComponentType::Select, R"(<select style="{}">{}</select>)"},
      {UIComponentType::Button, R"(<button type="button" style="{}">{}</button>)"},
      {UIComponentType::MenuItem, R"(<li role="menuitem" style="{}">{}</li>)"}};

  static inline std::unordered_map<UIParamType, std::string_view> const HTMLTemplateByUIParamType = {
      {UIParamType::X, R"(left: {};)"},
      {UIParamType::Y, R"(top: {};)"},
      {UIParamType::Width, R"(width: {};)"},
      {UIParamType::Height, R"(height: {};)"},
      {UIParamType::Order, R"(order: {};)"},
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
    return
        R"(
  <!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover" />
    <title>Format converter</title>
    <style>
      body {
        margin: 0;
        min-height: auto;
        display: flex;
        justify-content: center;
        align-items: center;

        order: 1;
        visibility: visible;
        opacity: 1;

        background-color: #f4f4f4;
        color: #1a1a1a;

        border-color: #6f6f6f;
        border-width: 1px;
        border-style: solid;
        border-radius: 0;

        font-size: 13px;
        font-weight: normal;
        text-align: left;

        padding: 0;
        margin: 0;

        min-width: 0;
        min-height: 0;

        max-width: none;
        max-height: none;
      }
    </style>
  </head>
  <body>)"
        + body +
        R"(
  </body>
  </html>
  )";
  }
};
}  // namespace htmlTranslationModule
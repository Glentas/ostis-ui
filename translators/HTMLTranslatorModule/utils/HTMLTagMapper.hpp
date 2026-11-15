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
  static std::unordered_map<ScKeynode::HashType, ComponentType> const & GetComponentTypeByKeynodeHash()
  {
    static auto const componentTypeByKeynodeHash = std::unordered_map<ScKeynode::HashType, ComponentType>(
        {{HTMLTranslatorKeynodes::concept_ui_image.Hash(), ComponentType::Image},
         {HTMLTranslatorKeynodes::concept_ui_sound.Hash(), ComponentType::Sound},
         {HTMLTranslatorKeynodes::concept_ui_text.Hash(), ComponentType::Text},
         {HTMLTranslatorKeynodes::concept_ui_divider.Hash(), ComponentType::Divider},
         {HTMLTranslatorKeynodes::concept_ui_spacer.Hash(), ComponentType::Spacer},
         {HTMLTranslatorKeynodes::concept_ui_menu.Hash(), ComponentType::Menu},
         {HTMLTranslatorKeynodes::concept_ui_menu_bar.Hash(), ComponentType::MenuBar},
         {HTMLTranslatorKeynodes::concept_ui_list.Hash(), ComponentType::List},
         {HTMLTranslatorKeynodes::concept_ui_scroll.Hash(), ComponentType::Scroll},
         {HTMLTranslatorKeynodes::concept_ui_window.Hash(), ComponentType::Window},
         {HTMLTranslatorKeynodes::concept_ui_text_field.Hash(), ComponentType::TextField},
         {HTMLTranslatorKeynodes::concept_ui_text_area.Hash(), ComponentType::TextArea},
         {HTMLTranslatorKeynodes::concept_ui_slider.Hash(), ComponentType::Slider},
         {HTMLTranslatorKeynodes::concept_ui_select.Hash(), ComponentType::Select},
         {HTMLTranslatorKeynodes::concept_ui_button.Hash(), ComponentType::Button},
         {HTMLTranslatorKeynodes::concept_ui_menu_item.Hash(), ComponentType::MenuItem}});
    return componentTypeByKeynodeHash;
  }

  static std::unordered_map<ScKeynode::HashType, ParamType> const & GetParamTypeByKeynodeHash()
  {
    static auto const paramTypeByKeynodeHash = std::unordered_map<ScKeynode::HashType, ParamType>(
        {{HTMLTranslatorKeynodes::concept_ui_name.Hash(), ParamType::Name},
         {HTMLTranslatorKeynodes::concept_ui_x.Hash(), ParamType::X},
         {HTMLTranslatorKeynodes::concept_ui_y.Hash(), ParamType::Y},
         {HTMLTranslatorKeynodes::concept_ui_width.Hash(), ParamType::Width},
         {HTMLTranslatorKeynodes::concept_ui_height.Hash(), ParamType::Height},
         {HTMLTranslatorKeynodes::concept_ui_order.Hash(), ParamType::Order},
         {HTMLTranslatorKeynodes::concept_ui_visible.Hash(), ParamType::Visible},
         {HTMLTranslatorKeynodes::concept_ui_enabled.Hash(), ParamType::Enabled},
         {HTMLTranslatorKeynodes::concept_ui_opacity.Hash(), ParamType::Opacity},
         {HTMLTranslatorKeynodes::concept_ui_text_value.Hash(), ParamType::TextValue},
         {HTMLTranslatorKeynodes::concept_ui_tooltip.Hash(), ParamType::Tooltip},
         {HTMLTranslatorKeynodes::concept_ui_action_id.Hash(), ParamType::ActionId},
         {HTMLTranslatorKeynodes::concept_ui_background_color.Hash(), ParamType::BackgroundColor},
         {HTMLTranslatorKeynodes::concept_ui_color.Hash(), ParamType::Color},
         {HTMLTranslatorKeynodes::concept_ui_border_color.Hash(), ParamType::BorderColor},
         {HTMLTranslatorKeynodes::concept_ui_border_width.Hash(), ParamType::BorderWidth},
         {HTMLTranslatorKeynodes::concept_ui_border_style.Hash(), ParamType::BorderStyle},
         {HTMLTranslatorKeynodes::concept_ui_corner_radius.Hash(), ParamType::CornerRadius},
         {HTMLTranslatorKeynodes::concept_ui_font_size.Hash(), ParamType::FontSize},
         {HTMLTranslatorKeynodes::concept_ui_font_weight.Hash(), ParamType::FontWeight},
         {HTMLTranslatorKeynodes::concept_ui_text_align.Hash(), ParamType::TextAlign},
         {HTMLTranslatorKeynodes::concept_ui_padding.Hash(), ParamType::Padding},
         {HTMLTranslatorKeynodes::concept_ui_margin.Hash(), ParamType::Margin},
         {HTMLTranslatorKeynodes::concept_ui_min_width.Hash(), ParamType::MinWidth},
         {HTMLTranslatorKeynodes::concept_ui_min_height.Hash(), ParamType::MinHeight},
         {HTMLTranslatorKeynodes::concept_ui_max_width.Hash(), ParamType::MaxWidth},
         {HTMLTranslatorKeynodes::concept_ui_max_height.Hash(), ParamType::MaxHeight}});
    return paramTypeByKeynodeHash;
  };

  static std::unordered_map<ScKeynode::HashType, LinkType> const & GetLinkTypeByKeynodeClass()
  {
    static auto const linkTypeByKeynode = std::unordered_map<ScKeynode::HashType, LinkType>(
        {{HTMLTranslatorKeynodes::nrel_ui_contains.Hash(), LinkType::Contains}});
    return linkTypeByKeynode;
  };

  static inline std::unordered_map<ComponentType, std::string_view> const HTMLTemplateByComponentType = {
      {ComponentType::Image, R"(<img style="{}" src="{}" alt="">)"},
      {ComponentType::Sound, R"(<audio style="{}" src="{}" controls></audio>)"},
      {ComponentType::Text, R"(<p style="{}">{}</p>)"},
      {ComponentType::Divider, R"(<hr style="{}">)"},
      {ComponentType::Spacer, R"(<div style="{}"></div>)"},
      {ComponentType::Menu, R"(<nav style="{}">{}</nav>)"},
      {ComponentType::MenuBar, R"(<div role="menubar" style="{}">{}</div>)"},
      {ComponentType::List, R"(<ul style="{}">{}</ul>)"},
      {ComponentType::Scroll, R"(<div style="{};overflow:auto">{}</div>)"},
      {ComponentType::Window, R"(<section role="dialog" style="{}">{}</section>)"},
      {ComponentType::TextField, R"(<input type="text" style="{}" value="{}">)"},
      {ComponentType::TextArea, R"(<textarea style="{}">{}</textarea>)"},
      {ComponentType::Slider, R"(<input type="range" style="{}" value="{}">)"},
      {ComponentType::Select, R"(<select style="{}">{}</select>)"},
      {ComponentType::Button, R"(<button type="button" style="{}">{}</button>)"},
      {ComponentType::MenuItem, R"(<li role="menuitem" style="{}">{}</li>)"}};

  static inline std::unordered_map<ParamType, std::string_view> const HTMLTemplateByParamType = {
      {ParamType::X, R"(left: {};)"},
      {ParamType::Y, R"(top: {};)"},
      {ParamType::Width, R"(width: {};)"},
      {ParamType::Height, R"(height: {};)"},
      {ParamType::Order, R"(order: {};)"},
      {ParamType::Visible, R"(visibility: {};)"},
      {ParamType::Opacity, R"(opacity: {};)"},
      {ParamType::BackgroundColor, R"(background-color: {};)"},
      {ParamType::Color, R"(color: {};)"},
      {ParamType::BorderColor, R"(border-color: {};)"},
      {ParamType::BorderWidth, R"(border-width: {};)"},
      {ParamType::BorderStyle, R"(border-style: {};)"},
      {ParamType::CornerRadius, R"(border-radius: {};)"},
      {ParamType::FontSize, R"(font-size: {};)"},
      {ParamType::FontWeight, R"(font-weight: {};)"},
      {ParamType::TextAlign, R"(text-align: {};)"},
      {ParamType::Padding, R"(padding: {};)"},
      {ParamType::Margin, R"(margin: {};)"},
      {ParamType::MinWidth, R"(min-width: {};)"},
      {ParamType::MinHeight, R"(min-height: {};)"},
      {ParamType::MaxWidth, R"(max-width: {};)"},
      {ParamType::MaxHeight, R"(max-height: {};)"}};

  static constexpr inline std::string_view HTMLBodyTemplate = R"(
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
  {}
  <body>
  )";
};
}  // namespace htmlTranslationModule
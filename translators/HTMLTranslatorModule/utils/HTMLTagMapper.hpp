/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "keynodes/HTMLTranslatorKeynodes.hpp"

#include <cstdint>
#include <map>
#include <string_view>
#include <unordered_map>

#include <sc-memory/sc_keynodes.hpp>

namespace htmlTranslationModule
{
class HTMLTagMapper
{
public:
  enum class ComponentTypes : std::uint8_t
  {
    Image,
    Sound,
    Text,
    Divider,
    Spacer,
    Menu,
    MenuBar,
    List,
    Scroll,
    Window,
    TextField,
    TextArea,
    Slider,
    Select,
    Button,
    MenuItem
  };
  enum class ComponentParams : std::uint8_t
  {
    Name,
    X,
    Y,
    Width,
    Height,
    Order,
    Visible,
    Enabled,
    Opacity,
    TextValue,
    Tooltip,
    ActionId,
    BackgroundColor,
    Color,
    BorderColor,
    BorderWidth,
    BorderStyle,
    CornerRadius,
    FontSize,
    FontWeight,
    TextAlign,
    Padding,
    Margin,
    MinWidth,
    MinHeight,
    MaxWidth,
    MaxHeight
  };

  static std::map<ScKeynode, ComponentTypes> const & GetComponentTypeByKeynodeClass()
  {
    static std::map<ScKeynode, ComponentTypes> const componentTypeByKeynodeClass = {
        {HTMLTranslatorKeynodes::concept_ui_image, ComponentTypes::Image},
        {HTMLTranslatorKeynodes::concept_ui_sound, ComponentTypes::Sound},
        {HTMLTranslatorKeynodes::concept_ui_text, ComponentTypes::Text},
        {HTMLTranslatorKeynodes::concept_ui_divider, ComponentTypes::Divider},
        {HTMLTranslatorKeynodes::concept_ui_spacer, ComponentTypes::Spacer},
        {HTMLTranslatorKeynodes::concept_ui_menu, ComponentTypes::Menu},
        {HTMLTranslatorKeynodes::concept_ui_menu_bar, ComponentTypes::MenuBar},
        {HTMLTranslatorKeynodes::concept_ui_list, ComponentTypes::List},
        {HTMLTranslatorKeynodes::concept_ui_scroll, ComponentTypes::Scroll},
        {HTMLTranslatorKeynodes::concept_ui_window, ComponentTypes::Window},
        {HTMLTranslatorKeynodes::concept_ui_text_field, ComponentTypes::TextField},
        {HTMLTranslatorKeynodes::concept_ui_text_area, ComponentTypes::TextArea},
        {HTMLTranslatorKeynodes::concept_ui_slider, ComponentTypes::Slider},
        {HTMLTranslatorKeynodes::concept_ui_select, ComponentTypes::Select},
        {HTMLTranslatorKeynodes::concept_ui_button, ComponentTypes::Button},
        {HTMLTranslatorKeynodes::concept_ui_menu_item, ComponentTypes::MenuItem}};
    return componentTypeByKeynodeClass;
  }

  static std::map<ScKeynode, ComponentParams> const & GetComponentParamByKeynodeClass()
  {
    static std::map<ScKeynode, ComponentParams> const componentParamByKeynodeClass = {
        {HTMLTranslatorKeynodes::concept_ui_name, ComponentParams::Name},
        {HTMLTranslatorKeynodes::concept_ui_x, ComponentParams::X},
        {HTMLTranslatorKeynodes::concept_ui_y, ComponentParams::Y},
        {HTMLTranslatorKeynodes::concept_ui_width, ComponentParams::Width},
        {HTMLTranslatorKeynodes::concept_ui_height, ComponentParams::Height},
        {HTMLTranslatorKeynodes::concept_ui_order, ComponentParams::Order},
        {HTMLTranslatorKeynodes::concept_ui_visible, ComponentParams::Visible},
        {HTMLTranslatorKeynodes::concept_ui_enabled, ComponentParams::Enabled},
        {HTMLTranslatorKeynodes::concept_ui_opacity, ComponentParams::Opacity},
        {HTMLTranslatorKeynodes::concept_ui_text_value, ComponentParams::TextValue},
        {HTMLTranslatorKeynodes::concept_ui_tooltip, ComponentParams::Tooltip},
        {HTMLTranslatorKeynodes::concept_ui_action_id, ComponentParams::ActionId},
        {HTMLTranslatorKeynodes::concept_ui_background_color, ComponentParams::BackgroundColor},
        {HTMLTranslatorKeynodes::concept_ui_color, ComponentParams::Color},
        {HTMLTranslatorKeynodes::concept_ui_border_color, ComponentParams::BorderColor},
        {HTMLTranslatorKeynodes::concept_ui_border_width, ComponentParams::BorderWidth},
        {HTMLTranslatorKeynodes::concept_ui_border_style, ComponentParams::BorderStyle},
        {HTMLTranslatorKeynodes::concept_ui_corner_radius, ComponentParams::CornerRadius},
        {HTMLTranslatorKeynodes::concept_ui_font_size, ComponentParams::FontSize},
        {HTMLTranslatorKeynodes::concept_ui_font_weight, ComponentParams::FontWeight},
        {HTMLTranslatorKeynodes::concept_ui_text_align, ComponentParams::TextAlign},
        {HTMLTranslatorKeynodes::concept_ui_padding, ComponentParams::Padding},
        {HTMLTranslatorKeynodes::concept_ui_margin, ComponentParams::Margin},
        {HTMLTranslatorKeynodes::concept_ui_min_width, ComponentParams::MinWidth},
        {HTMLTranslatorKeynodes::concept_ui_min_height, ComponentParams::MinHeight},
        {HTMLTranslatorKeynodes::concept_ui_max_width, ComponentParams::MaxWidth},
        {HTMLTranslatorKeynodes::concept_ui_max_height, ComponentParams::MaxHeight}};
    return componentParamByKeynodeClass;
  };

  static inline std::unordered_map<ComponentTypes, std::string_view> const HTMLTemplateByComponentType = {
      {ComponentTypes::Image, R"(<img style="{}" src="{}" alt="">)"},
      {ComponentTypes::Sound, R"(<audio style="{}" src="{}" controls></audio>)"},
      {ComponentTypes::Text, R"(<p style="{}">{}</p>)"},
      {ComponentTypes::Divider, R"(<hr style="{}">)"},
      {ComponentTypes::Spacer, R"(<div style="{}"></div>)"},
      {ComponentTypes::Menu, R"(<nav style="{}">{}</nav>)"},
      {ComponentTypes::MenuBar, R"(<div role="menubar" style="{}">{}</div>)"},
      {ComponentTypes::List, R"(<ul style="{}">{}</ul>)"},
      {ComponentTypes::Scroll, R"(<div style="{};overflow:auto">{}</div>)"},
      {ComponentTypes::Window, R"(<section role="dialog" style="{}">{}</section>)"},
      {ComponentTypes::TextField, R"(<input type="text" style="{}" value="{}">)"},
      {ComponentTypes::TextArea, R"(<textarea style="{}">{}</textarea>)"},
      {ComponentTypes::Slider, R"(<input type="range" style="{}" value="{}">)"},
      {ComponentTypes::Select, R"(<select style="{}">{}</select>)"},
      {ComponentTypes::Button, R"(<button type="button" style="{}">{}</button>)"},
      {ComponentTypes::MenuItem, R"(<li role="menuitem" style="{}">{}</li>)"}};

  static inline std::unordered_map<ComponentParams, std::string_view> const HTMLTemplateByParamType = {
      {ComponentParams::X, R"(left: {};)"},
      {ComponentParams::Y, R"(top: {};)"},
      {ComponentParams::Width, R"(width: {};)"},
      {ComponentParams::Height, R"(height: {};)"},
      {ComponentParams::Order, R"(order: {};)"},
      {ComponentParams::Visible, R"(visibility: {};)"},
      {ComponentParams::Opacity, R"(opacity: {};)"},
      {ComponentParams::BackgroundColor, R"(background-color: {};)"},
      {ComponentParams::Color, R"(color: {};)"},
      {ComponentParams::BorderColor, R"(border-color: {};)"},
      {ComponentParams::BorderWidth, R"(border-width: {};)"},
      {ComponentParams::BorderStyle, R"(border-style: {};)"},
      {ComponentParams::CornerRadius, R"(border-radius: {};)"},
      {ComponentParams::FontSize, R"(font-size: {};)"},
      {ComponentParams::FontWeight, R"(font-weight: {};)"},
      {ComponentParams::TextAlign, R"(text-align: {};)"},
      {ComponentParams::Padding, R"(padding: {};)"},
      {ComponentParams::Margin, R"(margin: {};)"},
      {ComponentParams::MinWidth, R"(min-width: {};)"},
      {ComponentParams::MinHeight, R"(min-height: {};)"},
      {ComponentParams::MaxWidth, R"(max-width: {};)"},
      {ComponentParams::MaxHeight, R"(max-height: {};)"}};
};
}  // namespace htmlTranslationModule
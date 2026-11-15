/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace htmlTranslationModule
{
enum class ComponentType : std::uint8_t
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
enum class ParamType : std::uint8_t
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
enum class LinkType : std::uint8_t
{
  Contains
};

struct UIComponent
{
  std::string ID;
  ComponentType Type;
  std::unordered_map<ComponentType, std::string> Params;
};

struct UILink
{
  std::string ID;
  LinkType Type;
  std::string Source;
  std::string Target;
};
}  // namespace htmlTranslationModule
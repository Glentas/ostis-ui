/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_agent_context.hpp>

namespace specifiedStringTemplateModule
{

class StringTemplateRenderer
{
public:
  static std::string RenderStringTemplate(ScAgentContext & context, ScAddr const & stringTemplateLink, ScAddr const & stringTemplateLinkReplacements, ScAddr const & stringFormatAddr);
  static ScTemplateParams GetScTemplateParamsFromTemplateReplacements(ScAgentContext & context, ScAddr const & templateAddr, ScAddr const & stringTemplateLinkReplacements);
 
  // modified by me
  static std::string RenderStringTemplate1(ScAgentContext & context, ScAddr const & stringTemplateLink, ScAddr const & stringTemplateLinkReplacements, ScAddr const & stringFormatAddr);
  static ScTemplateParams GetScTemplateParamsFromTemplateReplacements1(ScAgentContext & context, ScAddr const & templateAddr, ScAddr const & stringTemplateLinkReplacements);
};

} // namespace specifiedStringTemplateModule

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/SpecifiedStringTemplateAgent.hpp"

#include "SpecifiedStringTemplateModule.hpp"

using namespace specifiedStringTemplateModule;

SC_MODULE_REGISTER(SpecifiedStringTemplateModule)
  ->Agent<SpecifiedStringTemplateAgent>();

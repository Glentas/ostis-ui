/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "renderer/JsonToXMLAgent.hpp"
#include "renderer/XMLToJsonAgent.hpp"
#include "renderer/TemplateActionGenerator.hpp"

#include "SpecifiedStringTemplateModule.hpp"

using namespace specifiedStringTemplateModule;

SC_MODULE_REGISTER(SpecifiedStringTemplateModule)
    ->Agent<JsonToXMLAgent>()
    ->Agent<XMLToJsonAgent>()
    ->Agent<GenerateTemplateAgent>();

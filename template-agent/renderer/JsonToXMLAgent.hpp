/*
 * Translate json to XML
 */

#pragma once

#include <string>

#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>
#include <nlohmann/json.hpp>

namespace specifiedStringTemplateModule
{
class JsonToXMLAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

  std::string escapeXml(const std::string& str);

  std::string sanitizeTagName(const std::string& name);

  std::string jsonToXml(const nlohmann::json& j, const std::string& tagName = "item");

  std::string convertJsonToXml(const std::string& jsonStr);

};
}  // namespace specifiedStringTemplateModulebui

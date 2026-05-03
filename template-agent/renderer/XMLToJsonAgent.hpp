/*
 * Translate XML to json
 */

#pragma once

#include <string>

#include <sc-memory/sc_agent.hpp>
#include <sc-memory/sc_memory.hpp>
#include <nlohmann/json.hpp>

namespace specifiedStringTemplateModule
{
class XMLToJsonAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

  std::string trim(const std::string& str);

  bool isNumber(const std::string& str);

  void processText(std::string& currentText, nlohmann::json* currentElement);

  std::map<std::string, std::string> parseAttributes(const std::string& attrsStr);

  void parseTag(const std::string& tagContent, std::string& name, bool& isClosing, bool& selfClosing, 
              std::map<std::string, std::string>& attributes);

  void addToParent(nlohmann::json* parent, const std::string& tagName, nlohmann::json& element,
                 std::stack<nlohmann::json*>& stack, std::stack<std::string>& tagStack, bool selfClosing);

  std::string xmlToJson(const std::string& xml);

};
}  // namespace specifiedStringTemplateModulebui

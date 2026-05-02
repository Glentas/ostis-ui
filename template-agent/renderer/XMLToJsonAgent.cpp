

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "XMLToJsonAgent.hpp"


#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>


#include <sc-agents-common/utils/GenerationUtils.hpp>

#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"

#include <sstream>

#include <stack>
#include <map>
#include <regex>
#include <cctype>
#include <nlohmann/json.hpp>


using namespace utils;

namespace specifiedStringTemplateModule
{

ScResult XMLToJsonAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{

    auto [link_param] = action.GetArguments<1>();
    SC_LOG_INFO("XMLToJsonAgent started" );
      if (!m_context.IsElement(link_param))
        {
           SC_THROW_EXCEPTION(
            utils::ExceptionItemNotFound, "Link not found");
        }


    std::string link_content_xml;
    bool const templateStringLinkExists = m_context.GetLinkContent(link_param, link_content_xml);

    if (!templateStringLinkExists)
    {
        SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "XMLToJsonAgent: string template link has no content.");
    }
    
    
    std::string json = xmlToJson(link_content_xml);
    
    bool const result_of_translation = m_context.SetLinkContent(link_param, json);

    if (!result_of_translation)
    {
        SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "XMLToJsonAgent: string template link has no content.");
    }

    SC_LOG_INFO("XMLToJsonAgent finished " << json);
    return action.FinishSuccessfully();
}




// ============== Вспомогательные функции ==============
std::string XMLToJsonAgent::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// ============== Парсинг атрибутов ==============
std::map<std::string, std::string> XMLToJsonAgent::parseAttributes(const std::string& attrsStr) {
    std::map<std::string, std::string> attrs;
    size_t pos = 0;
    
    while (pos < attrsStr.length()) {
        while (pos < attrsStr.length() && attrsStr[pos] == ' ') pos++;
        if (pos >= attrsStr.length()) break;
        
        size_t eqPos = attrsStr.find('=', pos);
        if (eqPos == std::string::npos) break;
        
        std::string attrName = attrsStr.substr(pos, eqPos - pos);
        pos = eqPos + 1;
        
        while (pos < attrsStr.length() && attrsStr[pos] == ' ') pos++;
        
        if (pos < attrsStr.length() && (attrsStr[pos] == '"' || attrsStr[pos] == '\'')) {
            char quote = attrsStr[pos];
            pos++;
            size_t valueStart = pos;
            
            while (pos < attrsStr.length() && attrsStr[pos] != quote) pos++;
            std::string attrValue = attrsStr.substr(valueStart, pos - valueStart);
            attrs[attrName] = attrValue;
            pos++;
        }
    }
    return attrs;
}

// ============== Обработка текста ==============
bool XMLToJsonAgent::isNumber(const std::string& str) {
    if (str.empty()) return false;
    
    size_t i = 0;
    if (str[0] == '-') i++;
    
    bool hasDigit = false;
    bool hasDot = false;
    
    for (; i < str.length(); i++) {
        if (std::isdigit(str[i])) {
            hasDigit = true;
        } else if (str[i] == '.' && !hasDot) {
            hasDot = true;
        } else {
            return false;
        }
    }
    
    return hasDigit;
}

void XMLToJsonAgent::processText(std::string& currentText, nlohmann::json* currentElement) {
    std::string trimmedText = trim(currentText);
    if (!trimmedText.empty() && currentElement->is_object()) {
        nlohmann::json value;
        
        if (isNumber(trimmedText)) {
            if (trimmedText.find('.') != std::string::npos) {
                value = std::stod(trimmedText);
            } else {
                value = std::stoi(trimmedText);
            }
        } else {
            value = trimmedText;
        }
        
        if (currentElement->empty()) {
            *currentElement = value;
        } else if (currentElement->is_object() && !currentElement->contains("#text")) {
            (*currentElement)["#text"] = value;
        }
    }
    currentText.clear();
}

// ============== Разбор тега ==============
void XMLToJsonAgent::parseTag(const std::string& tagContent, std::string& name, bool& isClosing, bool& selfClosing, 
              std::map<std::string, std::string>& attributes) {
    isClosing = false;
    selfClosing = false;
    attributes.clear();
    
    std::string content = tagContent;
    
    if (!content.empty() && content[0] == '/') {
        isClosing = true;
        name = content.substr(1);
        return;
    }
    
    if (!content.empty() && content.back() == '/') {
        selfClosing = true;
        content.pop_back();
    }
    
    size_t spacePos = content.find(' ');
    if (spacePos != std::string::npos) {
        name = content.substr(0, spacePos);
        std::string attrsStr = content.substr(spacePos + 1);
        attributes = parseAttributes(attrsStr);
    } else {
        name = content;
    }
}

// ============== Добавление элемента в родителя ==============
void XMLToJsonAgent::addToParent(nlohmann::json* parent, const std::string& tagName, nlohmann::json& element,
                 std::stack<nlohmann::json*>& stack, std::stack<std::string>& tagStack, bool selfClosing) {
    if (!parent->is_object()) return;
    
    if (parent->contains(tagName)) {
        if (!parent->at(tagName).is_array()) {
            nlohmann::json existing = parent->at(tagName);
            parent->operator[](tagName) = nlohmann::json::array();
            parent->at(tagName).push_back(existing);
        }
        parent->at(tagName).push_back(element);
        if (!selfClosing) {
            stack.push(&parent->at(tagName).back());
            tagStack.push(tagName);
        }
    } else {
        (*parent)[tagName] = element;
        if (!selfClosing) {
            stack.push(&(*parent)[tagName]);
            tagStack.push(tagName);
        }
    }
}

// ============== Основная функция ==============
std::string XMLToJsonAgent::xmlToJson(const std::string& xml) {
    nlohmann::json root = nlohmann::json::object();
    std::stack<nlohmann::json*> stack;
    std::stack<std::string> tagStack;
    stack.push(&root);
    
    std::string currentText;
    size_t pos = 0;
    
    while (pos < xml.length()) {
        if (xml[pos] == '<') {
            if (!currentText.empty()) {
                processText(currentText, stack.top());
            }
            
            size_t end = xml.find('>', pos);
            if (end == std::string::npos) break;
            
            std::string tagContent = xml.substr(pos + 1, end - pos - 1);
            
            std::string name;
            bool isClosing, selfClosing;
            std::map<std::string, std::string> attrs;
            parseTag(tagContent, name, isClosing, selfClosing, attrs);
            
            if (isClosing) {
                if (!tagStack.empty() && tagStack.top() == name) {
                    tagStack.pop();
                    stack.pop();
                }
            } else {
                nlohmann::json element = nlohmann::json::object();
                
                if (!attrs.empty()) {
                    element["@attributes"] = nlohmann::json::object();
                    for (const auto& attr : attrs) {
                        element["@attributes"][attr.first] = attr.second;
                    }
                }
                
                addToParent(stack.top(), name, element, stack, tagStack, selfClosing);
                
                if (selfClosing) {
                    stack.pop();
                    tagStack.pop();
                }
            }
            
            pos = end + 1;
        } else {
            currentText += xml[pos];
            pos++;
        }
    }
    
    if (!currentText.empty()) {
        processText(currentText, stack.top());
    }
    
    return root.is_null() ? "{}" : root.dump(2);
}



ScAddr XMLToJsonAgent::GetActionClass() const
{
  return SpecifiedStringTemplateKeynodes::action_translate_XML_to_json;
}

}  // namespace specifiedStringTemplateModule

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "JsonToXMLAgent.hpp"


#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>


#include <sc-agents-common/utils/GenerationUtils.hpp>

#include "keynodes/SpecifiedStringTemplateKeynodes.hpp"

#include <nlohmann/json.hpp>
#include <sstream>

using namespace utils;

namespace specifiedStringTemplateModule
{

ScResult JsonToXMLAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{

    auto [link_param] = action.GetArguments<1>();

      if (!m_context.IsElement(link_param))
        {
           SC_THROW_EXCEPTION(
            utils::ExceptionItemNotFound, "Link not found");
        }


    std::string link_content_json;
    bool const templateStringLinkExists = m_context.GetLinkContent(link_param, link_content_json);

    if (!templateStringLinkExists)
    {
        SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "JsonToXMLAgent: string template link has no content.");
    }
    
    
    std::string xml = convertJsonToXml(link_content_json);
    
    bool const result_of_translation = m_context.SetLinkContent(link_param, xml);

    if (!result_of_translation)
    {
        SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "JsonToXMLAgent: string template link has no content.");
    }

    SC_LOG_INFO("GenerateTemplateAgent finished " << xml);
    return action.FinishSuccessfully();
}


// Экранирование XML спецсимволов
std::string JsonToXMLAgent::escapeXml(const std::string& str) {
    std::string result;
    result.reserve(str.length() * 1.2);
    
    for (char c : str) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default: result += c; break;
        }
    }
    return result;
}

// Очистка имени тега
std::string JsonToXMLAgent::sanitizeTagName(const std::string& name) {
    if (name.empty()) return "item";
    
    std::string result;
    for (char c : name) {
        if (isalnum(c) || c == '_' || c == '-' || c == ':') {
            result += c;
        } else {
            result += '_';
        }
    }
    
    if (isdigit(result[0])) {
        result = "_" + result;
    }
    
    return result;
}

// Преобразование JSON в XML (рекурсивная)
std::string JsonToXMLAgent::jsonToXml(const nlohmann::json& j, const std::string& tagName) {
    std::stringstream xml;
    
    switch (j.type()) {
        case nlohmann::json::value_t::object:
            for (auto& [key, value] : j.items()) {
                std::string safeKey = sanitizeTagName(key);
                xml << "<" << safeKey << ">";
                xml << jsonToXml(value, safeKey);
                xml << "</" << safeKey << ">";
            }
            break;
            
        case nlohmann::json::value_t::array:
            for (const auto& item : j) {
                xml << "<" << tagName << ">";
                xml << jsonToXml(item, tagName);
                xml << "</" << tagName << ">";
            }
            break;
            
        case nlohmann::json::value_t::string:
            xml << escapeXml(j.get<std::string>());
            break;
            
        case nlohmann::json::value_t::number_integer:
        case nlohmann::json::value_t::number_unsigned:
        case nlohmann::json::value_t::number_float:
            xml << j.dump();
            break;
            
        case nlohmann::json::value_t::boolean:
            xml << (j.get<bool>() ? "true" : "false");
            break;
            
        case nlohmann::json::value_t::null:
            xml << "";
            break;
            
        default:
            xml << "";
            break;
    }
    
    return xml.str();
}

// Главная функция конвертации
std::string JsonToXMLAgent::convertJsonToXml(const std::string& jsonStr) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        return jsonToXml(j, "root");
    } catch (const std::exception& e) {
        return "<error>" + std::string(e.what()) + "</error>";
    }
}



ScAddr JsonToXMLAgent::GetActionClass() const
{
  return SpecifiedStringTemplateKeynodes::action_translate_json_to_XML;
}

}  // namespace specifiedStringTemplateModule

#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "VisualAdaptationAgent.hpp"
#include "parameter-retriever/ParameterRetriever.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace htmlTranslationModule
{

// Универсальная функция очистки CSS-значения от мусора БЗ
static std::string cleanCssValue(const std::string& raw) {
    std::string cleaned = raw;
    // Удаляем кавычки, пробелы, табы, переносы строк
    cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(), [](unsigned char c) {
        return std::isspace(c) || c == '"' || c == '\'' || c == '\n' || c == '\r' || c == '\t';
    }), cleaned.end());
    
    // Удаляем артефактный префикс "float:"
    if (cleaned.size() >= 6 && cleaned.substr(0, 6) == "float:") {
        cleaned = cleaned.substr(6);
    }
    return cleaned;
}

ScResult VisualAdaptationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
    SC_LOG_INFO("ХОХОХЕХЕ");
    
    auto const [component, multiplierLink] = action.GetArguments<2>();
    if (!component.IsValid()) {
        SC_LOG_ERROR("VisualAdaptationAgent: component is invalid.");
        return action.FinishUnsuccessfully();
    }

    double multiplier = 1.0;
    if (multiplierLink.IsValid()) {
        std::string multiplierStr;
        m_context.GetLinkContent(multiplierLink, multiplierStr);
        try { multiplier = std::stod(multiplierStr); } 
        catch (...) { SC_LOG_WARNING("Invalid multiplier, using 1.0"); }
    }

    SC_LOG_INFO("VisualAdaptation: Applying multiplier " + std::to_string(multiplier));

    StringScAddrMap parameters = ParameterRetriever::GetNestedUIComponents(m_context, component);

    for (auto const & [id, paramAddr] : parameters) {
        if (id.find("fz") == 0 || id.find("width") == 0 || id.find("height") == 0 ||
            id.find("margin") == 0 || id.find("padding") == 0 ||
            id.find("line-height") == 0 || id.find("font-weight") == 0)
        {
            ScAddr valueLink = utils::IteratorUtils::getAnyByOutRelation(&m_context, paramAddr, HTMLTranslatorKeynodes::nrel_html_representation);
            if (!m_context.IsElement(valueLink) || !m_context.GetElementType(valueLink).IsLink()) continue;

            std::string currentValueStr;
            m_context.GetLinkContent(valueLink, currentValueStr);
            std::string cleanValue = cleanCssValue(currentValueStr);

            if (cleanValue.empty()) continue;

            try {
                size_t pos = 0;
                double value = std::stod(cleanValue, &pos);
                std::string unit = cleanValue.substr(pos);

                // Дополнительная зачистка юнита от спецсимволов
                unit.erase(std::remove_if(unit.begin(), unit.end(), [](unsigned char c) {
                    return c == '"' || c == '\'' || c == '=' || c == ';' || std::isspace(c);
                }), unit.end());

                double newValue = value * multiplier;
                std::ostringstream numSs;
                numSs << std::fixed << std::setprecision(1) << newValue;
                std::string numStr = numSs.str();

                // Убираем лишние нули (2.0 -> 2)
                if (numStr.find('.') != std::string::npos) {
                    numStr.erase(numStr.find_last_not_of('0') + 1, std::string::npos);
                    if (!numStr.empty() && numStr.back() == '.') numStr.pop_back();
                }

                std::string finalValue = numStr + unit;
                m_context.SetLinkContent(valueLink, finalValue);
                SC_LOG_INFO("VisualAdaptation: Updated " + id + " -> " + finalValue);
            } 
            catch (...) {
                // чтобы не сломать валидный CSS. Логируем и пропускаем.
                SC_LOG_WARNING("VisualAdaptation: Non-numeric " + id + " ('" + cleanValue + "'). Keeping original.");
            }
        }
    }

    return action.FinishSuccessfully();
}

ScAddr VisualAdaptationAgent::GetActionClass() const {
    return HTMLTranslatorKeynodes::action_apply_visual_adaptation;
}

}  // namespace htmlTranslationModule
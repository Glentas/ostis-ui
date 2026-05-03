#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "VisualAdaptationAgent.hpp"
#include "parameter-retriever/ParameterRetriever.hpp"
#include <iomanip>
#include <sstream>
#include <cmath>
#include <iomanip>

namespace htmlTranslationModule
{

// Универсальная функция очистки CSS-значения от мусора БЗ
bool IsScalableProperty(const std::string& id)
{
    static const std::vector<std::string> prefixes = {
        "width", "height", "fz", "font-size", "margin", "padding", 
        "line-height", "border-width", "border-radius", "top", "left", "bottom", "right"
    };

    for (const auto& p : prefixes) {
        if (id.find(p) == 0) return true;
    }
    return false;
}

// Основная умная функция парсинга
std::string ScaleCssValue(const std::string& value, double multiplier)
{
    if (value.empty() || std::abs(multiplier - 1.0) < 0.001) 
        return value;

    std::string v = value;
    // trim
    v.erase(0, v.find_first_not_of(" \t\r\n"));
    v.erase(v.find_last_not_of(" \t\r\n") + 1);

    if (v == "auto" || v == "none" || v.find_first_of(":#(") == 0) 
        return value;

    std::stringstream ss(v);
    double num = 0.0;
    std::string unit;

    if (!(ss >> num)) return value;

    ss >> unit;

    double newNum = num * multiplier;

    std::ostringstream oss;
    if (newNum == std::floor(newNum) && newNum < 1000) {
        oss << static_cast<long long>(newNum);
    } else {
        oss << std::fixed << std::setprecision(2) << newNum;
    }
    oss << unit;

    return oss.str();
}


ScResult VisualAdaptationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
    auto const [component, multiplierLink] = action.GetArguments<2>();
    if (!component.IsValid()) {
        SC_LOG_ERROR("VisualAdaptationAgent: component is invalid.");
        return action.FinishUnsuccessfully();
    }

    double multiplier = 1.0;
    if (multiplierLink.IsValid()) {
        std::string multiplierStr;
        m_context.GetLinkContent(multiplierLink, multiplierStr);
        try {
            multiplier = std::stod(multiplierStr);
        } catch (...) {
            SC_LOG_WARNING("VisualAdaptationAgent: failed to parse multiplier '" + multiplierStr + "', using 1.0");
        }
    }

    if (std::abs(multiplier - 1.0) < 0.001) {
        return action.FinishSuccessfully(); // ничего не делаем
    }

    SC_LOG_INFO("VisualAdaptation: Applying multiplier " + std::to_string(multiplier) + " to component.");

    StringScAddrMap parameters = ParameterRetriever::GetNestedUIComponents(m_context, component);

    for (auto const& [id, paramAddr] : parameters)
    {
        // Расширили список, но будем проверять внутри
        if (!IsScalableProperty(id)) continue;

        ScAddr valueLink = utils::IteratorUtils::getAnyByOutRelation(
            &m_context, paramAddr, HTMLTranslatorKeynodes::nrel_html_representation);

        if (!valueLink.IsValid() || !m_context.GetElementType(valueLink).IsLink()) {
            continue;
        }

        std::string currentValueStr;
        m_context.GetLinkContent(valueLink, currentValueStr);

        std::string newValueStr = ScaleCssValue(currentValueStr, multiplier);

        if (newValueStr != currentValueStr) {
            SC_LOG_INFO("VisualAdaptation: " + id + " : " + currentValueStr + " → " + newValueStr);
            m_context.SetLinkContent(valueLink, newValueStr);
        }
    }
    SC_LOG_INFO("Visual adaptation completed successfully");
    return action.FinishSuccessfully();
}

ScAddr VisualAdaptationAgent::GetActionClass() const {
    return HTMLTranslatorKeynodes::action_apply_visual_adaptation;
}

}  // namespace htmlTranslationModule
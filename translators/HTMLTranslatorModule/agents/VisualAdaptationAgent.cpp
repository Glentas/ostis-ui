#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "VisualAdaptationAgent.hpp"
#include "html-translator/HTMLTranslator.hpp"
#include "parameter-retriever/ParameterRetriever.hpp"

#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace htmlTranslationModule
{

// ===================================================================
static bool IsScalableProperty(const std::string & id)
{
    static const std::vector<std::string> prefixes = {
        "fz", "font-size", "width", "height", "margin", "padding",
        "line-height", "border-radius"
    };
    for (const auto & p : prefixes)
        if (id.find(p) == 0) return true;
    return false;
}

static std::string SanitizeCssValue(const std::string & raw)
{
    std::string v = raw;

    // Убираем пробелы, кавычки, точку с запятой
    v.erase(std::remove_if(v.begin(), v.end(), [](unsigned char c) {
        return c == '"' || c == '\'' || c == ';';
    }), v.end());

    // Убираем float: префикс
    if (v.size() >= 6 && v.compare(0, 6, "float:") == 0)
        v = v.substr(6);

    return v;
}

// ===================================================================
static std::string ScaleCssValue(const std::string & rawValue, double multiplier)
{
    if (std::abs(multiplier - 1.0) < 0.001)
        return rawValue;

    std::string v = rawValue;

    v.erase(std::remove_if(v.begin(), v.end(), [](unsigned char c) {
        return std::isspace(c) || c == '"' || c == '\'' || c == ';';
    }), v.end());

    if (v.size() >= 6 && v.compare(0, 6, "float:") == 0)
        v = v.substr(6);

    if (v.empty() || v == "auto" || v == "none" || v[0] == '#' ||
        v.find("rgb") == 0 || v.find("hsl") == 0)
        return rawValue;

    // Защита от процентов — не масштабируем
    if (v.back() == '%')
        return rawValue;

    // Защита от calc(), var() и т.п.
    if (v.find('(') != std::string::npos)
        return rawValue;

    std::istringstream ss(v);
    double num = 0.0;
    std::string unit;

    if (!(ss >> num)) return rawValue;
    ss >> unit;

    double newValue = num * multiplier;

    std::ostringstream oss;
    if (std::floor(newValue) == newValue)
        oss << static_cast<long long>(newValue);
    else
        oss << std::fixed << std::setprecision(2) << newValue;
    oss << unit;

    return oss.str();
}

// ===================================================================
static void InvalidateHTMLCache(ScAgentContext & ctx, ScAddr component)
{
    ScAddr cached = utils::IteratorUtils::getAnyByOutRelation(
        &ctx, component, HTMLTranslatorKeynodes::nrel_html_representation);

    if (cached.IsValid())
        ctx.EraseElement(cached);

    ScIterator3Ptr it = ctx.CreateIterator3(ScType::Unknown, ScType::ConstPermPosArc, component);
    while (it->Next())
    {
        ScAddr parent = it->Get(0);
        ScAddr arc    = it->Get(1);

        if (ctx.CheckConnector(HTMLTranslatorKeynodes::nrel_inclusion, arc, ScType::ConstPermPosArc))
            InvalidateHTMLCache(ctx, parent);
    }
}

// ===================================================================
ScResult VisualAdaptationAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
    ScAddr component = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, action, ScKeynodes::rrel_1);
    ScAddr multiplierLink = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, action, ScKeynodes::rrel_2);

    SC_LOG_INFO("VisualAdaptationAgent: component hash = " + std::to_string(component.Hash()));
    SC_LOG_INFO("VisualAdaptationAgent: multiplierLink hash = " + std::to_string(multiplierLink.Hash()));

    if (!component.IsValid())
    {
        SC_LOG_ERROR("VisualAdaptationAgent: component is invalid.");
        return action.FinishUnsuccessfully();
    }

    double multiplier = 1.0;
    if (multiplierLink.IsValid())
    {
        std::string s;
        m_context.GetLinkContent(multiplierLink, s);
        try { multiplier = std::stod(s); }
        catch (...)
        {
            SC_LOG_WARNING("VisualAdaptationAgent: failed to parse multiplier, using 1.0");
        }
    }

    SC_LOG_INFO("VisualAdaptationAgent: multiplier = " + std::to_string(multiplier));

    auto parameters = ParameterRetriever::GetNestedUIComponents(m_context, component);

    for (auto const & [id, paramAddr] : parameters)
{
    ScAddr valueLink = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, paramAddr, HTMLTranslatorKeynodes::nrel_html_representation);

    if (!valueLink.IsValid() || !m_context.GetElementType(valueLink).IsLink())
        continue;

    std::string currentValue;
    m_context.GetLinkContent(valueLink, currentValue);

    std::string newValue = SanitizeCssValue(currentValue); // ← всегда чистим

    if (IsScalableProperty(id))
        newValue = ScaleCssValue(newValue, multiplier);    // ← масштабируем только нужные

    if (newValue != currentValue)
    {
        m_context.SetLinkContent(valueLink, newValue);
        SC_LOG_INFO("VisualAdaptationAgent: " + id + " : " + currentValue + " → " + newValue);
    }
}

    InvalidateHTMLCache(m_context, component);
    SC_LOG_INFO("VisualAdaptationAgent: done.");

    try
    {
        ScAddr newRepr = HTMLTranslator::RegenerateHTMLRepresentation(m_context, component);
        if (newRepr.IsValid())
            SC_LOG_INFO("VisualAdaptationAgent: HTML regenerated successfully.");
        else
            SC_LOG_WARNING("VisualAdaptationAgent: HTML regeneration returned invalid link.");
    }
    catch (std::exception const & e)
    {
        SC_LOG_ERROR("VisualAdaptationAgent: HTML regeneration failed: " + std::string(e.what()));
    }

    return action.FinishSuccessfully();
}

ScAddr VisualAdaptationAgent::GetActionClass() const
{
    return HTMLTranslatorKeynodes::action_apply_visual_adaptation;
}

}  // namespace htmlTranslationModule
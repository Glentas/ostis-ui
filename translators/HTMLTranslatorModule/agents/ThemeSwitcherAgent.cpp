#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "ThemeSwitcherAgent.hpp"
#include "html-translator/HTMLTranslator.hpp"
#include "parameter-retriever/ParameterRetriever.hpp"

#include <unordered_map>
#include <string>

namespace htmlTranslationModule
{

using ThemeMap = std::unordered_map<std::string, std::string>;

static ThemeMap LoadThemeFromKB(ScAgentContext & ctx, std::string const & themeName)
{
    ThemeMap result;

    // 1. Ищем узел темы по nrel_theme_name
    ScAddr themeNode;
    ScTemplate findTheme;
    findTheme.Quintuple(
        ScType::VarNode >> "theme",
        ScType::VarCommonArc,
        ScType::VarNodeLink >> "name_link",
        ScType::VarPermPosArc,
        HTMLTranslatorKeynodes::nrel_theme_name);

    ctx.SearchByTemplate(findTheme, [&](ScTemplateSearchResultItem const & item)
    {
        ScAddr nameLink;
        item.Get("name_link", nameLink);
        std::string name;
        ctx.GetLinkContent(nameLink, name);
        if (name == themeName)
            item.Get("theme", themeNode);
    });

    if (!ctx.IsElement(themeNode))
    {
        SC_LOG_ERROR("ThemeSwitcherAgent: theme '" + themeName + "' not found in KB.");
        return result;
    }

    // 2. Перебираем все param-узлы темы: theme -> ..param
    ScIterator3Ptr paramIt = ctx.CreateIterator3(
        themeNode, ScType::ConstPermPosArc, ScType::Unknown);

    while (paramIt->Next())
    {
        ScAddr paramNode = paramIt->Get(2);

        // Читаем nrel_css_param_id
        ScAddr idLink = utils::IteratorUtils::getAnyByOutRelation(
            &ctx, paramNode, HTMLTranslatorKeynodes::nrel_css_param_id);

        // Читаем nrel_css_param_value
        ScAddr valueLink = utils::IteratorUtils::getAnyByOutRelation(
            &ctx, paramNode, HTMLTranslatorKeynodes::nrel_css_param_value);

        if (!ctx.IsElement(idLink) || !ctx.IsElement(valueLink))
            continue;

        std::string paramId, paramValue;
        ctx.GetLinkContent(idLink, paramId);
        ctx.GetLinkContent(valueLink, paramValue);

        result[paramId] = paramValue;
        SC_LOG_INFO("ThemeSwitcherAgent: loaded param '" + paramId + "' = '" + paramValue + "'");
    }

    return result;
}

// ===================================================================
//  Применяем тему к компоненту
// ===================================================================
static void ApplyThemeToComponent(
    ScAgentContext & ctx,
    ScAddr const & component,
    ThemeMap const & theme)
{
    StringScAddrMap parameters = ParameterRetriever::GetNestedUIComponents(ctx, component);

    for (auto const & [id, paramAddr] : parameters)
    {
        // Ищем подходящий prefix из темы
        std::string const * newValue = nullptr;
        for (auto const & [prefix, value] : theme)
        {
            if (id.find(prefix) == 0)
            {
                newValue = &value;
                break;
            }
        }
        if (!newValue) continue;

        ScAddr valueLink = utils::IteratorUtils::getAnyByOutRelation(
            &ctx, paramAddr, HTMLTranslatorKeynodes::nrel_html_representation);

        if (!ctx.IsElement(valueLink) || !ctx.GetElementType(valueLink).IsLink())
            continue;

        std::string currentValue;
        ctx.GetLinkContent(valueLink, currentValue);

        if (currentValue == *newValue) continue;

        ctx.SetLinkContent(valueLink, *newValue);
        SC_LOG_INFO("ThemeSwitcherAgent: " + id + " : " + currentValue + " → " + *newValue);
    }
}

// ===================================================================
//  Агент
// ===================================================================
ScResult ThemeSwitcherAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
    ScAddr component = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, action, ScKeynodes::rrel_1);
    ScAddr themeLink = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, action, ScKeynodes::rrel_2);

    SC_LOG_INFO("ThemeSwitcherAgent: component hash = " + std::to_string(component.Hash()));

    if (!component.IsValid())
    {
        SC_LOG_ERROR("ThemeSwitcherAgent: component is invalid.");
        return action.FinishUnsuccessfully();
    }

    std::string themeName = "light";
    if (themeLink.IsValid())
    {
        if (m_context.GetElementType(themeLink).IsLink())
        {
            m_context.GetLinkContent(themeLink, themeName);
        }
        else
        {
            themeName = m_context.GetElementSystemIdentifier(themeLink);
        }
    }

    SC_LOG_INFO("ThemeSwitcherAgent: switching to theme '" + themeName + "'");

    ThemeMap theme = LoadThemeFromKB(m_context, themeName);
    if (theme.empty())
    {
        SC_LOG_ERROR("ThemeSwitcherAgent: theme is empty, aborting.");
        return action.FinishUnsuccessfully();
    }

    ApplyThemeToComponent(m_context, component, theme);

    try
    {
        ScAddr newRepr = HTMLTranslator::RegenerateHTMLRepresentation(m_context, component);
        if (newRepr.IsValid())
            SC_LOG_INFO("ThemeSwitcherAgent: HTML regenerated successfully.");
        else
            SC_LOG_WARNING("ThemeSwitcherAgent: HTML regeneration returned invalid link.");
    }
    catch (std::exception const & e)
    {
        SC_LOG_ERROR("ThemeSwitcherAgent: regeneration failed: " + std::string(e.what()));
        return action.FinishUnsuccessfully();
    }

    SC_LOG_INFO("ThemeSwitcherAgent: done.");
    return action.FinishSuccessfully();
}

ScAddr ThemeSwitcherAgent::GetActionClass() const
{
    return HTMLTranslatorKeynodes::action_switch_theme;
}

}  // namespace htmlTranslationModule
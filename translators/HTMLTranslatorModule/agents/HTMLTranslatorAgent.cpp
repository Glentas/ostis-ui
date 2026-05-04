/*
This source file is part of an OSTIS project. For the latest info, see
http://ostis.net Distributed under the MIT License (See accompanying file
COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/
/*
Purpose: transform sc-model of the UI to a state that's translatable to HTML
and launch the process of getting the full HTML component tree
Traverse through the model: create relations from sc-nodes of components to
their respective templates 1.1 Use rules to check which templates should be used
for a respective component class 1.2 generate a relation
Generate the variables needed for the templates
2.1 Check specs of the template: required variables, optional variables
2.2 Generate variable map for the templates (lazy evaluation: <name, sc-link
| sc-action | sc-template>) 2.2.1 OPTION 1: Template spec will define
sc-templates /sc-actions to get the value of the variables 2.2.1.1 Run a search
/ run agent against each required variable, add it to the map, oportunistically
complete with optional variables 2.2.1 OPTION 2: (slow but extensible) We could
check each parameter that the component has 2.2.1.1 does the parameter have an
html translation rule? we should know how can it be integrated into the
component template 2.2.1.2 each parameter that has an html template gets
inserted into a variable map 2.3 We'll get a tree of templates that depend on
each other (something like an execution plan for the templater) root template ->
variable <name, (sc-template | sc-action) -> nested template -> variable <name,
(...)>  >
Run template evaluation agent for the root of the tree
*/
#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_event.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-memory/sc_structure.hpp>
#include "html-translator/HTMLTranslator.hpp"
#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "HTMLTranslatorAgent.hpp"

using namespace utils;

namespace htmlTranslationModule
{
ScResult HTMLTranslatorAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
    SC_LOG_INFO("ХИХИХАХА");
    auto const [rootUiElement] = action.GetArguments<1>();
    if (!rootUiElement.IsValid())
    {
        SC_LOG_ERROR("Given UI element is invalid.");
        return action.FinishUnsuccessfully();
    }

    // Пробуем перегенерировать представление (для актуальности при изменении параметров)
    ScAddr answerHTMLLink;
    try
    {
        answerHTMLLink = HTMLTranslator::RegenerateHTMLRepresentation(m_context, rootUiElement);
    }
    catch (utils::ScException const & e)
    {
        // Если шаблон не найден, фоллбэк на кеш через TranslateScToHTML
        SC_LOG_WARNING("RegenerateHTMLRepresentation failed: " + std::string(e.what()) + 
                      ". Falling back to cached representation via TranslateScToHTML.");
        answerHTMLLink = HTMLTranslator::TranslateScToHTML(m_context, rootUiElement);
    }
    
    if (!answerHTMLLink.IsValid())
    {
        SC_LOG_ERROR("Failed to get HTML representation.");
        return action.FinishUnsuccessfully();
    }

    // Ищем уже существующую дугу от компонента к ссылке с HTML
    ScAddr arcAddr;
    ScIterator3Ptr itArc = m_context.CreateIterator3(rootUiElement, ScType::Unknown, answerHTMLLink);
    if (itArc->Next())
    {
        arcAddr = itArc->Get(1);
    }
    else
    {
        SC_LOG_ERROR("Existing connector arc not found in knowledge base.");
        return action.FinishUnsuccessfully();
    }

    // Ищем уже существующую дугу отношения nrel_html_representation к найденной дуге
    ScAddr arcToArcAddr;
    ScIterator3Ptr itRel = m_context.CreateIterator3(
        HTMLTranslatorKeynodes::nrel_html_representation, 
        ScType::Unknown, 
        arcAddr);
    if (itRel->Next())
    {
        arcToArcAddr = itRel->Get(1);
    }
    else
    {
        SC_LOG_ERROR("Existing relation arc not found in knowledge base.");
        return action.FinishUnsuccessfully();
    }

    // Формируем структуру ответа (строка не изменена, как требовалось)
    ScStructure structAddr = m_context.GenerateStructure();
    structAddr << answerHTMLLink << arcAddr << rootUiElement << arcToArcAddr << HTMLTranslatorKeynodes::nrel_html_representation;
    action.SetResult(structAddr);
    return action.FinishSuccessfully();
}

ScAddr HTMLTranslatorAgent::GetActionClass() const
{
    return HTMLTranslatorKeynodes::action_translate_sc_to_html;
}
}  // namespace htmlTranslationModule
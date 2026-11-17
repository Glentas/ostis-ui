#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-memory/sc_memory.hpp"
#include <HTTPRequestHandler.hpp>

#include <sc-memory/sc_agent.hpp>

namespace htmlTranslationModule
{

void HTTPRequestHandler::RetrieveCurrentUIHandler(httplib::Request const & req, httplib::Response & res)
{
  // TODO: use user-specific current model
  ScAgentContext * context = new ScAgentContext();

  ScAddr const currentModel =
      utils::IteratorUtils::getAnyFromSet(context, HTMLTranslatorKeynodes::concept_current_ostis_ui_model);
  if (!context->IsElement(currentModel))
  {
    res.set_content("Error: current ui model is not found.", "text/html");
    return;
  }

  ScAction action = context->GenerateAction(HTMLTranslatorKeynodes::action_translate_sc_to_html);
  action.SetArguments(currentModel);
  action.InitiateAndWait();

  ScStructure translationResult = action.GetResult();
  ScAddr translationResultLink = utils::IteratorUtils::getAnyFromSet(context, translationResult);
  if (!context->IsElement(translationResultLink))
  {
    res.set_content("Error: translation result is invalid.", "text/html");
  }
  else
  {
    std::string result;
    context->GetLinkContent(translationResultLink, result);
    res.set_content(result, "text/html");
  }
}

}  // namespace htmlTranslationModule

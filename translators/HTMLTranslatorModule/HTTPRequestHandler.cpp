#include "keynodes/HTMLTranslatorKeynodes.hpp"
#include "sc-memory/sc_agent_context.hpp"
#include <HTTPRequestHandler.hpp>
#include "UILib.hpp"
#include <memory>

namespace htmlTranslationModule
{
void HTTPRequestHandler::RetrieveCurrentUIHandler(
    httplib::Request const & req,
    httplib::Response & res)
{
    SC_LOG_INFO("NEW_BINARY_LOADED: " + std::string(__TIMESTAMP__));
    
    auto context = std::make_unique<ScAgentContext>();

    try
    {
        auto result = UILib::InvokeVisualAdaptation(*context, "paragraph_rivaking", 1.5);
        SC_LOG_INFO("Visual adaptation completed successfully");
    }
    catch (std::exception const & e)
    {
        SC_LOG_ERROR("Adaptation failed: " + std::string(e.what()));
    }

    auto htmlOpt = UILib::GetHTMLForModel(
        *context,
        "translator_model",  //Идентификатор модели
        500);                               // Таймаут

    if (!htmlOpt.has_value())
    {
        SC_LOG_ERROR("Failed to retrieve HTML for current UI model");
        res.set_content("Error: failed to generate UI HTML.", "text/html");
        return;
    }
    

    res.set_content(htmlOpt.value(), "text/html");
}
}  // namespace htmlTranslationModule
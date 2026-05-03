/*
 * This source file is part of an OSTIS project...
 */
#include "agents/HTMLTranslatorAgent.hpp"
#include "HTMLTranslatorModule.hpp"
#include "agents/VisualAdaptationAgent.hpp"
#include "agents/ThemeSwitcherAgent.hpp"

using namespace htmlTranslationModule;

void HTMLTranslatorModule::Initialize(ScMemoryContext * context)
{
  SC_LOG_INFO("[ostis-ui] Initializing server...");
  
  m_server = std::make_unique<htmlTranslationModule::ServerWrapper>();
  m_server->StartServer();
  
}

void HTMLTranslatorModule::Shutdown(ScMemoryContext * context)
{
  SC_LOG_INFO("[ostis-ui] Shutting down server...");
  
  if (m_server) {
    m_server->StopServer();
  }
}

SC_MODULE_REGISTER(HTMLTranslatorModule)
    ->Agent<HTMLTranslatorAgent>()
    ->Agent<VisualAdaptationAgent>()
    ->Agent<ThemeSwitcherAgent>();
// FORCE REBUILD: 04/30/2026 09:43:03

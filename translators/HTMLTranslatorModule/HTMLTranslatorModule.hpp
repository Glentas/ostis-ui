/*
 * This source file is part of an OSTIS project...
 */
#pragma once

#include <sc-memory/sc_module.hpp>
#include <memory>
#include "HTTPServer.hpp"

class HTMLTranslatorModule : public ScModule
{
public:
  //ПРАВИЛЬНАЯ СИГНАТУРА из sc_module.cpp:
  void Initialize(ScMemoryContext * context) override;
  void Shutdown(ScMemoryContext * context) override;
  
  std::unique_ptr<htmlTranslationModule::ServerWrapper> m_server;
};
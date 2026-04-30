#pragma once

#include <sc-memory/sc_type.hpp>
#include <httplib.h>
#include <thread>
#include <atomic>

namespace htmlTranslationModule
{
class ServerWrapper
{
public:
  //Конструктор по умолчанию — память не нужна
  ServerWrapper() = default;
  ~ServerWrapper() = default;

  void Run();
  void StartServer();
  void StopServer();

protected:
  std::thread m_serverThread;
  httplib::Server m_server;
  std::atomic<sc_bool> m_isRunning{SC_FALSE};
};
}
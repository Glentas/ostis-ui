#include <string>
#include <httplib.h>
#include <HTTPRequestHandler.hpp>
#include <HTTPServer.hpp>

namespace htmlTranslationModule
{
void ServerWrapper::StartServer()
{
  m_isRunning = SC_TRUE;
  m_serverThread = std::thread(&ServerWrapper::Run, this);
}

void ServerWrapper::Run()
{
  m_server.Get(
      "/",
      [](httplib::Request const & req, httplib::Response & res) -> void
      {
        HTTPRequestHandler::RetrieveCurrentUIHandler(req, res);
      });

  SC_LOG_INFO("[ostis-ui] HTTP-server is running on port 8080");
  m_server.listen("0.0.0.0", 8080);
}

void ServerWrapper::StopServer()
{
  m_server.stop();
  m_isRunning = SC_FALSE;
  if (m_serverThread.joinable())
    m_serverThread.join();
  SC_LOG_INFO("[ostis-ui] HTTP-server is stopped");
}
}  // namespace htmlTranslationModule
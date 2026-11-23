#include <string>
#include <httplib.h>

#include <HTTPRequestHandler.hpp>
#include <HTTPServer.hpp>

namespace htmlTranslationModule
{
void ServerWrapper::StartServer()
{
  // Instantiate the server
  m_isRunning = SC_TRUE;
  m_serverThread = std::jthread(&ServerWrapper::Run, this);
}

void ServerWrapper::Run()
{
  // Define a handler for the root path
  m_server.Get(
      "/",
      [&](httplib::Request const & req, httplib::Response & res) -> void
      {
        HTTPRequestHandler::RetrieveCurrentUIHandler(req, res);
      });

  // Start the server
  SC_LOG_INFO("[ostis-ui] HTTP-server is running on port 8080");
  m_server.listen("localhost", 8080);
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

#include <sc-memory/sc_memory.hpp>
#include <httplib.h>

namespace htmlTranslationModule
{
class HTTPRequestHandler
{
public:
  static void RetrieveCurrentUIHandler(
      httplib::Request const & req, 
      httplib::Response & res);
};
}
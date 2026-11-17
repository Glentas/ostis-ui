/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "utils.hpp"
#include <filesystem>
#include <format>
#include <iostream>
namespace fs = std::filesystem;

void loadKB(ScAgentContext & context, ScsLoader & loader)
{
  auto path = fs::path(__FILE__).parent_path().parent_path();
  if (fs::is_directory(path))
  {
    for (auto const & entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied))
    {
      if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".scs")
      {
        std::cout << std::format("Found entry: {}\n", entry.path().generic_string());
        loader.loadScsFile(context, entry.path());
      }
    }
  }
}

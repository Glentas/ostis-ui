/*
 * This source file is part of an OSTIS project. For the latest info, see
 * http://ostis.net Distributed under the MIT License (See accompanying file
 * COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>

#include <sc-memory/sc_action.hpp>
#include <sc-memory/sc_event.hpp>
#include <sc-memory/sc_result.hpp>
#include <sc-memory/sc_structure.hpp>
#include <sc-memory/utils/sc_logger.hpp>

#include "keynodes/CalculatorKeynodes.hpp"
#include "CalculatorAgent.hpp"

using namespace utils;

namespace
{

class Parser
{
public:
  explicit Parser(std::string const & input)
    : s(input)
    , pos(0)
  {
  }

  double parse()
  {
    double v = parseExpr();
    if (!eof())
      throw std::runtime_error("trailing");
    return v;
  }

private:
  std::string const & s;
  size_t pos;

  void skip()
  {
    while (pos < s.size() && std::isspace((unsigned char)s[pos]))
      ++pos;
  }

  bool eof()
  {
    skip();
    return pos >= s.size();
  }

  char peek()
  {
    skip();
    return pos < s.size() ? s[pos] : '\0';
  }

  // expr := term (('+' | '-') term)*
  double parseExpr()
  {
    double v = parseTerm();
    for (;;)
    {
      char c = peek();
      if (c == '+')
      {
        ++pos;
        v += parseTerm();
      }
      else if (c == '-')
      {
        ++pos;
        v -= parseTerm();
      }
      else
        return v;
    }
  }

  // term := unary (('*' | '/') unary)*
  double parseTerm()
  {
    double v = parseUnary();
    for (;;)
    {
      char c = peek();
      if (c == '*')
      {
        ++pos;
        v *= parseUnary();
      }
      else if (c == '/')
      {
        ++pos;
        double d = parseUnary();
        if (d == 0.0)
          throw std::runtime_error("div by zero");
        v /= d;
      }
      else
        return v;
    }
  }

  // unary := ('+' | '-') unary | power
  double parseUnary()
  {
    char c = peek();
    if (c == '+')
    {
      ++pos;
      return parseUnary();
    }
    if (c == '-')
    {
      ++pos;
      return -parseUnary();
    }
    return parsePower();
  }

  // power := primary ('^' unary)?   -- правоассоциативно
  double parsePower()
  {
    double base = parsePrimary();
    if (peek() == '^')
    {
      ++pos;
      return std::pow(base, parseUnary());
    }
    return base;
  }

  // primary := digit+ | '(' expr ')'
  double parsePrimary()
  {
    char c = peek();
    if (c == '(')
    {
      ++pos;
      double v = parseExpr();
      if (peek() != ')')
        throw std::runtime_error("expected )");
      ++pos;
      return v;
    }
    if (std::isdigit((unsigned char)c))
    {
      double v = 0;
      while (pos < s.size() && std::isdigit((unsigned char)s[pos]))
      {
        v = v * 10 + (s[pos] - '0');
        ++pos;
      }
      return v;
    }
    throw std::runtime_error("unexpected char");
  }
};

std::string format(double v)
{
  if (std::isnan(v) || std::isinf(v))
    return "Error";
  std::string out = std::to_string(v);
  size_t dot = out.find('.');
  if (dot != std::string::npos)
  {
    size_t last = out.find_last_not_of('0');
    if (last == dot)
      last = dot - 1;  // оставить хотя бы одну цифру до точки
    out.erase(last + 1);
  }
  if (out == "-0")
    out = "0";
  return out;
}

}  // namespace

std::string calc(std::string const & input)
{
  try
  {
    Parser p(input);
    return format(p.parse());
  }
  catch (...)
  {
    return "Error";
  }
}

namespace calculatorModule
{
ScResult CalculatorAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto const [inputLink] = action.GetArguments<1>();
  if (!inputLink.IsValid())
  {
    SC_LOG_ERROR("Argument rrel_1 (input link) is invalid.");
    return action.FinishUnsuccessfully();
  }

  std::string content;
  if (!m_context.GetLinkContent(inputLink, content))
  {
    SC_LOG_ERROR("Failed to read content of rrel_1 link.");
    return action.FinishUnsuccessfully();
  }
  content = calc(content);
  auto answerHTMLLink = m_context.GenerateLink();
  m_context.SetLinkContent(answerHTMLLink, content);

  ScStructure structAddr = m_context.GenerateStructure();
  structAddr << answerHTMLLink;
  action.SetResult(structAddr);
  return action.FinishSuccessfully();
}

ScAddr CalculatorAgent::GetActionClass() const
{
  return CalculatorKeynodes::action_calculate_or_append;
}

}  // namespace calculatorModule

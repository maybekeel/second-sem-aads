#include "Funcs.h"
#include <iostream>
#include <string>
#include <limits>
#include "Queue.h"
#include "Stack.h"
#include "ExpressionPart.h"

namespace
{
  long long sum(long long a, long long b)
  {
    if (a > 0 && b > 0)
    {
      const long long max_int = std::numeric_limits< long long >::max();
      if (max_int - a < b)
      {
        throw std::overflow_error("Overflow");
      }
    }
    if (a < 0 && b < 0)
    {
      const long long min_int = std::numeric_limits< long long >::min();
      if (min_int - a > b)
      {
        throw std::overflow_error("Overflow");
      }
    }
    return a + b;
  }
  long long mult(long long a, long long b)
  {
    if ((a > 0 && b > 0) || (a < 0 && b < 0))
    {
      const long long max_int = std::numeric_limits< long long >::max();
      if (a > max_int / b)
      {
        throw std::overflow_error("Overflow");
      }
    }
    else if (a < 0 && b > 0)
    {
      const long long min_int = std::numeric_limits< long long >::min();
      if (a < min_int / b)
      {
        throw std::overflow_error("Overflow");
      }
    }
    else if (a > 0 && b < 0)
    {
      const long long min_int = std::numeric_limits< long long >::min();
      if (b < min_int / a)
      {
        throw std::overflow_error("Overflow");
      }
    }
    return a * b;
  }

  bool isNumber(std::string s)
  {
    bool isNumber = true;
    for (size_t i = 0; i < s.size() && isNumber; i++)
    {
      isNumber = isNumber && std::isdigit(s.at(i));
    }
    return isNumber;
  }

  bool comparator (ivlicheva::ExpressionPart op1, ivlicheva::ExpressionPart op2)
  {
    bool isFirst = op1 == ivlicheva::ExpressionPart('*') || op1 == ivlicheva::ExpressionPart('/') || op1 == ivlicheva::ExpressionPart('%');
    bool isSecond = op2 == ivlicheva::ExpressionPart('+') || op2 == ivlicheva::ExpressionPart('-');
    return isFirst && isSecond;
  }
}

bool ivlicheva::isBrackets(char s)
{
  return s == '(' || s == ')';
}

bool ivlicheva::isMathOperation(char s)
{
  return s == '+' || s == '-' || s == '/' || s == '*' || s == '%';
}

ivlicheva::Queue< std::string > ivlicheva::readFromStream(std::istream& stream)
{
  ivlicheva::Queue< std::string > inputsQueue;
  while (!stream.eof())
  {
    if (stream.peek() && stream.eof())
    {
      break;
    }
    std::string str = "";
    std::getline(stream, str);
    if (!str.empty())
    {
      inputsQueue.push(str);
    }
  }
  return inputsQueue;
}

ivlicheva::Stack< long long > ivlicheva::convertExpressions(ivlicheva::Queue< std::string > inputsQueue)
{
  ivlicheva::Stack< long long > output;
  while (!inputsQueue.isEmpty())
  {
    std::string str = inputsQueue.getNext();
    inputsQueue.drop();
    ivlicheva::Queue< ExpressionPart > queue;
    queue = splitStringToExpression(str);
    translateFromInfixToPostfixExpression(queue);
    output.push(calculateExpression(queue));
  }
  return output;
}

void ivlicheva::outputExpressions(std::ostream& stream, ivlicheva::Stack< long long > output)
{
  while (!output.isEmpty())
  {
    stream << output.getTop();
    output.drop();
    if (!output.isEmpty())
    {
      stream << ' ';
    }
  }
  stream << '\n';
}

ivlicheva::Queue< ivlicheva::ExpressionPart > ivlicheva::translateFromInfixToPostfixExpression(Queue< ExpressionPart >& queue)
{
  Queue< ExpressionPart > queueNew;
  Stack< ExpressionPart > stack;

  while (!queue.isEmpty())
  {
    if (queue.getNext() == ExpressionPart('('))
    {
      stack.push(queue.getNext());
      queue.drop();
    }
    else if (queue.getNext() == ExpressionPart(')'))
    {
      while (stack.getTop() != ExpressionPart('('))
      {
        queueNew.push(stack.getTop());
        stack.drop();
      }
      stack.drop();
      queue.drop();
    }
    else if (queue.getNext().getType() == ExpressionPartType::OPERATION)
    {
      if (stack.isEmpty() || stack.getTop() == ExpressionPart('('))
      {
        stack.push(queue.getNext());
        queue.drop();
      }
      else
      {
        while (!stack.isEmpty() && !comparator(queue.getNext(), stack.getTop()) && stack.getTop() != ExpressionPart('('))
        {
          queueNew.push(stack.getTop());
          stack.drop();
        }
        stack.push(queue.getNext());
        queue.drop();
      }
    }
    else
    {
      queueNew.push(queue.getNext());
      queue.drop();
    }
  }
  if (!stack.isEmpty())
  {
    queueNew.push(stack.getTop());
    stack.drop();
  }
  queue = queueNew;
  return queue;
}

ivlicheva::Queue< ivlicheva::ExpressionPart > ivlicheva::splitStringToExpression(std::string& str)
{
  ivlicheva::Queue< ivlicheva::ExpressionPart > queue;
  std::string str_ = "";
  for (unsigned int i = 0; i < str.length(); i++)
  {
    if (str[i] != ' ')
    {
      if (std::isalpha(str[i]))
      {
        throw std::logic_error("The symbol is an alpha\n");
      }
      else if (isBrackets(str[i]) || isMathOperation(str[i]))
      {
        if (!str_.empty() && isNumber(str_))
        {
          queue.push(ExpressionPart(std::stoll(str_)));
          str_ = "";
        }
        queue.push(ExpressionPart(str[i]));
      }
      else if (std::isdigit(str[i]))
      {
        str_ += str[i];
      }
    }
  }
  if (!str_.empty())
  {
    if (isNumber(str_))
    {
      queue.push(ExpressionPart(std::stoll(str_)));
    }
    else
    {
      queue.push(ExpressionPart(str_[0]));
    }
  }
  return queue;
}

long long ivlicheva::calculateExpression(Queue< ExpressionPart >& queue)
{
  Stack< ExpressionPart > stack;
  while (!queue.isEmpty())
  {
    ExpressionPart cell = queue.getNext();
    queue.drop();
    long long a = 0, b = 0;
    if (cell.getType() == ivlicheva::ExpressionPartType::OPERATION)
    {
      b = stack.getTop().getOperand();
      stack.drop();
      a = stack.getTop().getOperand();
      stack.drop();
    }
    if (cell == ExpressionPart('-'))
    {
      stack.push(ExpressionPart(::sum(a, b * (-1))));
    }
    else if (cell == ExpressionPart('+'))
    {
      stack.push(ExpressionPart(::sum(a, b)));
    }
    else if (cell == ExpressionPart('*'))
    {
      stack.push(ExpressionPart(::mult(a, b)));
    }
    else if (cell == ExpressionPart('/'))
    {
      if (b == 0)
      {
        throw std::logic_error("Error. Division by zero");
      }
      stack.push(ExpressionPart(a / b));
    }
    else if (cell == ExpressionPart('%'))
    {
      long long result = 0;
      if (a < 0)
      {
        result = a + b * (std::abs(a) / b + 1);
      }
      else
      {
        result = a % b;
      }
      stack.push(ExpressionPart(result));
    }
    else
    {
      stack.push(cell);
    }
  }
  long long result = stack.getTop().getOperand();
  stack.drop();
  if (!stack.isEmpty())
  {
    throw std::logic_error("bad expression");
  }

  return result;
}

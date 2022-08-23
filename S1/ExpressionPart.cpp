#include "ExpressionPart.h"
#include <stdexcept>
#include "Funcs.h"

ivlicheva::ExpressionPart::ExpressionPart(long long x):
  l(x),
  type_(ExpressionPartType::OPERAND)
{}
ivlicheva::ExpressionPart::ExpressionPart(char x)
{
  if (isBrackets(x))
  {
    type_ = ExpressionPartType::BRACKETS;
    c = x;
  }
  else if (isMathOperation(x))
  {
    type_ = ExpressionPartType::OPERATION;
    c = x;
  }
  else
  {
    throw std::logic_error("Bad value");
  }
}
bool ivlicheva::ExpressionPart::operator==(const ExpressionPart& obj) const
{
  if (obj.type_ != type_)
  {
    return false;
  }
  if (type_ == ExpressionPartType::BRACKETS || type_ == ExpressionPartType::OPERATION)
  {
    return c == obj.c;
  }
  else
  {
    return l == obj.l;
  }
}
bool ivlicheva::ExpressionPart::operator!=(const ExpressionPart& obj) const
{
  return !(*this == obj);
}
ivlicheva::ExpressionPartType ivlicheva::ExpressionPart::getType() const
{
  return type_;
}
char ivlicheva::ExpressionPart::getBracket() const
{
  if (type_ != ExpressionPartType::BRACKETS)
  {
    throw std::logic_error("It's not brackets!");
  }
  return c;
}
long long ivlicheva::ExpressionPart::getOperand() const
{
  if (type_ != ExpressionPartType::OPERAND)
  {
    throw std::logic_error("It's not operand!");
  }
  return l;
}
char ivlicheva::ExpressionPart::getOperation() const
{
  if (type_ != ExpressionPartType::OPERATION)
  {
    throw std::logic_error("It's not operation!");
  }
  return c;
}

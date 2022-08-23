#ifndef FUNCS_H
#define FUNCS_H

#include <string>
#include "Queue.h"
#include "Stack.h"
#include "ExpressionPart.h"

namespace ivlicheva
{
  Queue< std::string > readFromStream(std::istream& stream);
  Stack< long long > convertExpressions(Queue< std::string > inputsQueue);
  void outputExpressions(std::ostream& stream, Stack< long long > output);
  Queue< ExpressionPart > translateFromInfixToPostfixExpression(Queue< ExpressionPart >& queue);
  Queue< ExpressionPart > splitStringToExpression(std::string& str);
  long long calculateExpression(Queue< ExpressionPart >& queue);
  bool isBrackets(char);
  bool isMathOperation(char);
}

#endif

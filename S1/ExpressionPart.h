#ifndef EXPRESSIONPART_H
#define EXPRESSIONPART_H

namespace ivlicheva
{
  enum class ExpressionPartType
  {
    OPERAND,
    OPERATION,
    BRACKETS
  };

  class ExpressionPart
  {
    public:
      ExpressionPart() = delete;
      explicit ExpressionPart(long long x);
      explicit ExpressionPart(char x);
      ~ExpressionPart() = default;

      bool operator==(const ExpressionPart&) const;
      bool operator!=(const ExpressionPart&) const;

      ExpressionPartType getType() const;
      char getBracket() const;
      long long getOperand() const;
      char getOperation() const;

    private:
      union
      {
        char c;
        long long l;
      };
      ExpressionPartType type_;
  };
}

#endif

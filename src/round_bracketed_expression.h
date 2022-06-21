#ifndef ROUNDBRACKETEDEXPRESSION_H
#define ROUNDBRACKETEDEXPRESSION_H

#include "bracketed_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT RoundBracketedExpression : public BracketedExpression
{
public:
  RoundBracketedExpression(ExpressionPtr son);

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;
};

} // namespace MathExpressions

#endif // ROUNDBRACKETEDEXPRESSION_H

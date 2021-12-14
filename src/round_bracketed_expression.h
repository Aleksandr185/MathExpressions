#ifndef ROUNDBRACKETEDEXPRESSION_H
#define ROUNDBRACKETEDEXPRESSION_H

#include "bracketed_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT RoundBracketedExpression : BracketedExpression
{
public:
  RoundBracketedExpression();

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;
};

} // namespace ExprDraw

#endif // ROUNDBRACKETEDEXPRESSION_H

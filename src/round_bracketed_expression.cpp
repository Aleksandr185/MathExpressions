#include "round_bracketed_expression.h"

namespace MathExpressions {

RoundBracketedExpression::RoundBracketedExpression(ExpressionPtr son)
  : BracketedExpression()
{
  setSon(son);
}

MultiplicationFlags RoundBracketedExpression::multiplicationFlags() const
{
  return MultiplicationFlags(  MultiplicationFlag::Left
                             | MultiplicationFlag::Right
                             | MultiplicationFlag::Brackets);
}

} // namespace MathExpressions

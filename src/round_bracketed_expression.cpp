#include "round_bracketed_expression.h"

namespace ExprDraw {

RoundBracketedExpression::RoundBracketedExpression()
{

}

MultiplicationFlags RoundBracketedExpression::multiplicationFlags() const
{
  return MultiplicationFlags(  MultiplicationFlag::Left
                             | MultiplicationFlag::Right
                             | MultiplicationFlag::Brackets);
}

} // namespace ExprDraw

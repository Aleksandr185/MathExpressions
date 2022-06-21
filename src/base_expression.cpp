#include "base_expression.h"

namespace MathExpressions {

BaseExpression::BaseExpression(ExpressionPtr son)
{
  setSon(son);
}


bool BaseExpression::isBracketed() const
{
  return hasSon() ? son()->hasNext()
                  : false;
}

} // namespace MathExpressions

#include "base_expression.h"

namespace MathExpressions {

BaseExpression::BaseExpression(AbstractExpression *son)
{
  setSon(son);
}


bool BaseExpression::isBracketed() const
{
  return hasSon() ? son()->hasNext()
                  : false;
}

} // namespace MathExpressions

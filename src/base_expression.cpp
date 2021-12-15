#include "base_expression.h"

namespace ExprDraw {

BaseExpression::BaseExpression()
{

}


bool BaseExpression::isBracketed() const
{
  return hasSon() ? son()->hasNext()
                  : false;
}

} // namespace ExprDraw

#include "product_expression.h"

namespace MathExpressions {

const ushort CODE_PRODUCT = 0x220f; // ∏

ProductExpression::ProductExpression(ExpressionPtr expression,
                                     ExpressionPtr lowerLimit, ExpressionPtr upperLimit)
    :GroupExpression(QChar(CODE_PRODUCT))
{
  setSon(expression);
  setFirstTwin(lowerLimit);
  setSeconsTwin(upperLimit);
}

} // namespace MathExpressions

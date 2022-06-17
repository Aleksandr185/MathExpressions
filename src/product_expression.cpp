#include "product_expression.h"

namespace MathExpressions {

const ushort CODE_PRODUCT = 0x220f; // ∏

ProductExpression::ProductExpression(AbstractExpression *expression,
                                     AbstractExpression *lowerLimit, AbstractExpression *upperLimit)
    :GroupExpression(QChar(CODE_PRODUCT))
{
  setSon(expression);
  setFirstTwin(lowerLimit);
  setSeconsTwin(upperLimit);
}

} // namespace MathExpressions

#include "summa_expression.h"

namespace MathExpressions {

const ushort CODE_SUMMA = 0x2211; // ∑

SummaExpression::SummaExpression(ExpressionPtr expression,
                                 ExpressionPtr lowerLimit, ExpressionPtr upperLimit)
    : GroupExpression(QChar(CODE_SUMMA))
{
  setSon(expression);
  setFirstTwin(lowerLimit);
  setSeconsTwin(upperLimit);
}

} // namespace MathExpressions

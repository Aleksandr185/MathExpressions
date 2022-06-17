#include "summa_expression.h"

namespace MathExpressions {

const ushort CODE_SUMMA = 0x2211; // ∑

SummaExpression::SummaExpression(AbstractExpression *expression,
                                 AbstractExpression *lowerLimit, AbstractExpression *upperLimit)
    : GroupExpression(QChar(CODE_SUMMA))
{
  setSon(expression);
  setFirstTwin(lowerLimit);
  setSeconsTwin(upperLimit);
}

} // namespace MathExpressions

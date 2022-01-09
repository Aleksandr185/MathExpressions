#include "summa_expression.h"

namespace ExprDraw {

const ushort CODE_SUMMA = 0x2211; // ∑

SummaExpression::SummaExpression()
    : GroupExpression(QChar(CODE_SUMMA))
{

}

} // namespace ExprDraw

#include "summa_expression.h"

namespace ExprDraw {

const ushort SUMMA_CODE = 0x2211;

SummaExpression::SummaExpression()
    : GroupExpression(QChar(SUMMA_CODE))
{

}

} // namespace ExprDraw

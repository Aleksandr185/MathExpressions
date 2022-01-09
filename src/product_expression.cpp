#include "product_expression.h"

namespace ExprDraw {

const ushort CODE_PRODUCT = 0x220f; // ∏

ProductExpression::ProductExpression()
    :GroupExpression(QChar(CODE_PRODUCT))
{

}

} // namespace ExprDraw

#include "product_expression.h"

namespace ExprDraw {

const ushort PRODUCT_CODE = 0x220f;

ProductExpression::ProductExpression()
    :GroupExpression(QChar(PRODUCT_CODE))
{

}

} // namespace ExprDraw

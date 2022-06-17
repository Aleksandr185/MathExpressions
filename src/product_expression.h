#ifndef PRODUCTEXPRESSION_H
#define PRODUCTEXPRESSION_H

#include "group_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT ProductExpression : public GroupExpression
{
public:
    ProductExpression(ExpressionPtr expression,
                      ExpressionPtr lowerLimit, ExpressionPtr upperLimit);
};

} // namespace MathExpressions

#endif // PRODUCTEXPRESSION_H

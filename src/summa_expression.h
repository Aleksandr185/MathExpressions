#ifndef SUMMAEXPRESSION_H
#define SUMMAEXPRESSION_H

#include "group_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT SummaExpression : public GroupExpression
{
public:
    SummaExpression(ExpressionPtr expression,
                    ExpressionPtr lowerLimit, ExpressionPtr upperLimit);
};

} // namespace MathExpressions

#endif // SUMMAEXPRESSION_H

#ifndef CIRCULATIONEXPRESSION_H
#define CIRCULATIONEXPRESSION_H

#include "group_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT CirculationExpression : public GroupExpression
{
public:
    CirculationExpression(ExpressionPtr expression,
                          ExpressionPtr lowerLimit, ExpressionPtr upperLimit);

protected:
    // GroupExpression interface
    void drawSymbol(QPainter *painter, int x, int y) const;
};

} // namespace MathExpressions

#endif // CIRCULATIONEXPRESSION_H

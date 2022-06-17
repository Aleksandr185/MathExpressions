#ifndef INTEGRALEXPRESSION_H
#define INTEGRALEXPRESSION_H

#include "group_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT IntegralExpression : public GroupExpression
{
public:
  IntegralExpression(ExpressionPtr expression,
                     ExpressionPtr lowerLimit, ExpressionPtr upperLimit,
                     int multiplicity = 0);

protected:
  // GroupExpression interface
  int calcSymbolWidth() const override;
  void drawSymbol(QPainter* painter, int x, int y) const override;

private:
  QString m_text;
};

} // namespace MathExpressions

#endif // INTEGRALEXPRESSION_H

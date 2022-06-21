#ifndef FRACTIONEXPRESSION_H
#define FRACTIONEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT FractionExpression : public AbstractBigParentExpression
{
public:
  FractionExpression(ExpressionPtr numerator, ExpressionPtr denominator);

  inline ExpressionPtr numerator() const { return son(); }
  inline ExpressionPtr denominator() const { return daughter(); }

  inline void setNumerator(ExpressionPtr p) { setSon(p); }
  inline void setDenominator(ExpressionPtr p) { setDaughter(p); }

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
};

} // namespace MathExpressions

#endif // FRACTIONEXPRESSION_H

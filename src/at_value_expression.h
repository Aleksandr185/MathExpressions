#ifndef ATVALUEEXPRESSION_H
#define ATVALUEEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT AtValueExpression : public AbstractBigParentExpression
{
public:
  AtValueExpression();
  AtValueExpression(ExpressionPtr expression, ExpressionPtr condition);

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;

protected:
  // AbstractBigParentExpression interface
  void updateDaughterFont() override;
};

} // namespace MathExpressions

#endif // ATVALUEEXPRESSION_H

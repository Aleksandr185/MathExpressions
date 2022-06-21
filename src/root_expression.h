#ifndef ROOTEXPRESSION_H
#define ROOTEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT RootExpression : public AbstractBigParentExpression
{
public:
  RootExpression();
  RootExpression(ExpressionPtr radical_expression, ExpressionPtr index = nullptr);

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;

  // AbstractBigParentExpression interface
  void updateDaughterFont() override;
};

} // namespace MathExpressions

#endif // ROOTEXPRESSION_H

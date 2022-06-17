#ifndef COMMONFUNCTIONEXPRESSION_H
#define COMMONFUNCTIONEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT CommonFunctionExpression : public AbstractBigParentExpression
{
public:
  CommonFunctionExpression();
  CommonFunctionExpression(ExpressionPtr func_name, ExpressionPtr arg);

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;
  bool isArgNeedBrackets() const override;

protected:
   // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
};

} // namespace MathExpressions

#endif // COMMONFUNCTIONEXPRESSION_H

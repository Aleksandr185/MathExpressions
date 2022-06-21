#ifndef CHAINEXPRESSION_H
#define CHAINEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT ChainExpression : public AbstractParentExpression
{
public:
  ChainExpression(ExpressionPtr son = nullptr);

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
  int calcCapDY() const override;
  void calcCapDX(int& dxLeft, int& dxRight) const override;

private:
  void calcOverAbove(int &over, int &above) const;
};

} // namespace MathExpressions

#endif // CHAINEXPRESSION_H

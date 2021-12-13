#ifndef ATVALUEEXPRESSION_H
#define ATVALUEEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT AtValueExpression : public AbstractBigParentExpression
{
public:
  AtValueExpression();

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

} // namespace ExprDraw

#endif // ATVALUEEXPRESSION_H

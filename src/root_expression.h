#ifndef ROOTEXPRESSION_H
#define ROOTEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT RootExpression : public AbstractBigParentExpression
{
public:
  RootExpression();

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

} // namespace ExprDraw

#endif // ROOTEXPRESSION_H

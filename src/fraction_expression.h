#ifndef FRACTIONEXPRESSION_H
#define FRACTIONEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT FractionExpression : public AbstractBigParentExpression
{
public:
  FractionExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
};

} // namespace ExprDraw

#endif // FRACTIONEXPRESSION_H

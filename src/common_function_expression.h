#ifndef COMMONFUNCTIONEXPRESSION_H
#define COMMONFUNCTIONEXPRESSION_H

#include "abstract_big_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT CommonFunctionExpression : public AbstractBigParentExpression
{
public:
  CommonFunctionExpression();

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

} // namespace ExprDraw

#endif // COMMONFUNCTIONEXPRESSION_H

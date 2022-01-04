#ifndef LIMITEXPRESSION_H
#define LIMITEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT LimitExpression : public AbstractParentExpression
{
public:
  LimitExpression();

  // AbstractExpression interface
  bool isArgNeedBrackets() const override;

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;

  // AbstractParentExpression interface
  void updateSonFont() override;
};

} // namespace ExprDraw

#endif // LIMITEXPRESSION_H

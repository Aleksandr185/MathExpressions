#ifndef EMPTYEXPRESSION_H
#define EMPTYEXPRESSION_H

#include "abstract_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT EmptyExpression : public AbstractExpression
{
public:
  EmptyExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcHeight() const override;
};

} // namespace ExprDraw

#endif // EMPTYEXPRESSION_H

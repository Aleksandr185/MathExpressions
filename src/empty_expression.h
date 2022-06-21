#ifndef EMPTYEXPRESSION_H
#define EMPTYEXPRESSION_H

#include "abstract_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT EmptyExpression : public AbstractExpression
{
public:
  EmptyExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcHeight() const override;
};

} // namespace MathExpressions

#endif // EMPTYEXPRESSION_H

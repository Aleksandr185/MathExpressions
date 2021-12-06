#ifndef ASTERIXEXPRESSION_H
#define ASTERIXEXPRESSION_H

#include "simple_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT AsterixExpression : public SimpleExpression
{
public:
  AsterixExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
};

} // namespace ExprDraw


#endif // ASTERIXEXPRESSION_H

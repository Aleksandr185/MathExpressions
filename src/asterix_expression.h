#ifndef ASTERIXEXPRESSION_H
#define ASTERIXEXPRESSION_H

#include "simple_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT AsterixExpression : public SimpleExpression
{
public:
  AsterixExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
};

} // namespace MathExpressions


#endif // ASTERIXEXPRESSION_H

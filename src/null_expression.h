#ifndef NULLEXPRESSION_H
#define NULLEXPRESSION_H

#include "abstract_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT NullExpression : public AbstractExpression
{
public:
  NullExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter*, int, int) const override;
};

} // namespace MathExpressions

#endif // NULLEXPRESSION_H

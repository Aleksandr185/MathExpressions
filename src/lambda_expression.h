#ifndef LAMBDAEXPRESSION_H
#define LAMBDAEXPRESSION_H

#include "character_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT LambdaExpression : public CharacterExpression
{
public:
  LambdaExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
};

} // namespace MathExpressions

#endif // LAMBDAEXPRESSION_H

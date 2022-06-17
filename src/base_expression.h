#ifndef BASEEXPRESSION_H
#define BASEEXPRESSION_H

#include "bracketed_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT BaseExpression : public BracketedExpression
{
public:
  BaseExpression(ExpressionPtr son);

protected:
  // BracketedExpression interface
  bool isBracketed() const override;
};

} // namespace MathExpressions

#endif // BASEEXPRESSION_H

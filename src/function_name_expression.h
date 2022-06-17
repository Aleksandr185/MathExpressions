#ifndef FUNCTIONNAMEEXPRESSION_H
#define FUNCTIONNAMEEXPRESSION_H

#include "simple_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT FunctionNameExpression : public SimpleExpression
{
public:
  explicit FunctionNameExpression(const QString& text);

  // AbstractExpression interface
  bool isArgNeedBrackets() const override;
};

} // namespace MathExpressions

#endif // FUNCTIONNAMEEXPRESSION_H

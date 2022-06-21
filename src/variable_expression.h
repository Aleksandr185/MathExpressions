#ifndef VARIABLEEXPRESSION_H
#define VARIABLEEXPRESSION_H

#include "simple_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT VariableExpression : public SimpleExpression
{
public:
  explicit VariableExpression(const QString& text);
  explicit VariableExpression(const QChar& ch);

  // AbstractExpression interface
  QFont font() const override;

protected:
  int calcSuperscriptX() const override;
  int calcSubscriptX() const override;
  void calcCapDX(int& dxLeft, int& dxRight) const override;
};

} // namespace MathExpressions

#endif // VARIABLEEXPRESSION_H

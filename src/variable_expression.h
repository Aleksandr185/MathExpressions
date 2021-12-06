#ifndef VARIABLEEXPRESSION_H
#define VARIABLEEXPRESSION_H

#include "simple_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT VariableExpression : public SimpleExpression
{
public:
  explicit VariableExpression(const QString& text);
  explicit VariableExpression(const QChar& ch);

  // AbstractExpression interface
  QFont font() const override;

protected:
  int calcSuperscriptX() const override;
  int calcSubscriptX() const override;
  void calcCapDX(int& DLeft, int& DRight) const override;
};

} // namespace ExprDraw

#endif // VARIABLEEXPRESSION_H

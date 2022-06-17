#ifndef PLANCKEXPRESSION_H
#define PLANCKEXPRESSION_H

#include "character_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT PlanckExpression : public CharacterExpression
{
public:
  PlanckExpression();

protected:
  // AbstractExpression interface
  int calcCapDY() const override;
  void calcCapDX(int& dxLeft, int& dxRight) const override;
  QFont font() const override;
};

} // namespace MathExpressions

#endif // PLANCKEXPRESSION_H

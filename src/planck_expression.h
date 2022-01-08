#ifndef PLANCKEXPRESSION_H
#define PLANCKEXPRESSION_H

#include "character_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT PlanckExpression : public CharacterExpression
{
public:
  PlanckExpression();

protected:
  // AbstractExpression interface
  int calcCapDY() const override;
  void calcCapDX(int& dxLeft, int& dxRight) const override;
  QFont font() const override;
};

} // namespace ExprDraw

#endif // PLANCKEXPRESSION_H

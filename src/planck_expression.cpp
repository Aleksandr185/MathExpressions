#include "planck_expression.h"

namespace MathExpressions {

const ushort CODE_PLANCK = 0x0127; // ħ

PlanckExpression::PlanckExpression()
  : CharacterExpression(QChar(CODE_PLANCK), true)
{

}

// AbstractExpression interface

int PlanckExpression::calcCapDY() const
{
  return qRound(4.0 * capMultiplier().y());
}

void PlanckExpression::calcCapDX(int& dxLeft, int& dxRight) const
{
  dxLeft = qRound(capMultiplier().x());
  dxRight = dxLeft;
}

QFont PlanckExpression::font() const
{
  QFont my_font = CharacterExpression::font();
  my_font.setItalic(true);
  return my_font;
}

} // namespace MathExpressions

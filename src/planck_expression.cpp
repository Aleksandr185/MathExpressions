#include "planck_expression.h"

namespace ExprDraw {

const ushort PLANCK_CODE = 0x0127; // ħ

PlanckExpression::PlanckExpression()
  : CharacterExpression(QChar(PLANCK_CODE))
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

// CharacterExpression interface

bool PlanckExpression::canChangeCharacter(const QChar& newCharacter) const
{
  return newCharacter.unicode() == PLANCK_CODE;
}

} // namespace ExprDraw

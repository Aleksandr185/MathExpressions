#include "comma_expression.h"

namespace ExprDraw {

const ushort COMMA_CODE = 0x002C; // ,

CommaExpression::CommaExpression()
  :CharacterExpression(QChar(COMMA_CODE))
{

}

// AbstractExpression interface

bool CommaExpression::isNeedBrackets() const
{
  return true;
}

int CommaExpression::calcCapDY() const
{
  return INT_MAX;
}

// CharacterExpression interface

bool CommaExpression::canChangeCharacter(const QChar& newCharacter) const
{
  return newCharacter.unicode() == COMMA_CODE;
}

} // namespace ExprDraw




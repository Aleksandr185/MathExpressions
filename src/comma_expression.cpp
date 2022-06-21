#include "comma_expression.h"

namespace MathExpressions {

const ushort COMMA_CODE = 0x002C; // ,

CommaExpression::CommaExpression()
  :CharacterExpression(QChar(COMMA_CODE), true)
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

} // namespace MathExpressions




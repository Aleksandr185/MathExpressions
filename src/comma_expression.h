#ifndef COMMAEXPRESSION_H
#define COMMAEXPRESSION_H

#include "character_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT CommaExpression : public CharacterExpression
{
public:
  CommaExpression();

  // AbstractExpression interface
  bool isNeedBrackets() const override;
protected:
  int calcCapDY() const override;

  // CharacterExpression interface
  bool canChangeCharacter(const QChar& newCharacter) const override;
};

} // namespace ExprDraw

#endif // COMMAEXPRESSION_H

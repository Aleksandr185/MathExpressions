#ifndef COMMAEXPRESSION_H
#define COMMAEXPRESSION_H

#include "character_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT CommaExpression : public CharacterExpression
{
public:
  CommaExpression();

  // AbstractExpression interface
  bool isNeedBrackets() const override;
protected:
  int calcCapDY() const override;

};

} // namespace MathExpressions

#endif // COMMAEXPRESSION_H

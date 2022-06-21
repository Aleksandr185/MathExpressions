#ifndef NABLAEXPRESSION_H
#define NABLAEXPRESSION_H

#include "character_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT NablaExpression : public CharacterExpression
{
public:
  Q_DECL_DEPRECATED_X("Class NablaExpression is deprecated! Use CharacterExpression(0x2207)!")
  NablaExpression();
};

} // namespace MathExpressions

#endif // NABLAEXPRESSION_H

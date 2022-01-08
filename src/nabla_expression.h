#ifndef NABLAEXPRESSION_H
#define NABLAEXPRESSION_H

#include "character_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT NablaExpression : public CharacterExpression
{
public:
  Q_DECL_DEPRECATED_X("Class NablaExpression is deprecated! Use CharacterExpression(0x2207)!")
  NablaExpression();
};

} // namespace ExprDraw

#endif // NABLAEXPRESSION_H

#ifndef BASEEXPRESSION_H
#define BASEEXPRESSION_H

#include "bracketed_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT BaseExpression : public BracketedExpression
{
public:
  BaseExpression();

protected:
  // BracketedExpression interface
  bool isBracketed() const override;
};

} // namespace ExprDraw

#endif // BASEEXPRESSION_H

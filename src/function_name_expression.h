#ifndef FUNCTIONNAMEEXPRESSION_H
#define FUNCTIONNAMEEXPRESSION_H

#include "simple_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT FunctionNameExpression : public SimpleExpression
{
public:
  explicit FunctionNameExpression(const QString& text);

  // AbstractExpression interface
  bool isArgNeedBrackets() const override;
};

} // namespace ExprDraw

#endif // FUNCTIONNAMEEXPRESSION_H

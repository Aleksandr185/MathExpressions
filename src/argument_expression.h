#ifndef ARGUMENTEXPRESSION_H
#define ARGUMENTEXPRESSION_H

#include "bracketed_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT ArgumentExpression : public BracketedExpression
{
public:
  ArgumentExpression(ExpressionPtr son);

  inline bool forsedBrackets() const { return m_forced_brackets; }
  void setForsedBrackets(bool value);
protected:
  // BracketedExpression interface
  bool isBracketed() const override;

private:
  bool m_forced_brackets;
};

} // namespace MathExpressions

#endif // ARGUMENTEXPRESSION_H

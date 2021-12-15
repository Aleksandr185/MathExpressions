#ifndef ARGUMENTEXPRESSION_H
#define ARGUMENTEXPRESSION_H

#include "bracketed_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT ArgumentExpression : public BracketedExpression
{
public:
  ArgumentExpression();

  inline bool forsedBrackets() const { return m_forced_brackets; }
  void setForsedBrackets(bool value);
protected:
  // BracketedExpression interface
  bool isBracketed() const override;

private:
  bool m_forced_brackets;
};

} // namespace ExprDraw

#endif // ARGUMENTEXPRESSION_H

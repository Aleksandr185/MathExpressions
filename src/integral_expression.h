#ifndef INTEGRALEXPRESSION_H
#define INTEGRALEXPRESSION_H

#include "group_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT IntegralExpression : public GroupExpression
{
public:
  IntegralExpression(int multiplicity = 0);

protected:
  // GroupExpression interface
  int calcSymbolWidth() const override;
  void drawSymbol(QPainter* painter, int x, int y) const override;

private:
  QString m_text;
};

} // namespace ExprDraw

#endif // INTEGRALEXPRESSION_H

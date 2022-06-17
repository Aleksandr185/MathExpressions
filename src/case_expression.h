#ifndef CASEEXPRESSION_H
#define CASEEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT CaseExpression : public AbstractParentExpression
{
public:
  CaseExpression(ExpressionPtr expression);

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;

private:
  QFont bracketFont() const;
};

} // namespace MathExpressions

#endif // CASEEXPRESSION_H

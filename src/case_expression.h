#ifndef CASEEXPRESSION_H
#define CASEEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT CaseExpression : public AbstractParentExpression
{
public:
  CaseExpression();

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;

private:
  QFont bracketFont() const;
};

} // namespace ExprDraw

#endif // CASEEXPRESSION_H

#ifndef STANDEXPRESSION_H
#define STANDEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT StandExpression : public AbstractParentExpression
{
public:
  StandExpression(ExpressionPtr expression, Qt::Alignment horizontal_alignment = Qt::AlignLeft);

  void setHorizontalAlignment(Qt::Alignment horizontal_alignment);
  inline Qt::Alignment horizontalAlignment() const { return m_horizontal_alignment; }

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;

private:
  Qt::Alignment m_horizontal_alignment;
};

} // namespace MathExpressions

#endif // STANDEXPRESSION_H

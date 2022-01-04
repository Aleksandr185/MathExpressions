#ifndef STANDEXPRESSION_H
#define STANDEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT StandExpression : public AbstractParentExpression
{
public:
  StandExpression(HorizontalAlignment _hor_align = HorizontalAlignment::Left);

  void setHorizontalAlignment(HorizontalAlignment _hor_align);
  inline HorizontalAlignment horizontalAlignment() const { return m_horizontal_alignment; }

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;

private:
  HorizontalAlignment m_horizontal_alignment;
};

} // namespace ExprDraw

#endif // STANDEXPRESSION_H

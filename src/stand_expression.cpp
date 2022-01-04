#include "stand_expression.h"

namespace ExprDraw {

StandExpression::StandExpression(HorizontalAlignment _hor_align)
 : m_horizontal_alignment(_hor_align)
{

}

void StandExpression::setHorizontalAlignment(HorizontalAlignment _hor_align)
{
  m_horizontal_alignment = _hor_align;
}

// AbstractExpression interface

void StandExpression::paint(QPainter* painter, int x, int y) const
{
  if ( hasSon() ) {
    HorizontalAlignment hor_align = m_horizontal_alignment;

    if (m_horizontal_alignment == HorizontalAlignment::Right) {
      x += width();
      hor_align = HorizontalAlignment::Left;
    }
    else if (m_horizontal_alignment == HorizontalAlignment::Center) {
      x += width() / 2;
    }
    else if (m_horizontal_alignment == HorizontalAlignment::Left) {
      hor_align = HorizontalAlignment::Right;
    }

    AbstractExpression* next_expr = son();
    while ( next_expr ) {
      next_expr->draw(painter, x, y, hor_align, VerticalAlignment::Bottom);

      y += next_expr->height();
      next_expr = next_expr->next();
    }
  }
}

int StandExpression::calcWidth() const
{
  int result = 0;

  if ( hasSon() ) {
    result = son()->width();

    AbstractExpression* next_expr = son();
    while ( next_expr->hasNext() ) {
      next_expr = next_expr->next();
      result = qMax(result, next_expr->width());
    }
  }

  return result;
}

int StandExpression::calcHeight() const
{
  int result = 0;

  if ( hasSon() ) {
    result = son()->height();

    AbstractExpression* next_expr = son();
    while ( next_expr->hasNext() ) {
      next_expr = next_expr->next();
      result += next_expr->height();
    }
  }

  return result;
}

} // namespace ExprDraw

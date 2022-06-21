#include "stand_expression.h"

namespace MathExpressions {

StandExpression::StandExpression(ExpressionPtr expression, Qt::Alignment horizontal_alignment)
{
  setHorizontalAlignment(horizontal_alignment);
  setSon(expression);
}

void StandExpression::setHorizontalAlignment(Qt::Alignment horizontal_alignment)
{
  m_horizontal_alignment = horizontal_alignment & Qt::AlignHorizontal_Mask;
}

// AbstractExpression interface

void StandExpression::paint(QPainter* painter, int x, int y) const
{
  if ( hasSon() ) {
    Qt::Alignment alignment = Qt::AlignBottom;

    if ( m_horizontal_alignment.testFlag(Qt::AlignRight) ) {
      x += width();
      alignment.setFlag(Qt::AlignLeft);
    }
    else if ( m_horizontal_alignment.testFlag(Qt::AlignHCenter) ) {
      x += width() / 2;
      alignment.setFlag(Qt::AlignHCenter);
    }
    else if ( m_horizontal_alignment.testFlag(Qt::AlignLeft) ) {
      alignment.setFlag(Qt::AlignRight);
    }

    ExpressionPtr next_expr = son();
    while ( next_expr ) {
      next_expr->draw(painter, x, y, alignment);

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

    ExpressionPtr next_expr = son();
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

    ExpressionPtr next_expr = son();
    while ( next_expr->hasNext() ) {
      next_expr = next_expr->next();
      result += next_expr->height();
    }
  }

  return result;
}

} // namespace MathExpressions

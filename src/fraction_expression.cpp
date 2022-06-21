#include "fraction_expression.h"

#include <QPainter>

namespace MathExpressions {

FractionExpression::FractionExpression(ExpressionPtr numerator, ExpressionPtr denominator)
{
  setSon(numerator);
  setDaughter(denominator);
}

// AbstractExpression interface

void FractionExpression::paint(QPainter* painter, int x, int y) const
{
  const int center_y = y + ascent();
  const int center_x = x + width() / 2;

  const int line_x = lineWidth().x();
  const int line_y = lineWidth().y();
  const int half_line_y = line_y / 2;

  if ( hasSon() ){
    son()->draw(painter,
                center_x, center_y - line_y - half_line_y,
                Qt::AlignHCenter | Qt::AlignTop);
  }

  if ( hasDaughter() ) {
    daughter()->draw(painter,
                     center_x, center_y + line_y + half_line_y,
                     Qt::AlignHCenter | Qt::AlignBottom);
  }

  // draw line
  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  const int pen_width = painter->pen().width();
  QRectF rect;
  rect.setLeft(x + 3 * line_x);
  rect.setRight(x + width() - 3 * line_x + 1 - pen_width);
  rect.setTop(center_y - half_line_y);
  rect.setBottom(center_y + half_line_y + (line_y & 1) - pen_width);

  painter->setRenderHint(QPainter::Antialiasing);
  painter->drawRect(rect);

  painter->restore();
}

int FractionExpression::calcWidth() const
{
  const int son_width = hasSon() ? son()->width() : 0;
  const int daughter_width = hasDaughter() ? daughter()->width() : 0;
  return 8 * lineWidth().x() + qMax(son_width, daughter_width);
}

int FractionExpression::calcHeight() const
{
  const int son_height = hasSon() ? son()->height() : 0;
  const int daughter_height = hasDaughter() ? daughter()->height() : 0;
  return 3 * lineWidth().y() + son_height + daughter_height;
}

int FractionExpression::calcAscent() const
{
  const int son_height = hasSon() ? son()->height() : 0;
  return son_height + lineWidth().y() + (lineWidth().y() / 2);
}

int FractionExpression::calcDescent() const
{
  const int daughter_height = hasDaughter() ? daughter()->height() : 0;
  return daughter_height - lineWidth().y() - (lineWidth().y() / 2);
}

} // namespace MathExpressions

#include "at_value_expression.h"

#include <QPainter>

namespace MathExpressions {

const int SPACE = 3;

AtValueExpression::AtValueExpression()
{

}

AtValueExpression::AtValueExpression(ExpressionPtr expression, ExpressionPtr condition)
{
  setSon(expression);
  setDaughter(condition);
}

// AbstractExpression interface

MultiplicationFlags AtValueExpression::multiplicationFlags() const
{
  MultiplicationFlags result;

  if (hasSon() && son()->multiplicationFlags().testFlag(MultiplicationFlag::Left)) {
    result.setFlag(MultiplicationFlag::Left);
  }

  return result;
}

void AtValueExpression::paint(QPainter* painter, int x, int y) const
{
  const int son_height = hasSon() ? son()->height() : 0;
  const int son_width = hasSon() ? son()->width() : 0;
  const int daughter_height = hasDaughter() ? daughter()->height() : 0;
  const int dh = qMax(0, daughter_height - son_height);

  if (hasSon()) {
    son()->draw(painter, x, y + dh, Qt::AlignRight | Qt::AlignBottom) ;
  }

  const int H = qMax(son_height, daughter_height);
  const int W = x + son_width;

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  const int pen_width = painter->pen().width();

  QRectF rec;
  rec.setLeft(W);
  rec.setTop(y);
  rec.setRight(W + lineWidth().x() - pen_width);
  rec.setBottom(y + H - pen_width);
  painter->drawRect(rec);
  painter->restore();

  if (hasDaughter()) {
    daughter()->draw(painter, W + (SPACE * lineWidth().x()), y + H, Qt::AlignRight | Qt::AlignTop);
  }
}

int AtValueExpression::calcWidth() const
{
  const int son_width = hasSon() ? son()->width() : 0;
  const int daughter_width = hasDaughter() ? daughter()->width() : 0;

  return son_width + (SPACE * lineWidth().x()) + daughter_width;
}

int AtValueExpression::calcHeight() const
{
  const int son_height = hasSon() ? son()->height() : 0;
  const int daughter_height = hasDaughter() ? daughter()->height() : 0;

  return qMax(son_height, daughter_height);
}

int AtValueExpression::calcAscent() const
{
  const int son_height = hasSon() ? son()->height() : 0;
  const int son_ascent = hasSon() ? son()->ascent() : 0;
  const int daughter_height = hasDaughter() ? daughter()->height() : 0;

  return son_ascent + qMax(0, daughter_height - son_height);
}

int AtValueExpression::calcDescent() const
{
  if (hasSon()) {
    return son()->descent();
  }
  else {
    return AbstractBigParentExpression::descent();
  }
}

// AbstractBigParentExpression interface

void AtValueExpression::updateDaughterFont()
{
  const double FONT_FACTOR = 0.7;

  QFont small_font(font());
  small_font.setPointSizeF(FONT_FACTOR * small_font.pointSizeF());
  daughter()->setFont(small_font);
}

} // namespace MathExpressions

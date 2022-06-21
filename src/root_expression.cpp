#include "root_expression.h"

#include <QPainter>

namespace MathExpressions {

RootExpression::RootExpression()
{

}

RootExpression::RootExpression(ExpressionPtr radical_expression, ExpressionPtr index)
{
  setSon(radical_expression);
  setDaughter(index);
}

// AbstractExpression interface

void RootExpression::paint(QPainter* painter, int x, int y) const
{
  if (hasSon()) {
    const int son_height = son()->height();
    const int son_width  = son()->width();

    const int line_x = lineWidth().x();
    const int line_y = lineWidth().y();

    const int radical_height = son_height + 3 * line_y;
    const int radical_width  = son_width + qRound( (son_height + line_y) / 2.0) + 8 * line_x;

    QPolygonF pl;
    pl << QPointF(x + line_x, y + 6 * line_y);                                                // 0
    pl << QPointF(x + 6 * line_x - 1, pl.at(0).y());                                          // 1
    pl << QPointF(pl.constLast().x(), y + qRound(radical_height - 3 * sqrt(3) * line_y) );    // 2
    pl << QPointF(x + radical_width - 3 * line_x - son_width - (line_x / 2.0) - 1, y + line_y); // 3
    pl << QPointF(x + radical_width - line_x - 1, pl.constLast().y());                        // 4
    pl << QPointF(pl.constLast().x(), y + 4 * line_y - 1);                                    // 5
    pl << QPointF(x + radical_width - 2 * line_x, pl.constLast().y());                        // 6
    pl << QPointF(pl.constLast().x(), y + 2 * line_y - 1);                                    // 7
    pl << QPointF(x + radical_width - 3 * line_x - son_width - 1, pl.constLast().y());        // 8
    pl << QPointF(x + 4 * line_x, y + radical_height);                                        // 9
    pl << QPointF(pl.constLast().x(), y + 7 * line_y - 1);                                    // 10
    pl << QPointF(pl.constFirst().x(), pl.constLast().y());                                   // 11
    pl << pl.constFirst();

    if (hasDaughter()) {
      const int dx = qMax(0, daughter()->width()  - 5 * line_x);
      const int dy = qMax(0, daughter()->height() - 4 * line_y);

      for (int i = 0; i < pl.size(); ++i) {
        pl[i].setX(pl.at(i).x() + dx);
        pl[i].setY(pl.at(i).y() + dy);
      }

      daughter()->draw(painter,
                       pl.at(1).x(),
                       pl.at(1).y(),
                       Qt::AlignLeft | Qt::AlignTop);
    }

    son()->draw(painter,
                pl.at(8).x(),
                pl.at(8).y(),
                Qt::AlignRight | Qt::AlignBottom);

    painter->save();
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawConvexPolygon(pl);
    painter->restore();
  }
}

int RootExpression::calcWidth() const
{
  int result = 8 * lineWidth().x();

  if (hasSon()) {
    result += son()->width() + qRound((son()->height() + lineWidth().y()) / 2.0);
  }
  if (hasDaughter()) {
    result += qMax(0, daughter()->width() - 5 * lineWidth().x());
  }

  return result;
}

int RootExpression::calcHeight() const
{
  int result = 3 * lineWidth().y();

  if (hasSon()) {
    result += son()->height();
  }
  if (hasDaughter()) {
    result += qMax(0, daughter()->height() - 4 * lineWidth().x());
  }

  return result;
}

int RootExpression::calcAscent() const
{
  if (hasSon()) {
    int result = son()->ascent();

    if ( hasDaughter() && daughter()->height() > (4 * lineWidth().y()) ) {
      result = result - lineWidth().y() + daughter()->height();
    }
    else {
      result = result + (3 * lineWidth().y());
    }

    return result;
  }
  else {
    return AbstractBigParentExpression::calcAscent();
  }
}

int RootExpression::calcDescent() const
{
  if (hasSon()) {
    return son()->descent();
  }
  else {
    return AbstractBigParentExpression::calcDescent();
  }
}

// AbstractBigParentExpression interface

void RootExpression::updateDaughterFont()
{
  const double FONT_FACTOR = 0.7;

  QFont fn = font();
  fn.setPointSizeF( qRound(FONT_FACTOR * fn.pointSizeF()) );
  daughter()->setFont(fn);
}

} // namespace MathExpressions

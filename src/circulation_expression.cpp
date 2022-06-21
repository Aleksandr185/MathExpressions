#include "circulation_expression.h"

#include <QPainter>
#include <QFontMetrics>
#include <QPaintDevice>

namespace MathExpressions {

const ushort CODE_INTEGRAL = 0x222b;  // ∫

CirculationExpression::CirculationExpression(ExpressionPtr expression,
                                             ExpressionPtr lowerLimit,
                                             ExpressionPtr upperLimit)
    : GroupExpression(QChar(CODE_INTEGRAL))
{
  setSon(expression);
  setFirstTwin(lowerLimit);
  setSeconsTwin(upperLimit);
}

void CirculationExpression::drawSymbol(QPainter *painter, int x, int y) const
{
  // x,y - is a center point of symbol
  GroupExpression::drawSymbol(painter, x, y);

  const int dx = QFontMetrics(symbolFont()).boundingRect(symbol()).width() / 2;
  const double factorDpi = paintDevice() ? paintDevice()->logicalDpiX() / paintDevice()->logicalDpiY()
                                         : 1.0;
  const int dy = qRound(dx * factorDpi);

  painter->save();
  QPen _pen = pen();
  _pen.setWidth(lineWidth().x());
  painter->setPen(_pen);
  painter->setBrush(QBrush(Qt::BrushStyle::NoBrush));
  painter->setRenderHint(QPainter::Antialiasing);

  const int half_pen_width = qRound(painter->pen().widthF() / 2.0);
  x += half_pen_width;
  y += half_pen_width;

  QRect rect;
  rect.setLeft(x - dx);
  rect.setRight(x + dx - lineWidth().x());
  rect.setTop(y - dy);
  rect.setBottom(y + dy - lineWidth().y());

  painter->drawEllipse(rect);

  painter->restore();
}

} // namespace MathExpressions

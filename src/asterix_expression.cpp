#include "asterix_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace ExprDraw {

AsterixExpression::AsterixExpression()
  : SimpleExpression('*')
{

}

// AbstractExpression interface

void AsterixExpression::paint(QPainter* painter, int x, int y) const
{
  const QFont my_font = font();
  QFontMetrics fm(my_font);
  QPoint pos(x, y + qRound(8 * rLineWidthY()) + fm.ascent());

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->setFont(my_font);
  painter->drawText(pos, text());
  painter->restore();
}

} // namespace ExprDraw



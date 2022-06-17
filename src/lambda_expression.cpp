#include "lambda_expression.h"

#include <QPainter>

namespace MathExpressions {

const ushort CODE_LAMBDA = 0x03bb; // λ

LambdaExpression::LambdaExpression()
  : CharacterExpression(QChar(CODE_LAMBDA), true)
{

}

// AbstractExpression interface

void LambdaExpression::paint(QPainter* painter, int x, int y) const
{
  CharacterExpression::paint(painter, x, y);

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());

  const int dy = y + qRound(9.0 * capMultiplier().y());
  const int pen_width = painter->pen().width();

  QRectF rect;
  rect.setLeft(x + qRound(1.5 * capMultiplier().x()));
  rect.setRight(x + width() - qRound(2.0 * capMultiplier().x() - pen_width));
  rect.setTop(dy);
  rect.setBottom(dy + capMultiplier().y() - pen_width);
  painter->drawRect(rect);

  painter->restore();
}

} // namespace MathExpressions

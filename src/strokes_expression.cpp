#include "strokes_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace MathExpressions {

const ushort CODE_STROKE = 0x00B4;  // ´
const QChar STROKE_CHAR  = QChar(CODE_STROKE);

const int STROKE_SPACE = 2;  // * lineWidth().x()

StrokesExpression::StrokesExpression(int count)
  : m_count(1)
{
  setCount(count);
}

void StrokesExpression::setCount(int count)
{
  if (count < 1)
    count = 1;

  if (m_count != count) {
    m_count = count;
    setFlag(CalculateFlag::Width);
  }
}

// AbstractExpression interface

void StrokesExpression::paint(QPainter* painter, int x, int y) const
{
  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());

  const QFont fn = enlargedFont(font());
  const QRect stroke_rect = QFontMetrics(fn).boundingRect(STROKE_CHAR);
  const int space = lineWidth().x() * STROKE_SPACE;

  QPoint pos;
  pos.setX(x - stroke_rect.left() + space);
  pos.setY(y - stroke_rect.top() + qRound(2 * capMultiplier().y()));

  painter->setFont(fn);
  const QString draw_text(STROKE_CHAR);

  for (int i = 0; i < m_count; ++i) {
    painter->drawText(pos.x(), pos.y(), draw_text);
    pos.setX(pos.x() + stroke_rect.width() + space);
  }

  painter->restore();
}

int StrokesExpression::calcWidth() const
{
  const QFont fn = enlargedFont(font());
  const int stroke_width = QFontMetrics(fn).boundingRect(STROKE_CHAR).width();
  const int space = lineWidth().x() * STROKE_SPACE;

  return ((stroke_width + space) * m_count) + space;
}

int StrokesExpression::calcHeight() const
{
  return qRound(0.6 * QFontMetrics(font()).height());
}

// private

QFont StrokesExpression::enlargedFont(const QFont& font) const
{
  const double FONT_FACTOR = 1.4;

  QFont result = font;
  result.setPointSizeF(FONT_FACTOR * result.pointSizeF());
  return result;
}

} // namespace MathExpressions

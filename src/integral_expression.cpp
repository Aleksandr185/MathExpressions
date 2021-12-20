#include "integral_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace ExprDraw {

const ushort INTEGRAL_CODE = 0x222b;

IntegralExpression::IntegralExpression(int multiplicity /* = 0 */)
  : GroupExpression(QChar(INTEGRAL_CODE))
{
  if (multiplicity <= 0) {
    m_text.append(symbol());
    m_text.append(symbol());
    m_text.append(QChar(Symbol::Ellipsis));
    m_text.append(symbol());
  }
  else {
    for (int i = 0; i < multiplicity; ++i) {
      m_text.append(symbol());
    }
  }
}

// GroupExpression interface

int IntegralExpression::calcSymbolWidth() const
{
  return QFontMetrics(symbolFont()).horizontalAdvance(m_text);
}

void IntegralExpression::drawSymbol(QPainter* painter, int x, int y) const
{
  // x,y - is a center point of symbol

  const QFont symbol_font = symbolFont();
  QFontMetrics fm(symbol_font);

  const QRect rec = fm.boundingRect(symbol());
  const int dy = rec.top() + rec.height() / 2;
  const int dx = fm.horizontalAdvance(m_text) / 2;

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->setFont(symbol_font);
  painter->translate(x - dx, y - dy);
  painter->drawText(0, 0, m_text);
  painter->restore();
}

} // namespace ExprDraw

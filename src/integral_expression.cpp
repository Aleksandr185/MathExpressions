#include "integral_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace MathExpressions {

const ushort CODE_INTEGRAL = 0x222b;  // ∫
const ushort CODE_ELLIPSIS = 0x2026;  // …

IntegralExpression::IntegralExpression(ExpressionPtr expression,
                                       ExpressionPtr lowerLimit,
                                       ExpressionPtr upperLimit,
                                       int multiplicity /* = 0 */)
  : GroupExpression(QChar(CODE_INTEGRAL))
{
  if (multiplicity <= 0) {
    m_text.append(symbol());
    m_text.append(symbol());
    m_text.append(QChar(CODE_ELLIPSIS));
    m_text.append(symbol());
  }
  else {
    for (int i = 0; i < multiplicity; ++i) {
      m_text.append(symbol());
    }
  }

  setSon(expression);
  setFirstTwin(lowerLimit);
  setSeconsTwin(upperLimit);
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

} // namespace MathExpressions

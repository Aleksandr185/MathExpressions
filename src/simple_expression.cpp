#include "simple_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace MathExpressions {


SimpleExpression::SimpleExpression(const QString &text)
  : m_text(text)
{

}

SimpleExpression::SimpleExpression(const QChar &ch)
  :m_text(ch)
{

}

// protected
// AbstractExpression interface

void SimpleExpression::paint(QPainter* painter, int x, int y) const
{
  if (!m_text.isEmpty()) {
    const QFont my_font = font();
    QFontMetrics fm(my_font);
    const int leftBearing = fm.leftBearing(m_text.front());
    if (leftBearing < 0) {
      x -= leftBearing;
    }

    QPoint pos(x, y + fm.ascent());

    painter->save();
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setFont(my_font);
    painter->drawText(pos, m_text);
    painter->restore();
  }
}

int SimpleExpression::calcWidth() const
{
  QFontMetrics fm(font());
  int width = fm.horizontalAdvance(m_text);

  if (!m_text.isEmpty()) {
    const int leftBearing = fm.leftBearing(m_text.front());
    if (leftBearing < 0) {
      width -= leftBearing;
    }

    const int rightBearing = fm.rightBearing(m_text.back());
    if (rightBearing < 0) {
      width -= rightBearing;
    }
  }

  return width;
}

int SimpleExpression::calcHeight() const
{
  return QFontMetrics(font()).height();
}

int SimpleExpression::calcCapDY() const
{
  // TODO choose DY depending on the height of the symbol, and not depending on the symbol.

  const ushort CYRILLIC_SMALL_LETTER_A      = 0x430; // 'а'
  const ushort CYRILLIC_SMALL_LETTER_I      = 0x438; // 'и'
  const ushort CYRILLIC_SMALL_LETTER_ShortI = 0x439; // 'й'
  const ushort CYRILLIC_SMALL_LETTER_Ka     = 0x43A; // 'к'
  const ushort CYRILLIC_SMALL_LETTER_Ya     = 0x44F; // 'я'
  const ushort CYRILLIC_SMALL_LETTER_Lo     = 0x451; // 'ё'

  const ushort CYRILLIC_CAPITAL_LETTER_A      = 0x410; // 'А'
  const ushort CYRILLIC_CAPITAL_LETTER_I      = 0x418; // 'И'
  const ushort CYRILLIC_CAPITAL_LETTER_ShortI = 0x419; // 'Й'
  const ushort CYRILLIC_CAPITAL_LETTER_Ka     = 0x41A; // 'К'
  const ushort CYRILLIC_CAPITAL_LETTER_Ya     = 0x42F; // 'Я'
  const ushort CYRILLIC_CAPITAL_LETTER_Lo     = 0x401; // 'Ё'

  double DY = 0.0;
  const ushort code = m_text.isEmpty() ? 0 : m_text.at(0).unicode();

  switch (code) {
  case 'a':
  case 'c':
  case 'e':
  case 'g':
    DY = 9.5;
    break;

  case 'b':
  case 'd':
  case 'f':
  case 'h':
  case 'k':
  case 'l':
    DY = 4.0;
    break;

  case 'i':
  case 'j':
  case 't':
  case CYRILLIC_SMALL_LETTER_ShortI:
  case CYRILLIC_SMALL_LETTER_Lo:
    DY = 7.0;
    break;

  case CYRILLIC_CAPITAL_LETTER_Lo:
  case CYRILLIC_CAPITAL_LETTER_ShortI:
    DY = 2.5;
    break;

  }

  if (DY == 0.0) {
    if ( ((code >= 'A') && (code <= 'Z'))
         || ((code >= CYRILLIC_CAPITAL_LETTER_A) && (code <= CYRILLIC_CAPITAL_LETTER_I))
         || ((code >= CYRILLIC_CAPITAL_LETTER_Ka) && (code <= CYRILLIC_CAPITAL_LETTER_Ya)) ) {
      DY = 4.0;
    }
    else
      if ( ((code >= 'm') && (code <= 's')) || ((code >= 'u') && (code <= 'z'))
           || ((code >= CYRILLIC_SMALL_LETTER_A) && (code <= CYRILLIC_SMALL_LETTER_I))
           || ((code >= CYRILLIC_SMALL_LETTER_Ka) && (code <= CYRILLIC_SMALL_LETTER_Ya)) ) {
        DY = 9.5;
      }
  }

  return qRound(DY * capMultiplier().x());
}

} // namespace MathExpressions

#include "number_expresssion.h"

#include <QLocale>
#include <QFontMetrics>
#include <QPainter>

namespace ExprDraw {

NumberExpresssion::NumberExpresssion()
  : m_number(0.0), m_style(Style::Regular)
{

}

// public

void NumberExpresssion::setNumber(double number)
{
  if ( m_number != number) {
    m_number = number;
    update();
  }
}

void NumberExpresssion::setStyle(Style style)
{
  if (m_style != style) {
    m_style = style;
    update();
  }
}

// AbstractExpression interface

MultiplicationFlags NumberExpresssion::multiplicationFlags() const
{
  MultiplicationFlags result(MultiplicationFlag::Right);
  result.setFlag(MultiplicationFlag::Negative, m_number < 0.0);
  return result;
}

void NumberExpresssion::paint(QPainter* painter, int x, int y) const
{
  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->setFont(font());

  QFontMetrics fm(painter->font());
  QPoint pos(x, y + fm.ascent());

  if (m_exponent.isEmpty()) {
    painter->drawText(pos, m_mantissa);
  }
  else {
    const QString mantissa_text = isMantissaEqualOne() ? suffixOfMantissa()
                                                       : m_mantissa + suffixOfMantissa();

    const int exponent_height = qRound(0.2 * fm.height());
    pos.setY(pos.y() + exponent_height);
    painter->drawText(pos, mantissa_text);

    const int mantissa_width = fm.horizontalAdvance(mantissa_text);
    painter->setFont( smallFont(painter->font()) );
    pos.setX(x + mantissa_width);
    pos.setY(y + QFontMetrics(painter->font()).ascent());
    painter->drawText(pos, m_exponent);
  }

  painter->restore();
}

int NumberExpresssion::calcWidth() const
{
  const QFont my_font = font();
  QFontMetrics fm(my_font);

  if (m_exponent.isEmpty()) {
    return fm.horizontalAdvance(m_mantissa);
  }
  else {
    const int mantissa_width = isMantissaEqualOne() ? fm.horizontalAdvance(suffixOfMantissa())
                                                    : fm.horizontalAdvance(m_mantissa + suffixOfMantissa());
    const int exponent_width = QFontMetrics(smallFont(my_font)).horizontalAdvance(m_exponent);
    return mantissa_width + exponent_width;
  }
}

int NumberExpresssion::calcHeight() const
{
  const double EXPONENT_HEIGHT_FACTOR = 1.2;

  int result = QFontMetrics(font()).height();
  if (!m_exponent.isEmpty()) {
    result = qRound(EXPONENT_HEIGHT_FACTOR * result);
  }

  return result;
}

int NumberExpresssion::calcAscent() const
{
  if (m_exponent.isEmpty()) {
    return AbstractExpression::calcAscent();
  }
  else {
    const int font_height = QFontMetrics(font()).height();
    return (font_height / 2) + qRound(font_height * 0.2);
  }
}

int NumberExpresssion::calcDescent() const
{
  if (m_exponent.isEmpty()) {
    return AbstractExpression::calcDescent();
  }
  else {
    const int font_height = QFontMetrics(font()).height();
    return -( (font_height - 1) / 2);
  }
}

int NumberExpresssion::calcCapDY() const
{
  const int factor = m_exponent.isEmpty() ? 8 : 3;
  return qRound(factor * capMultiplier().y());
}

// NumberExpresssion interface

QString NumberExpresssion::toString() const
{
  const char format = m_style == Style::Exponential ? 'E' : 'g';
  return QLocale::system().toString(m_number, format, 14);
}

void NumberExpresssion::update()
{
  QString string = toString();
  const int pos_e = string.indexOf('E', 0, Qt::CaseInsensitive);
  const QLocale locale = QLocale::system();

  if (pos_e == -1) {
    m_mantissa = string;
    m_exponent.clear();
  }
  else {
    m_mantissa = string.mid(0, pos_e);
    m_exponent = string.mid(pos_e + 1);

    // cut symbol '+' if exist
    if (!m_exponent.isEmpty() && m_exponent.at(0) == '+') {
      m_exponent.remove(0, 1);
    }

    // cut left zero exponent value
    int i = 0;
    while (i < (m_exponent.size() - 1)) {
      if (m_exponent.at(i) == '0'){
        m_exponent.remove(i, 1);
        --i;
      }
      ++i;
    }
  }

  // cut right zero mantissa value
  const int decimal_pos = m_mantissa.indexOf(locale.decimalPoint(), Qt::CaseInsensitive);
  if ( decimal_pos >= 0) {
    int i = m_mantissa.size() - 1;
    while ( (i > decimal_pos + 1) && m_mantissa.at(i) == '0') {
      m_mantissa.remove(i, 1);
      --i;
    }
  }

  setFlag(CalculateFlag::Width);
  setFlag(CalculateFlag::Height);
  setFlag(CalculateFlag::Ascent);
  setFlag(CalculateFlag::Descent);
  setFlag(CalculateFlag::CapDY);
}

// private

QFont NumberExpresssion::smallFont(const QFont& font) const
{
  const double FONT_FACTOR = 0.7;

  QFont small_font(font);
  small_font.setPointSizeF(FONT_FACTOR * small_font.pointSizeF());
  return small_font;
}

QString NumberExpresssion::suffixOfMantissa() const
{
  return isMantissaEqualOne() ? QStringLiteral("10")
                              : QStringLiteral("·10");
}

bool NumberExpresssion::isMantissaEqualOne() const
{
  return m_mantissa == "1";
}

} // namespace ExprDraw

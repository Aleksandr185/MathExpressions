#include "bracketed_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace MathExpressions {

BracketedExpression::BracketedExpression()
  : m_left_bracket(BracketStyle::Round), m_right_bracket(BracketStyle::Round)
{

}

BracketedExpression::BracketedExpression(ExpressionPtr expression, BracketStyle bracket_style)
  : m_left_bracket(bracket_style), m_right_bracket(bracket_style)
{
  setSon(expression);
}

BracketedExpression::BracketedExpression(ExpressionPtr expression,
                                         BracketStyle left_style, BracketStyle rigth_style)
  : m_left_bracket(left_style), m_right_bracket(rigth_style)
{
  setSon(expression);
}

void BracketedExpression::setLeftBracket(BracketStyle style)
{
  if (m_left_bracket != style) {
    m_left_bracket = style;
    setFlag(CalculateFlag::Width);
    setFlag(CalculateFlag::Descent);
    setFlag(CalculateFlag::CapDY);
    setFlag(CalculateFlag::CapDX);
  }
}

void BracketedExpression::setRightBracket(BracketStyle style)
{
  if (m_right_bracket != style) {
    m_right_bracket = style;
    setFlag(CalculateFlag::Width);
    setFlag(CalculateFlag::Descent);
    setFlag(CalculateFlag::CapDY);
    setFlag(CalculateFlag::CapDX);
  }
}

void BracketedExpression::removeBrackets()
{
  setLeftBracket(BracketStyle::None);
  setRightBracket(BracketStyle::None);
}

// AbstractExpression interface

MultiplicationFlags BracketedExpression::multiplicationFlags() const
{
  if (isBracketed() && (leftBracket() != BracketStyle::None)
      && (rightBracket() != BracketStyle::None)) {
    MultiplicationFlags result;
    result.setFlag(MultiplicationFlag::Left);
    result.setFlag(MultiplicationFlag::Right);
    result.setFlag(MultiplicationFlag::Brackets);
    result.setFlag(MultiplicationFlag::RoundBrackets,
                   leftBracket() == BracketStyle::Round && rightBracket() == BracketStyle::Round);
    return result;
  }
  else {
    return ChainExpression::multiplicationFlags();
  }
}

void BracketedExpression::paint(QPainter* painter, int x, int y) const
{
  if ( !isBracketed()
       || ((leftBracket() == BracketStyle::None) && (rightBracket() == BracketStyle::None)) ) {
    ChainExpression::paint(painter, x, y);
  }
  else {
    int left_bracket_dx = 0;

    if (leftBracket() != BracketStyle::None) {
      drawBracket(painter, x, y, left_bracket_dx, BracketPosition::Left);
    }

    ChainExpression::paint(painter, x + left_bracket_dx, y);

    if (rightBracket() != BracketStyle::None) {
      const int expr_width = ChainExpression::calcWidth();
      int dummy;
      drawBracket(painter, x + left_bracket_dx + expr_width, y, dummy, BracketPosition::Right);
    }
  }
}

int BracketedExpression::calcWidth() const
{
  int result = ChainExpression::calcWidth();

  if (isBracketed()) {
    if (m_left_bracket != BracketStyle::None) {
      const QChar& bracket_char = bracketChar(BracketPosition::Left);
      result += QFontMetrics(bracketFont(bracket_char)).horizontalAdvance(bracket_char);
    }
    if (m_right_bracket != BracketStyle::None) {
      const QChar& bracket_char = bracketChar(BracketPosition::Right);
      result += QFontMetrics(bracketFont(bracket_char)).horizontalAdvance(bracket_char);
    }
  }

  return result;
}

int BracketedExpression::calcAscent() const
{
  return ChainExpression::calcAscent() + lineWidth().y();
}

int BracketedExpression::calcDescent() const
{
  int result = ChainExpression::calcDescent() - lineWidth().y();

  if ( isBracketed()
       && ((leftBracket() == BracketStyle::Figure) || (rightBracket() == BracketStyle::Figure))
       && ((result - lineWidth().y()) & 1) ) {
    --result;
  }

  return result;
}

int BracketedExpression::calcCapDY() const
{
  if ( isBracketed()
       && ((leftBracket() != BracketStyle::None) || (rightBracket() != BracketStyle::None)) ) {
    return 0;
  }
  else {
    return ChainExpression::calcCapDY();
  }
}

void BracketedExpression::calcCapDX(int& dxLeft, int& dxRight) const
{
  if ( isBracketed()
       && ((leftBracket() != BracketStyle::None) || (rightBracket() != BracketStyle::None)) ) {
    dxLeft = 0;
    dxRight = 0;
  }
  else {
    ChainExpression::calcCapDX(dxLeft, dxRight);
  }
}

// BracketedExpression interfase

bool BracketedExpression::isBracketed() const
{
  return true;
}

const QChar& BracketedExpression::bracketChar(BracketPosition position) const
{
  static const QChar LEFT_ROUND   = QChar('(');
  static const QChar LEFT_SQUARE  = QChar('[');
  static const QChar LEFT_FIGURE  = QChar('{');

  static const QChar RIGHT_ROUND  = QChar(')');
  static const QChar RIGHT_SQUARE = QChar(']');
  static const QChar RIGHT_FIGURE = QChar('}');

  static const QChar MODULE = QChar('|');

  if (position == BracketPosition::Left) {
    switch (m_left_bracket) {
    case BracketedExpression::BracketStyle::Round:
      return LEFT_ROUND;
      //break;
    case BracketedExpression::BracketStyle::Square:
      return LEFT_SQUARE;
      //break;
    case BracketedExpression::BracketStyle::Figure:
      return LEFT_FIGURE;
      //break;
    case BracketedExpression::BracketStyle::Module:
      return MODULE;
      //break;
    default:
      return LEFT_ROUND;   // for BracketStyle::None (never call)
    }
  }
  else {
    switch (m_right_bracket) {
    case BracketedExpression::BracketStyle::Round:
      return RIGHT_ROUND;
      //break;
    case BracketedExpression::BracketStyle::Square:
      return RIGHT_SQUARE;
      //break;
    case BracketedExpression::BracketStyle::Figure:
      return RIGHT_FIGURE;
      //break;
    case BracketedExpression::BracketStyle::Module:
      return MODULE;
      //break;
    default:
      return RIGHT_ROUND;   // for BracketStyle::None (never call)
    }
  }
}

// private

QFont BracketedExpression::bracketFont(const QChar& bracket) const
{
  QFont result(font());
  QFontMetricsF fm(result);
  result.setPointSizeF(result.pointSizeF() * height() / fm.boundingRect(bracket).height());
  return result;
}

void BracketedExpression::drawBracket(QPainter* painter,
                                      int x, int y,
                                      int& dx, BracketPosition position) const
{
  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());

  const QChar& bracket_char = bracketChar(position);
  const QFont fn(bracketFont(bracket_char));
  QFontMetrics fm(fn);

  dx = fm.horizontalAdvance(bracket_char);
  const QPoint pos(x, y + fm.ascent() + height() / 2);
  const QRect bounding_rect = fm.boundingRect(bracket_char);
  const int dy = fm.ascent() + bounding_rect.top() + bounding_rect.height() / 2;

  painter->setFont(fn);
  painter->translate(pos.x(), pos.y() - dy);
  painter->drawText(0, 0, QString(bracket_char));

  painter->restore();
}

} // namespace MathExpressions

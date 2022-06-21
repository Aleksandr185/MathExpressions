#include "sign_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace MathExpressions {

const ushort CODE_PLUS                 = 0x002b; // +
const ushort CODE_MINUS                = 0x2212; // -
const ushort CODE_PLUS_MINUS           = 0x00b1; // ±
const ushort CODE_MINUS_PLUS           = 0x2213; // ∓
const ushort CODE_MULTIPLY             = 0x00b7; // ·
const ushort CODE_CROSS_MULTIPLY       = 0x00d7; // ×
const ushort CODE_DIVIDE               = 0x00f7; // ÷
const ushort CODE_SLASH                = 0x002f; // /
const ushort CODE_TILDA                = 0x007e; // ~
const ushort CODE_ARROW                = 0x2192; // →
const ushort CODE_ELLIPSIS             = 0x2026; // …
const ushort CODE_INFINITY             = 0x221e; // ∞
const ushort CODE_PARTIAL_DIFFERENTIAL = 0x2202; // ∂
const ushort CODE_PARALLEL             = 0x2016; // ‖
const ushort CODE_PERPENDICULAR        = 0x22a5; // ⊥
const ushort CODE_ANGLE                = 0x2220; // ∠
const ushort CODE_EQUALS               = 0x003d; // =
const ushort CODE_IDENTICAL_TO         = 0x2261; // ≡
const ushort CODE_NOT_EQUAL_TO         = 0x2260; // ≠
const ushort CODE_APPROX_EQUAL         = 0x2248; // ≈
const ushort CODE_ALMOST_EQUAL         = 0x2243; // ≃
const ushort CODE_LESS                 = 0x003c; // <
const ushort CODE_LESS_OR_EQUAL        = 0x2264; // ≤
const ushort CODE_APPROX_LESS          = 0x2272; // ≲
const ushort CODE_MUCH_LESS            = 0x226a; // ≪
const ushort CODE_GREATER              = 0x003e; // >
const ushort CODE_GREATER_OR_EQUAL     = 0x2265; // ≥
const ushort CODE_APPROX_GREATER       = 0x2273; // ≳
const ushort CODE_MUCH_GREATER         = 0x226b; // ≫

constexpr int LEFT_MARGIN  = 2;
constexpr int RIGHT_MARGIN = 2;

constexpr double MUCH_LESS_SPACE_FACTOR    = 0.7;
constexpr double MUCH_GREATER_SPACE_FACTOR = MUCH_LESS_SPACE_FACTOR;

SignExpression::SignExpression(SignType sign)
  : CharacterExpression( QChar(codeFromSign(sign)), true)
{

}

MultiplicationFlags SignExpression::multiplicationFlags() const
{
  const ushort code = character().unicode();
  MultiplicationFlags result;
  result.setFlag(MultiplicationFlag::Negative,
                 (code == CODE_MINUS) || (code == CODE_PLUS_MINUS) || (code == CODE_MINUS_PLUS) );
  return result;
}

bool SignExpression::isNeedBrackets() const
{
  const ushort code = character().unicode();
  return (code == CODE_MINUS) || (code == CODE_PLUS)
      || (code == CODE_PLUS_MINUS) || (code == CODE_MINUS_PLUS);
}

void SignExpression::paint(QPainter *painter, int x, int y) const
{
  const ushort code = character().unicode();
  x += LEFT_MARGIN * lineWidth().x();

  if ( code == CODE_MUCH_LESS || code == CODE_MUCH_GREATER ) {

    const QFont my_font = font();
    QFontMetrics fm(my_font);

    y += fm.ascent();

    painter->save();
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setFont(my_font);

    switch (code) {
    case CODE_MUCH_LESS:{
      const QChar ch(CODE_LESS);
      const QString ch_string = QString(ch);
      painter->drawText(x, y, ch_string);
      const int char_width = fm.horizontalAdvance(ch);
      x += qRound(MUCH_LESS_SPACE_FACTOR * char_width);
      painter->drawText(x, y, ch_string);
      break;
    }
    case CODE_MUCH_GREATER:{
      const QChar ch(CODE_GREATER);
      const QString ch_string = QString(ch);
      painter->drawText(x, y, ch_string);
      const int char_width = fm.horizontalAdvance(ch);
      x += qRound(MUCH_GREATER_SPACE_FACTOR * char_width);
      painter->drawText(x, y, ch_string);
      break;
    }
    } // switch

    painter->restore();
  }
  else {
    CharacterExpression::paint(painter, x, y);
  }
}

int SignExpression::calcWidth() const
{
  const int MARGINS = (LEFT_MARGIN + RIGHT_MARGIN) * lineWidth().x();

  const ushort code = character().unicode();

  switch (code) {
  case CODE_MUCH_LESS:{
    QFontMetrics fm(font());
    const int char_width = fm.horizontalAdvance(QChar(CODE_LESS));
    return qRound(MUCH_LESS_SPACE_FACTOR * char_width) + char_width + MARGINS;
  }
  case CODE_MUCH_GREATER:{
    QFontMetrics fm(font());
    const int char_width = fm.horizontalAdvance(QChar(CODE_GREATER));
    return qRound(MUCH_GREATER_SPACE_FACTOR * char_width) + char_width + MARGINS;
  }
  default:
    return CharacterExpression::calcWidth() + MARGINS;
  } // switch
}

int SignExpression::calcCapDY() const
{
  return INT_MAX;
}

ushort SignExpression::codeFromSign(SignType sign)
{
  switch (sign) {
  case Plus:
    return CODE_PLUS;
  case Minus:
    return CODE_MINUS;
  case PlusMinus:
    return CODE_PLUS_MINUS;
  case MinusPlus:
    return CODE_MINUS_PLUS;
  case Multiply:
    return CODE_MULTIPLY;
  case CrossMultiply:
    return CODE_CROSS_MULTIPLY;
  case Divide:
    return CODE_DIVIDE;
  case Slash:
    return CODE_SLASH;
  case Tilda:
    return CODE_TILDA;
  case Arrow:
    return CODE_ARROW;
  case Ellipsis:
    return CODE_ELLIPSIS;
  case Infinity:
    return CODE_INFINITY;
  case PartialDifferential:
    return CODE_PARTIAL_DIFFERENTIAL;
  case Parallel:
    return CODE_PARALLEL;
  case Perpendicular:
    return CODE_PERPENDICULAR;
  case Angle:
    return CODE_ANGLE;
  case Equals:
    return CODE_EQUALS;
  case IdenticalTo:
    return CODE_IDENTICAL_TO;
  case NotEqualTo:
    return CODE_NOT_EQUAL_TO;
  case ApproxEqual:
    return CODE_APPROX_EQUAL;
  case AlmostEqual:
    return CODE_ALMOST_EQUAL;
  case Less:
    return CODE_LESS;
  case LessOrEqual:
    return CODE_LESS_OR_EQUAL;
  case ApproxLess:
    return CODE_APPROX_LESS;
  case MuchLess:
    return CODE_MUCH_LESS;
  case Greater:
    return CODE_GREATER;
  case GreaterOrEqual:
    return CODE_GREATER_OR_EQUAL;
  case ApproxGreater:
    return CODE_APPROX_GREATER;
  case MuchGreater:
    return CODE_MUCH_GREATER;
  default:
    qCritical() << "SignExpression::codeFromSign: Unknown SignType!";
    return 0;
  }
}

} // namespace MathExpressions

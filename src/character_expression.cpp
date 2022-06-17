#include "character_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace MathExpressions {

const ushort SMALL_ALPHA   = 0x03b1; // α
const ushort SMALL_BETTA   = 0x03b2; // β
const ushort SMALL_GAMMA   = 0x03b3; // γ
const ushort SMALL_DELTA   = 0x03b4; // δ
const ushort SMALL_EPSILON = 0x03b5; // ε
const ushort SMALL_ZETA    = 0x03b6; // ζ
const ushort SMALL_ETA     = 0x03b7; // η
const ushort SMALL_THETA   = 0x03b8; // θ
const ushort SMALL_LOTA    = 0x03b9; // ι
const ushort SMALL_KAPPA   = 0x03ba; // κ
const ushort SMALL_LAMDA   = 0x03bb; // λ
const ushort SMALL_MU      = 0x03bc; // μ
const ushort SMALL_NU      = 0x03bd; // ν
const ushort SMALL_XI      = 0x03be; // ξ
const ushort SMALL_OMICRON = 0x03bf; // ο
const ushort SMALL_PI      = 0x03c0; // π
const ushort SMALL_RHO     = 0x03c1; // ρ

const ushort SMALL_SIGMA   = 0x03c3; // σ
const ushort SMALL_OMEGA   = 0x03c9; // ω

const ushort CAPITAL_ALPHA = 0x0391; // Α
const ushort CAPITAL_GAMMA = 0x0393; // Γ
const ushort CAPITAL_RHO   = 0x03a1; // Ρ
const ushort CAPITAL_SIGMA = 0x03a3; // Σ
const ushort CAPITAL_OMEGA = 0x03a9; // Ω

const ushort ELLIPSIS      = 0x2026; // …
const ushort PARTIAL_DIFFERENTIAL = 0x2202; // ∂

CharacterExpression::CharacterExpression(const QChar& character, bool read_only)
  : m_character(character), m_read_only(read_only)
{

}

void CharacterExpression::setCharacter(const QChar& value)
{
  if ( !isReadOnly() && m_character != value){
    m_character = value;
    setFlag(CalculateFlag::Width);
    setFlag(CalculateFlag::CapDY);
    setFlag(CalculateFlag::CapDX);
    setFlag(CalculateFlag::SuperscriptX);
  }
}

// AbstractExpression interface

void CharacterExpression::paint(QPainter* painter, int x, int y) const
{
  if (!m_character.isNull()) {
    const QFont my_font = font();
    QFontMetrics fm(my_font);
    const int leftBearing = fm.leftBearing(m_character);
    if (leftBearing < 0) {
      x -= leftBearing;
    }

    QPoint pos(x, y + fm.ascent());

    painter->save();
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setFont(my_font);
    painter->drawText(pos, m_character);
    painter->restore();
  }
}

int CharacterExpression::calcWidth() const
{
  QFontMetrics fm(font());
  int width = fm.horizontalAdvance(m_character);

  if (!m_character.isNull()) {
    const int leftBearing = fm.leftBearing(m_character);
    if (leftBearing < 0) {
      width -= leftBearing;
    }

    const int rightBearing = fm.rightBearing(m_character);
    if (rightBearing < 0) {
      width -= rightBearing;
    }
  }

  return width;
}

int CharacterExpression::calcHeight() const
{
  return QFontMetrics(font()).height();
}

int CharacterExpression::calcSuperscriptX() const
{
  int result = AbstractExpression::calcSuperscriptX();

  if (character().unicode() == PARTIAL_DIFFERENTIAL) {
    result += qRound(2 * capMultiplier().x());
  }

  return result;
}

int CharacterExpression::calcCapDY() const
{
  double DY = 0.0;
  const ushort code = character().unicode();

  switch (code) {
  case SMALL_ALPHA:
  case SMALL_GAMMA:
  case SMALL_EPSILON:
  case SMALL_ETA:
  case SMALL_LOTA:
  case SMALL_KAPPA:
  case SMALL_MU:
  case SMALL_NU:
  case SMALL_OMICRON:
  case SMALL_PI:
  case SMALL_RHO:
    DY = 8.8;
    break;

  case SMALL_BETTA:
  case SMALL_DELTA:
  case SMALL_ZETA:
  case SMALL_THETA:
  case SMALL_LAMDA:
  case SMALL_XI:
    DY = 4.0;
    break;

  case ELLIPSIS:
    DY = capMultiplier().y() == 1.0 ? capMultiplier().y()
                                    : INT_MAX / capMultiplier().y() - 1.0;
    break;
  }

  if (DY == 0.0) {
    if (code >= SMALL_SIGMA && code <= SMALL_OMEGA){
      DY = 8.8;
    }
    else if ( (code >= CAPITAL_ALPHA && code <= CAPITAL_RHO)
              || (code >= CAPITAL_SIGMA && code <= CAPITAL_OMEGA)) {
      DY = 4.0;
    }
  }

  return qRound(DY * capMultiplier().y());
}

void CharacterExpression::calcCapDX(int& dxLeft, int& dxRight) const
{
  double DX = 0.0;
  const ushort code = m_character.unicode();

  if ( (code >= CAPITAL_GAMMA && code <= CAPITAL_RHO)
       || (code >= CAPITAL_SIGMA && code <= CAPITAL_OMEGA)
       || (code == CAPITAL_ALPHA) || (code == SMALL_THETA)) {
    DX = 1.0;
  }

  dxLeft = qRound(DX * capMultiplier().x());

  switch (code) {
  case SMALL_EPSILON:
    DX = 1.0;
    break;

  case SMALL_THETA:
    DX = -0.5;
    break;

  default:
    DX = 0.0;
    break;
  }

  if (DX == 0.0){
    if ( (code >= CAPITAL_ALPHA && code <= CAPITAL_RHO)
        || (code >= CAPITAL_SIGMA && code <= CAPITAL_OMEGA) ) {
      DX = -1.0;
    }
  }

  dxRight = qRound(DX * capMultiplier().x());
}

} // namespace MathExpressions

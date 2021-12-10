#include "character_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace ExprDraw {

const ushort SMALL_ALPHA   = 0x3b1; // α
const ushort SMALL_BETTA   = 0x3b2; // β
const ushort SMALL_GAMMA   = 0x3b3; // γ
const ushort SMALL_DELTA   = 0x3b4; // δ
const ushort SMALL_EPSILON = 0x3b5; // ε
const ushort SMALL_ZETA    = 0x3b6; // ζ
const ushort SMALL_ETA     = 0x3b7; // η
const ushort SMALL_THETA   = 0x3b8; // θ
const ushort SMALL_LOTA    = 0x3b9; // ι
const ushort SMALL_KAPPA   = 0x3ba; // κ
const ushort SMALL_LAMDA   = 0x3bb; // λ
const ushort SMALL_MU      = 0x3bc; // μ
const ushort SMALL_NU      = 0x3bd; // ν
const ushort SMALL_XI      = 0x3be; // ξ
const ushort SMALL_OMICRON = 0x3bf; // ο
const ushort SMALL_PI      = 0x3c0; // π
const ushort SMALL_RHO     = 0x3c1; // ρ

const ushort SMALL_SIGMA   = 0x3c3; // σ
const ushort SMALL_OMEGA   = 0x3c9; // ω

const ushort CAPITAL_ALPHA = 0x391; // Α
const ushort CAPITAL_GAMMA = 0x393; // Γ
const ushort CAPITAL_RHO   = 0x3a1; // Ρ
const ushort CAPITAL_SIGMA = 0x3a3; // Σ
const ushort CAPITAL_OMEGA = 0x3a9; // Ω

CharacterExpression::CharacterExpression(const QChar& character)
  :m_character(character)
{

}

void CharacterExpression::setCharacter(const QChar& value)
{
  if ( canChangeCharacter(value) && m_character != value){
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

  if (character().unicode() == Symbol::PartialDifferential) {
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

  case Symbol::Ellipsis:
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

bool CharacterExpression::canChangeCharacter(const QChar& ) const
{
  return true;
}

} // namespace ExprDraw

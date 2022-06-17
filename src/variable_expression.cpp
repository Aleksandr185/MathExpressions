#include "variable_expression.h"

namespace MathExpressions {

VariableExpression::VariableExpression(const QString& text)
  :SimpleExpression(text)
{

}

VariableExpression::VariableExpression(const QChar& ch)
  :SimpleExpression(ch)
{

}

// AbstractExpression interface

QFont VariableExpression::font() const
{
  QFont fn = SimpleExpression::font();
  fn.setItalic(true);
  return fn;
}

int VariableExpression::calcSuperscriptX() const
{
  double DX = 0.0;
  const ushort code = text().isEmpty() ? 0 : text().back().unicode();

  switch (code) {
  case 'f':
  case 'd':
    DX = 2.0;
    break;
  case 'r':
    DX = 1.0;
    break;
  }

  return SimpleExpression::calcSuperscriptX() + qRound(DX * capMultiplier().x());
}

int VariableExpression::calcSubscriptX() const
{
  double DX = 3.0;
  const ushort code = text().isEmpty() ? 0 : text().back().unicode();

  switch (code) {
  case 'R':
    DX = 1.5;
    break;
  case 'W':
    DX = 6.0;
    break;
  case 'x':
    DX = 1.0;
    break;
  }

  return SimpleExpression::calcSubscriptX() - qRound(DX * capMultiplier().x());
}

void VariableExpression::calcCapDX(int& dxLeft, int& dxRight) const
{
  double dx = 0.0;
  const ushort front = text().isEmpty() ? 0 : text().front().unicode();

  switch (front) {
  case 'A': case 'f':
    dx = 5.0;
    break;
  case 'B': case 'D': case 'E': case 'F': case 'L': case 'P': case 'R':
  case 'S': case 'T': case 'Y': case 'Z': case 'q': case 'r': case 's':
  case 't': case 'y': case 'z':
    dx =1.0;
    break;
  case 'U': case 'V': case 'W':
    dx = 1.5;
    break;
  case 'u': case 'v': case 'w':
    dx = 0.5;
    break;
  default:
    dx = 0.0;
    break;
  }

  if (dx == 0.0) {
    if ( ((front >= 'a') && (front <= 'e'))
        || ((front >= 'g') && (front <= 'o'))) {
      dx = 1.0;
    }
  }
  dxLeft = qRound(dx * capMultiplier().x());

  const ushort back = text().isEmpty() ? 0 : text().back().unicode();
  switch (back) {
  case 'A': case 'f':
    dx = 5.0;
    break;
  case 'B': case 'D': case 'E': case 'F': case 'I': case 'P': case 'R': case 'X':
    dx =4.0;
    break;
  case 'C': case 'G': case 'H': case 'J': case 'K': case 'L': case 'O': case 'Q':
  case 'S': case 'U': case 'V': case 'W': case 'Y': case 'Z': case 'p':
    dx = 2.0;
    break;
  case 'M': case 'N':
    dx = 1.5;
    break;
  case 'T':
    dx = 2.4;
    break;
  case 'g': case 'h': case 'k': case 'm': case 'n': case 'o': case 'q': case 'r':
  case 's':
    dx = 1.0;
    break;
  case 'i': case 'j': case 'l': case 't':
    dx = 3.0;
    break;
  default:
    dx = 0.0;
    break;
  }

  if (dx == 0.0) {
    if ( ((back >= 'a') && (back <= 'e'))
         || ((back >= 'u') && (back <= 'z'))) {
      dx = 1.0;
    }
  }

  dxRight = qRound(dx * capMultiplier().x());
}

} // namespace MathExpressions

#ifndef BRACKETEDEXPRESSION_H
#define BRACKETEDEXPRESSION_H

#include "chain_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT BracketedExpression : public ChainExpression
{
public:
  enum BracketStyle {
    None,
    Round,     // ( )
    Square,    // [ ]
    Figure,    // { }
    Module     // | |
  };

  BracketedExpression();

  inline BracketStyle leftBracket() const { return m_left_bracket; }
  inline BracketStyle rightBracket() const { return m_right_bracket; }

  void setLeftBracket(BracketStyle style);
  void setRightBracket(BracketStyle style);
  void removeBrackets();

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcAscent() const override;
  int calcDescent() const override;
  int calcCapDY() const override;
  void calcCapDX(int& dxLeft, int& dxRight) const override;

  // BracketedExpression interfase
  virtual bool isBracketed() const;

private:
  BracketStyle m_left_bracket;
  BracketStyle m_right_bracket;

  enum BracketPosition{
    Left,
    Right
  };

  const QChar& bracketChar(BracketPosition position) const;
  QFont bracketFont(const QChar& bracket) const;
  void drawBracket(QPainter* painter, int x, int y, int& dx, BracketPosition position) const;

};

} // namespace ExprDraw

#endif // BRACKETEDEXPRESSION_H

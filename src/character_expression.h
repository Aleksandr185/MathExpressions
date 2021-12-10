#ifndef CHAREXPRESSION_H
#define CHAREXPRESSION_H

#include <QChar>

#include "abstract_expression.h"

namespace ExprDraw {

class CharacterExpression : public AbstractExpression
{
public:
  explicit CharacterExpression(const QChar& character);

  inline const QChar& character() const { return m_character; }
  void setCharacter(const QChar& value);

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcSuperscriptX() const override;
  int calcCapDY() const override;
  void calcCapDX(int& dxLeft, int& dxRight) const override;

  virtual bool canChangeCharacter(const QChar& newCharacter) const;

private:
  QChar m_character;
};

} // namespace ExprDraw

#endif // CHAREXPRESSION_H

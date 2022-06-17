#ifndef CHAREXPRESSION_H
#define CHAREXPRESSION_H

#include <QChar>

#include "abstract_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT CharacterExpression : public AbstractExpression
{
public:
  explicit CharacterExpression(const QChar& character, bool read_only = false);

  inline const QChar& character() const { return m_character; }
  void setCharacter(const QChar& value);

  inline bool isReadOnly() const { return m_read_only; }

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcSuperscriptX() const override;
  int calcCapDY() const override;
  void calcCapDX(int& dxLeft, int& dxRight) const override;

private:
  QChar m_character;
  bool m_read_only;
};

} // namespace MathExpressions

#endif // CHAREXPRESSION_H

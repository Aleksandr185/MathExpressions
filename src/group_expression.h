#ifndef GROUPEXPRESSION_H
#define GROUPEXPRESSION_H

#include "abstract_twin_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT GroupExpression : public AbstractTwinParentExpression
{
public:
  explicit GroupExpression(const QChar& symbol);

protected:
  // AbstractExpression interface
  void fontChanged() override;
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;

  // AbstractTwinParentExpression interface
  void updateFirstTwinFont() override;
  void updateSecondTwinFont() override;

  // GroupExprassion interface
  virtual int calcSymbolWidth() const;
  virtual int calcSymbolHeight() const;
  virtual void drawSymbol(QPainter *painter, int x, int y) const;

  inline const QChar& symbol() const { return m_symbol; }

  QFont smallFont() const;
  QFont symbolFont() const;

private:
  const QChar m_symbol;

  mutable int m_symbol_height;
  mutable int m_symbol_width;

  int symbolWidth() const;
  int symbolHeight() const;

};

} // namespace MathExpressions

#endif // GROUPEXPRESSION_H

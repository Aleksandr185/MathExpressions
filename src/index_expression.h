#ifndef INDEXEXPRESSION_H
#define INDEXEXPRESSION_H

#include "abstract_twin_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT IndexExpression : public AbstractTwinParentExpression
{
public:
  IndexExpression();

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;
  bool isArgNeedBrackets() const override;

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
  int calcCapDY() const override;

protected:
  // AbstractTwinParentExpression interface
  void updateFirstTwinFont() override;
  void updateSecondTwinFont() override;

private:
  QFont smallFont() const;
};

} // namespace ExprDraw

#endif // INDEXEXPRESSION_H

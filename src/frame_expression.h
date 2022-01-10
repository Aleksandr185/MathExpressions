#ifndef FRAMEEXPRESSION_H
#define FRAMEEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class FrameExpression : public AbstractParentExpression
{
public:
  FrameExpression();
  ~FrameExpression() override;

  // AbstractExpression interface
  QPen pen() const override;
  QBrush brush() const override;
  MultiplicationFlags multiplicationFlags() const override;
  bool isArgNeedBrackets() const override;
  bool isNeedBrackets() const override;

protected:
  void paint(QPainter *painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
  int calcSuperscriptX() const override;
  int calcSuperscriptY() const override;
  int calcSubscriptX() const override;
  int calcSubscriptY() const override;
  int calcCapDY() const override;
  void calcCapDX(int &dxLeft, int &dxRight) const override;
  void updateSonFont() override;
};

} // namespace ExprDraw

#endif // FRAMEEXPRESSION_H

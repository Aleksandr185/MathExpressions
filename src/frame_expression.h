#ifndef FRAMEEXPRESSION_H
#define FRAMEEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class FrameExpression : public AbstractParentExpression
{
public:
  FrameExpression(ExpressionPtr expression);
  ~FrameExpression() override;

  inline int frameWidth() const { return m_frame_width; }
  inline Qt::PenStyle frameStyle() const { return m_frame_style; }

  void setFrameWidth(int);
  void setFrameStyle(Qt::PenStyle);

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

private:
  int m_frame_width;
  Qt::PenStyle m_frame_style;
};

} // namespace MathExpressions

#endif // FRAMEEXPRESSION_H

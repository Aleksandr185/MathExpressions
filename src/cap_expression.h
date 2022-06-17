#ifndef CAPEXPRESSION_H
#define CAPEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT CapExpression : public AbstractParentExpression
{
public:
  enum class CapStyle {
    Points,
    Vector,
    Cap,
    Tilde,
    Line
  };

  CapExpression(ExpressionPtr son, CapStyle capStyle);

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;

  inline CapStyle capStyle() const { return m_cap_style; }
  inline int pointCount() const { return m_point_count; }

  void setPointCount(int count);

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
  int calcSuperscriptX() const override;
  int calcSuperscriptY() const override;
  int calcSubscriptX() const override;
  int calcCapDY() const override;

private:
  CapStyle m_cap_style;
  int      m_point_count;

  int capWidth() const;
  int capHeight() const;
  int selfHeight() const;
};

} // namespace MathExpressions

#endif // CAPEXPRESSION_H

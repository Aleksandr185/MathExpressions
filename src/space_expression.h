#ifndef SPACEEXPRESSION_H
#define SPACEEXPRESSION_H

#include "abstract_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT SpaceExpression : public AbstractExpression
{
public:
  explicit SpaceExpression(int value);

  inline int value() const { return m_value; }
  void setValue(int value);

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;

private:
  int m_value;
};

} // namespace MathExpressions

#endif // SPACEEXPRESSION_H

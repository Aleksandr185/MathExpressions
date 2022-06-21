#ifndef LIMITEXPRESSION_H
#define LIMITEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT LimitExpression : public AbstractParentExpression
{
public:
  explicit LimitExpression(ExpressionPtr expression = nullptr);

  // AbstractExpression interface
  bool isArgNeedBrackets() const;

protected:
  void paint(QPainter *painter, int x, int y) const;
  int calcWidth() const;
  int calcHeight() const;
  int calcAscent() const;
  int calcDescent() const;

  // AbstractParentExpression interface
  void updateSonFont();

private:
  static const QString& text();
};

} // namespace MathExpressions

#endif // LIMITEXPRESSION_H

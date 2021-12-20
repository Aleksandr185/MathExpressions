#ifndef LIMITEXPRESSION_H
#define LIMITEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT LimitExpression : public AbstractParentExpression
{
public:
  LimitExpression();

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

} // namespace ExprDraw

#endif // LIMITEXPRESSION_H

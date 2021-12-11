#ifndef STROKESEXPRESSION_H
#define STROKESEXPRESSION_H

#include "abstract_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT StrokesExpression : public AbstractExpression
{
public:
  StrokesExpression(int count);

  inline int count() const { return m_count; }
  void setCount(int count);

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;

private:
  int m_count;

  QFont enlargedFont(const QFont& font) const;
};

} // namespace ExprDraw

#endif // STROKESEXPRESSION_H

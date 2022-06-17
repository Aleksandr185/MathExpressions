#ifndef INDEXEXPRESSION_H
#define INDEXEXPRESSION_H

#include "abstract_twin_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT IndexExpression : public AbstractTwinParentExpression
{
public:
  IndexExpression(ExpressionPtr expression, ExpressionPtr subscript, ExpressionPtr superscript);

  inline ExpressionPtr expression() const { return son(); }
  inline ExpressionPtr subscript() const { return firstTwin(); }
  inline ExpressionPtr superscript() const { return secondTwin(); }

  inline void setExpression(ExpressionPtr p) { setSon(p); }
  inline void setSubscript(ExpressionPtr p) { setFirstTwin(p); }
  inline void setSuperscript(ExpressionPtr p) { setSeconsTwin(p); }

  inline bool hasSubscript() const { return hasFirstTwin(); }
  inline bool hasSuperscript() const { return hasSecondTwin(); }

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

} // namespace MathExpressions

#endif // INDEXEXPRESSION_H

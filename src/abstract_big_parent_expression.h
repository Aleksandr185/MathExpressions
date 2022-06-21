#ifndef ABSTRACTBIGPARENTEXPRESSION_H
#define ABSTRACTBIGPARENTEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT AbstractBigParentExpression : public AbstractParentExpression
{
public:
  AbstractBigParentExpression();
  ~AbstractBigParentExpression() override;

  inline bool hasDaughter() const { return m_daughter; }
  inline ExpressionPtr daughter() const { return m_daughter; }
  ExpressionPtr cutOffDaughter();

  void setDaughter(ExpressionPtr daughter);

protected:
  // AbstractExpression interface
  void fontChanged() override;
  void colorChanged() override;
  void paintDeviceChanged() override;

  // AbstractBigParentExpression interfase
  virtual void updateDaughterFont();
  virtual void updateDaughterColor();

private:
  ExpressionPtr m_daughter = nullptr;

  void updateDaughterPaintDevice();
};

} // namespace MathExpressions

#endif // ABSTRACTBIGPARENTEXPRESSION_H

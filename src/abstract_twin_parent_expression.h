#ifndef ABSTRACTTWINPARENTEXPRESSION_H
#define ABSTRACTTWINPARENTEXPRESSION_H

#include "abstract_parent_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT AbstractTwinParentExpression : public AbstractParentExpression
{
public:
  AbstractTwinParentExpression();
  ~AbstractTwinParentExpression() override;

  inline ExpressionPtr firstTwin() const { return m_first_twin; }
  inline ExpressionPtr secondTwin() const { return m_second_twin; }

  inline bool hasFirstTwin() const { return m_first_twin; }
  inline bool hasSecondTwin() const { return m_second_twin; }

  void setFirstTwin(ExpressionPtr firstTwin);
  void setSeconsTwin(ExpressionPtr secondTwin);

protected:
  // AbstractExpression interface
  void fontChanged() override;
  void colorChanged() override;
  void paintDeviceChanged() override;

  // AbstractTwinParentExpression interface
  virtual void updateFirstTwinFont();
  virtual void updateFirstTwinColor();

  virtual void updateSecondTwinFont();
  virtual void updateSecondTwinColor();

private:
  ExpressionPtr m_first_twin;
  ExpressionPtr m_second_twin;

  void updateFirstTwinPaintDevice();
  void updateSecondTwinPaintDevice();
};

} // namespace MathExpressions

#endif // ABSTRACTTWINPARENTEXPRESSION_H

#ifndef ABSTRACTPARENTEXPRESSION_H
#define ABSTRACTPARENTEXPRESSION_H

#include "abstract_expression.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT AbstractParentExpression : public AbstractExpression
{
public:
  AbstractParentExpression();
  ~AbstractParentExpression() override;

  void setSon(ExpressionPtr son);
  inline ExpressionPtr son() const {return m_son;}
  inline bool hasSon() const { return m_son; }

  ExpressionPtr cutOffSun();

protected:
  // AbstractExpression interface
  void fontChanged() override;
  void colorChanged() override;
  void paintDeviceChanged() override;

  // AbstractParentExpression interface
  virtual void updateSonFont();
  virtual void updateSonColor();

private:
  ExpressionPtr m_son = nullptr;

  void updateSonPaintDevice();
};

} // namespace MathExpressions

#endif // ABSTRACTPARENTEXPRESSION_H

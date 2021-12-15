#ifndef ABSTRACTTWINPARENTEXPRESSION_H
#define ABSTRACTTWINPARENTEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT AbstractTwinParentExpression : public AbstractParentExpression
{
public:
  AbstractTwinParentExpression();
  ~AbstractTwinParentExpression() override;

  inline AbstractExpression* firstTwin() const { return m_first_twin; }
  inline AbstractExpression* secondTwin() const { return m_second_twin; }

  inline bool hasFirstTwin() const { return m_first_twin; }
  inline bool hasSecondTwin() const { return m_second_twin; }

  void setFirstTwin(AbstractExpression* firstTwin);
  void setSeconsTwin(AbstractExpression* secondTwin);

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
  AbstractExpression* m_first_twin;
  AbstractExpression* m_second_twin;

  void updateFirstTwinPaintDevice();
  void updateSecondTwinPaintDevice();
};

} // namespace ExprDraw

#endif // ABSTRACTTWINPARENTEXPRESSION_H

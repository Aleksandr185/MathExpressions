#ifndef ABSTRACTBIGPARENTEXPRESSION_H
#define ABSTRACTBIGPARENTEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT AbstractBigParentExpression : public AbstractParentExpression
{
public:
  AbstractBigParentExpression();
  ~AbstractBigParentExpression() override;

  inline bool hasDaughter() const { return m_daughter; }
  inline AbstractExpression* daughter() const { return m_daughter; }
  AbstractExpression* cutOffDaughter();

  void setDaughter(AbstractExpression* daughter);

protected:
  // AbstractExpression interface
  void fontChanged() override;
  void colorChanged() override;
  void paintDeviceChanged() override;

  // AbstractBigParentExpression interfase
  virtual void updateDaughterFont();
  virtual void updateDaughterColor();

private:
  AbstractExpression* m_daughter = nullptr;

  void updateDaughterPaintDevice();
};

} // namespace ExprDraw

#endif // ABSTRACTBIGPARENTEXPRESSION_H

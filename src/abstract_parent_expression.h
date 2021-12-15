#ifndef ABSTRACTPARENTEXPRESSION_H
#define ABSTRACTPARENTEXPRESSION_H

#include "abstract_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT AbstractParentExpression : public AbstractExpression
{
public:
  AbstractParentExpression();
  ~AbstractParentExpression() override;

  void setSon(AbstractExpression *son);
  inline AbstractExpression* son() const {return m_son;}
  inline bool hasSon() const { return m_son; }

  AbstractExpression* cutOffSun();

protected:
  // AbstractExpression interface
  void fontChanged() override;
  void colorChanged() override;
  void paintDeviceChanged() override;

  // AbstractParentExpression interface
  virtual void updateSonFont();
  virtual void updateSonColor();

private:
  AbstractExpression* m_son = nullptr;

  void updateSonPaintDevice();
};

} // namespace ExprDraw

#endif // ABSTRACTPARENTEXPRESSION_H

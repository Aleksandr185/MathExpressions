#ifndef ABSTRACTPARENTEXPRESSION_H
#define ABSTRACTPARENTEXPRESSION_H

#include "abstract_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT AbstractParentExpression : public AbstractExpression
{
public:
  explicit AbstractParentExpression(AbstractExpression *son);
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

  virtual void setSonFont();
  virtual void setSonColor();
  virtual void setSonPaintDevice();

private:
  AbstractExpression* m_son = nullptr;
};

} // namespace ExprDraw

#endif // ABSTRACTPARENTEXPRESSION_H

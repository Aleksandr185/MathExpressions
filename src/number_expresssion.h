#ifndef NUMBEREXPRESSSION_H
#define NUMBEREXPRESSSION_H

#include <QString>

#include "abstract_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT NumberExpresssion : public AbstractExpression
{
public:
  enum class Style{
    Regular,
    Exponential
  };

  NumberExpresssion();

  inline double number() const { return m_number; }
  inline Style style() const { return m_style; }

  void setNumber(double number);
  void setStyle(Style style);

  // AbstractExpression interface
  MultiplicationFlags multiplicationFlags() const override;

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  int calcAscent() const override;
  int calcDescent() const override;
  int calcCapDY() const override;

  // NumberExpresssion interface
  virtual QString toString() const;
  void update();

private:
  double m_number;
  Style m_style;
  QString m_mantissa;
  QString m_exponent;

  QFont smallFont(const QFont& font) const;
  QString suffixOfMantissa() const;
  bool isMantissaEqualOne() const;
};

} // namespace ExprDraw

#endif // NUMBEREXPRESSSION_H

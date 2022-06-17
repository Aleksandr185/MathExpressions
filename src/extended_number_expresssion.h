#ifndef EXTENDEDNUMBEREXPRESSSION_H
#define EXTENDEDNUMBEREXPRESSSION_H

#include "number_expresssion.h"

namespace MathExpressions {

class MATH_EXPRESSIONS_EXPORT ExtendedNumberExpresssion : public NumberExpression
{
public:
  ExtendedNumberExpresssion();
  ExtendedNumberExpresssion(double number, int precision, int decimals, int maxDegree);

  inline int precision() const { return m_precision; }
  inline int decimals() const { return m_decimals; }
  inline int maxDegree() const { return m_max_degree; }

  void setPrecision(int precision);
  void setDecimals(int value);
  void setMaxDegree(int value);

protected:
  // NumberExpresssion interface
  QString toString() const override;

private:
  int m_precision;
  int m_decimals;
  int m_max_degree;

};

} // namespace MathExpressions

#endif // EXTENDEDNUMBEREXPRESSSION_H

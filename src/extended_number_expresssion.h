#ifndef EXTENDEDNUMBEREXPRESSSION_H
#define EXTENDEDNUMBEREXPRESSSION_H

#include "number_expresssion.h"

namespace ExprDraw {

class ExtendedNumberExpresssion : public NumberExpresssion
{
public:
  ExtendedNumberExpresssion();

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

} // namespace ExprDraw

#endif // EXTENDEDNUMBEREXPRESSSION_H

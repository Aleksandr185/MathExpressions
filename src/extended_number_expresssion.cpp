#include "extended_number_expresssion.h"

#include <math.h>

namespace ExprDraw {

const int DEFAULT_PRECISION  = 4;
const int DEFAULT_DECIMALS   = 4;
const int DAFAULT_MAX_DEGREE = 2;

ExtendedNumberExpresssion::ExtendedNumberExpresssion()
{
  m_precision  = DEFAULT_PRECISION;
  m_decimals   = DEFAULT_DECIMALS;
  m_max_degree = DAFAULT_MAX_DEGREE;
}

// public

void ExtendedNumberExpresssion::setPrecision(int precision)
{
  if (precision < 0)
    precision = DEFAULT_PRECISION;

  if (m_precision != precision) {
    m_precision = precision;
    update();
  }
}

void ExtendedNumberExpresssion::setDecimals(int value)
{
  if (value < 0)
    value = DEFAULT_DECIMALS;

  if (m_decimals != value) {
    m_decimals = value;
    update();
  }
}

void ExtendedNumberExpresssion::setMaxDegree(int value)
{
  if (m_max_degree != value) {
    m_max_degree = value;
    update();
  }
}

// NumberExpresssion interface

QString ExtendedNumberExpresssion::toString() const
{
  QLocale locale = QLocale::system();
  QString result = locale.toString(number(), 'E', m_precision);

  if ( (number() == 0.0) || (log10(fabs(number())) > (m_max_degree)) ) {
    result = locale.toString(locale.toDouble(result), 'f', m_decimals);
  }

  return result;
}

} // namespace ExprDraw

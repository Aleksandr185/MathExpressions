#include "space_expression.h"

namespace MathExpressions {

SpaceExpression::SpaceExpression(int value)
  :m_value(value)
{

}

void SpaceExpression::setValue(int value)
{
  if (m_value != value) {
    m_value = value;
    setFlag(CalculateFlag::Width);
  }
}

// AbstractExpression interface

void SpaceExpression::paint(QPainter*, int , int ) const
{
  // Do nothing
}

int SpaceExpression::calcWidth() const
{
  return m_value * lineWidth().x();
}

} // namespace MathExpressions

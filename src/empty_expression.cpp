#include "empty_expression.h"

#include <QFontMetrics>

namespace MathExpressions {

EmptyExpression::EmptyExpression()
{

}


void EmptyExpression::paint(QPainter*, int , int ) const
{
  // Do nothing
}

int EmptyExpression::calcHeight() const
{
  return QFontMetrics(font()).height();
}

} // namespace MathExpressions

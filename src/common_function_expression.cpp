#include "common_function_expression.h"

namespace MathExpressions {

const int SPACE = 3;

CommonFunctionExpression::CommonFunctionExpression()
{

}

CommonFunctionExpression::CommonFunctionExpression(ExpressionPtr func_name, ExpressionPtr arg)
{
  setSon(func_name);
  setDaughter(arg);
}

// AbstractExpression interface

MultiplicationFlags CommonFunctionExpression::multiplicationFlags() const
{
  MultiplicationFlags result = hasDaughter() ? daughter()->multiplicationFlags()
                                             : MultiplicationFlags();
  const bool is_daughter_bracketed = result.testFlag(MultiplicationFlag::Brackets);
  result.setFlag(MultiplicationFlag::Right, is_daughter_bracketed);
  result.setFlag(MultiplicationFlag::Left);
  return result;
}

bool CommonFunctionExpression::isArgNeedBrackets() const
{
  if (hasSon()) {
    return son()->isArgNeedBrackets();
  }
  else {
    return AbstractBigParentExpression::isArgNeedBrackets();
  }
}

// protected

void CommonFunctionExpression::paint(QPainter* painter, int x, int y) const
{
  const int dy = y + ascent();

  if (hasSon()) {
    son()->draw(painter, x, dy - son()->ascent(), Qt::AlignRight | Qt::AlignBottom);
  }

  if (hasDaughter()) {
    const int son_width = hasSon() ? son()->width() : 0;

    daughter()->draw(painter,
                     x + (SPACE * lineWidth().x()) + son_width,
                     dy - daughter()->ascent(), Qt::AlignRight | Qt::AlignBottom);
  }
}

int CommonFunctionExpression::calcWidth() const
{
  const int son_width = hasSon() ? son()->width() : 0;
  const int daughter_width = hasDaughter() ? daughter()->width() : 0;

  return son_width + (SPACE * lineWidth().x()) + daughter_width;
}

int CommonFunctionExpression::calcHeight() const
{
  return ascent() - descent() + 1;
}

int CommonFunctionExpression::calcAscent() const
{
  const int son_ascent = hasSon() ? son()->ascent() : 0;
  const int daughter_ascent = hasDaughter() ? daughter()->ascent() : 0;

  return qMax(son_ascent, daughter_ascent);
}

int CommonFunctionExpression::calcDescent() const
{
  const int son_descent = hasSon() ? son()->descent() : 0;
  const int daughter_descent = hasDaughter() ? daughter()->descent() : 0;

  return qMin(son_descent, daughter_descent);
}

} // namespace MathExpressions

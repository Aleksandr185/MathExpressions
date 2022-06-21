#include "index_expression.h"

#include "function_name_expression.h"

namespace MathExpressions {

IndexExpression::IndexExpression(ExpressionPtr expression,
                                 ExpressionPtr subscript, ExpressionPtr superscript)
{
  setSon(expression);
  setFirstTwin(subscript);
  setSeconsTwin(superscript);
}

// AbstractExpression interface

MultiplicationFlags IndexExpression::multiplicationFlags() const
{
  MultiplicationFlags result = hasSon() ? son()->multiplicationFlags()
                                        : MultiplicationFlags();
  result.setFlag(MultiplicationFlag::Right);
  return result;
}

bool IndexExpression::isArgNeedBrackets() const
{
  return dynamic_cast<FunctionNameExpression*>(son()) == nullptr;
}

void IndexExpression::paint(QPainter* painter, int x, int y) const
{
  int dy = 0;

  if (hasSecondTwin()) {
    const int son_superscript_x = hasSon() ? son()->superscriptX() : 0;
    const int son_superscript_y = hasSon() ? son()->superscriptY() : 0;
    dy = qMax(0, secondTwin()->height() - son_superscript_y);
    secondTwin()->draw(painter,
                       x + son_superscript_x,
                       y + dy + son_superscript_y,
                       Qt::AlignRight | Qt::AlignTop);
  }

  if (hasSon()) {
    son()->draw(painter, x, y + dy, Qt::AlignRight | Qt::AlignBottom);
  }

  if (hasFirstTwin()) {
    const int son_subscript_x = hasSon() ? son()->subscriptX() : 0;
    const int son_subscript_y = hasSon() ? son()->subscriptY() : 0;
    firstTwin()->draw(painter,
                      x + son_subscript_x,
                      y + dy + son_subscript_y,
                      Qt::AlignRight | Qt::AlignBottom);
  }
}

int IndexExpression::calcWidth() const
{
  const int son_width = hasSon() ? son()->width() : 0;
  const int son_superscript_x = hasSon() ? son()->superscriptX() : 0;
  const int son_subscript_x = hasSon() ? son()->subscriptX() : 0;

  const int first_width = hasFirstTwin() ? firstTwin()->width() : 0;
  const int second_width = hasSecondTwin() ? secondTwin()->width() : 0;


  return qMax( qMax(son_width, son_superscript_x + first_width),
               son_subscript_x + second_width);
}

int IndexExpression::calcHeight() const
{
  int height = hasSon() ? son()->height() : 0;

  if (hasFirstTwin()) {
    const int son_superscript_y = hasSon() ? son()->superscriptY() : 0;
    height += qMax(0, firstTwin()->height() - height + son_superscript_y);
  }

  if (hasSecondTwin()) {
    const int son_subscript_y = hasSon() ? son()->subscriptY() : 0;
    height += qMax(0, secondTwin()->height() - son_subscript_y);
  }

  return height;
}

int IndexExpression::calcAscent() const
{
  int ascent = hasSon() ? son()->ascent() : 0;

  if (hasSecondTwin()) {
    const int son_subscript_y = hasSon() ? son()->subscriptY() : 0;
    ascent += qMax(0, secondTwin()->height() - son_subscript_y);
  }

  return ascent;
}

int IndexExpression::calcDescent() const
{
  int descent = hasSon() ? son()->descent() : 0;

  if (hasFirstTwin()) {
    const int son_height = hasSon() ? son()->height() : 0;
    const int son_superscript_y = hasSon() ? son()->superscriptY() : 0;
    descent -= qMax(0, firstTwin()->height() - son_height + son_superscript_y);
  }

  return descent;
}

int IndexExpression::calcCapDY() const
{
  if (hasSecondTwin()) {
    return secondTwin()->capDY();
  }
  else if (hasSon()) {
    return son()->capDY();
  }
  else {
    return 0;
  }
}

// AbstractTwinParentExpression interface

void IndexExpression::updateFirstTwinFont()
{
  firstTwin()->setFont(smallFont());
}

void IndexExpression::updateSecondTwinFont()
{
  secondTwin()->setFont(smallFont());
}

// private

QFont IndexExpression::smallFont() const
{
  const double FONT_FACTOR = 0.7;

  QFont small_font(font());
  small_font.setPointSizeF(FONT_FACTOR * small_font.pointSizeF());
  return small_font;
}

} // namespace MathExpressions

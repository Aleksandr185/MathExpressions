#include "chain_expression.h"

namespace ExprDraw {

ChainExpression::ChainExpression()
{

}

// AbstractExpression interface

MultiplicationFlags ChainExpression::multiplicationFlags() const
{

  AbstractExpression* right = nullptr;
  if ( hasSon() ) {
    right = son();

    while (right->hasNext()) {
      right = right->next();
    }
  }

  MultiplicationFlags right_flags = right ? right->multiplicationFlags()
                                          : AbstractExpression::multiplicationFlags();
  MultiplicationFlags left_flags = hasSon() ? son()->multiplicationFlags()
                                            : right_flags;

  MultiplicationFlags result;
  result.setFlag(MultiplicationFlag::Right, right_flags.testFlag(MultiplicationFlag::Right));
  result.setFlag(MultiplicationFlag::Left, left_flags.testFlag(MultiplicationFlag::Left));
  result.setFlag(MultiplicationFlag::Negative, left_flags.testFlag(MultiplicationFlag::Negative));

  return result;
}

void ChainExpression::paint(QPainter* painter, int x, int y) const
{
  y += ascent();

  AbstractExpression* expression = son();
  while (expression) {
    expression->draw(painter, x, y, HorizontalAlignment::Right, VerticalAlignment::Center);
    x += expression->width();

    expression = expression->next();
  }
}

int ChainExpression::calcWidth() const
{
  int width = 0;

  AbstractExpression* expression = son();
  while (expression) {
    width += expression->width();
    expression = expression->next();
  }

  return width;
}

int ChainExpression::calcHeight() const
{
  int over;
  int above;
  calcOverAbove(over, above);

  return over + above;
}

int ChainExpression::calcAscent() const
{
  int over;
  int above;
  calcOverAbove(over, above);

  return over - 1;
}

int ChainExpression::calcDescent() const
{
  int over;
  int above;
  calcOverAbove(over, above);

  return - above;
}

int ChainExpression::calcCapDY() const
{
  int result = INT_MAX;

  AbstractExpression* expression = son();
  while (expression) {
    int dy = expression->capDY();
    if (dy < result)
      result = dy;
    expression = expression->next();
  }

  return result; // NOTE: maybe return 0 if !hasSon() or AbstractExpression::calcCapDY() ?
}

void ChainExpression::calcCapDX(int& dxLeft, int& dxRight) const
{
  dxLeft = hasSon() ? son()->capDXLeft() : 0;

  AbstractExpression* right = nullptr;
  if ( hasSon()) {
    right = son();

    while (right) {
      right = right->next();
    }
  }

  dxRight = right ? right->capDXRight() : 0;
}

// private

void ChainExpression::calcOverAbove(int& over, int& above) const
{
  over = 0;
  above = 0;

  AbstractExpression* expression = son();
  while (expression) {
    over = qMax(over, expression->ascent() + 1);
    above = qMax(above, expression->height() - expression->ascent() - 1);
    expression = expression->next();
  }
}

} // namespace ExprDraw




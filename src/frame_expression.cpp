#include "frame_expression.h"

#include <QPainter>

namespace ExprDraw {

const int FRAME_WIDTH = 1;

FrameExpression::FrameExpression()
{

}

FrameExpression::~FrameExpression()
{
  cutOffSun();
}

QPen FrameExpression::pen() const
{
  return hasSon() ? son()->pen() : AbstractParentExpression::pen();
}

QBrush FrameExpression::brush() const
{
  return hasSon() ? son()->brush() : AbstractParentExpression::brush();
}

MultiplicationFlags FrameExpression::multiplicationFlags() const
{
  return hasSon() ? son()->multiplicationFlags()
                  : AbstractParentExpression::multiplicationFlags();
}

bool FrameExpression::isArgNeedBrackets() const
{
  return hasSon() ? son()->isArgNeedBrackets() : AbstractParentExpression::isArgNeedBrackets();
}

bool FrameExpression::isNeedBrackets() const
{
  return hasSon() ? son()->isNeedBrackets() : AbstractParentExpression::isNeedBrackets();
}

void FrameExpression::paint(QPainter *painter, int x, int y) const
{
  painter->save();
  QPen pen;
  pen.setStyle(Qt::DotLine);
  pen.setWidth(FRAME_WIDTH);
  pen.setColor(color());
  painter->setPen(pen);
  painter->setBrush(QBrush(Qt::NoBrush));
  painter->drawRect(x, y, width(), height());
  painter->restore();

  if (hasSon()) {
    son()->draw(painter, x, y, Qt::AlignRight | Qt::AlignBottom);
  }
}

int FrameExpression::calcWidth() const
{
  return hasSon() ? son()->width() : FRAME_WIDTH * 2;
}

int FrameExpression::calcHeight() const
{
  return hasSon() ? son()->height() : FRAME_WIDTH * 2;
}

int FrameExpression::calcAscent() const
{
  return hasSon() ? son()->ascent() : FRAME_WIDTH;
}

int FrameExpression::calcDescent() const
{
  return hasSon() ? son()->descent() : FRAME_WIDTH;
}

int FrameExpression::calcSuperscriptX() const
{
  return hasSon() ? son()->superscriptX() : AbstractParentExpression::calcSuperscriptX();
}

int FrameExpression::calcSuperscriptY() const
{
  return hasSon() ? son()->superscriptY() : AbstractParentExpression::calcSuperscriptY();
}

int FrameExpression::calcSubscriptX() const
{
  return hasSon() ? son()->subscriptX() : AbstractParentExpression::calcSubscriptX();
}

int FrameExpression::calcSubscriptY() const
{
  return hasSon() ? son()->subscriptY() : AbstractParentExpression::calcSubscriptY();
}

int FrameExpression::calcCapDY() const
{
  return hasSon() ? son()->capDY(): AbstractParentExpression::calcCapDY();
}

void FrameExpression::calcCapDX(int &dxLeft, int &dxRight) const
{
  if ( hasSon() ) {
    dxLeft = son()->capDXLeft();
    dxRight = son()->capDXRight();
  }
  else {
    dxLeft = AbstractParentExpression::capDXLeft();
    dxRight = AbstractParentExpression::capDXRight();
  }
}

void FrameExpression::updateSonFont()
{
  son()->setFont(font());
}

} // namespace ExprDraw




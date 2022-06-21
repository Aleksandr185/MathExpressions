#include "frame_expression.h"

#include <QPainter>
#include <QDebug>

namespace MathExpressions {

FrameExpression::FrameExpression(ExpressionPtr expression)
  : m_frame_width(1), m_frame_style(Qt::DotLine)
{
  setSon(expression);
}

FrameExpression::~FrameExpression()
{

}

void FrameExpression::setFrameWidth(int v)
{
  if (v < 0) {
    qWarning() << "frame width can't be less then 0!";
    return;
  }

  if (m_frame_width != v) {
    m_frame_width = v;
    setFlag(CalculateFlag::Width);
    setFlag(CalculateFlag::Height);
    setFlag(CalculateFlag::Ascent);
    setFlag(CalculateFlag::Descent);
  }
}

void FrameExpression::setFrameStyle(Qt::PenStyle v)
{
  if (m_frame_style != v) {
    m_frame_style = v;
  }
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
  pen.setStyle(m_frame_style);
  pen.setWidth(m_frame_width);
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
  return hasSon() ? son()->width() : m_frame_width * 2;
}

int FrameExpression::calcHeight() const
{
  return hasSon() ? son()->height() : m_frame_width * 2;
}

int FrameExpression::calcAscent() const
{
  return hasSon() ? son()->ascent() : m_frame_width;
}

int FrameExpression::calcDescent() const
{
  return hasSon() ? son()->descent() : m_frame_width;
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

} // namespace MathExpressions




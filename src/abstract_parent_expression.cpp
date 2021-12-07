#include "abstract_parent_expression.h"

namespace ExprDraw {

AbstractParentExpression::AbstractParentExpression()
{

}

AbstractParentExpression::~AbstractParentExpression()
{
  delete m_son;
}

void AbstractParentExpression::setSon(AbstractExpression* son)
{
  delete m_son;
  m_son = son;

  if ( hasSon() ) {
    setParent(m_son, this);
    setSonFont();
    setSonPaintDevice();
  }

  setFlag(CalculateFlag::All);
}

AbstractExpression* AbstractParentExpression::cutOffSun()
{
  AbstractExpression* result = m_son;
  m_son = nullptr;
  setFlag(CalculateFlag::All);
  return result;
}

// AbstractExpression interface

void AbstractParentExpression::fontChanged()
{
  AbstractExpression::fontChanged();
  setSonFont();
}

void AbstractParentExpression::colorChanged()
{
  AbstractExpression::colorChanged();
  setSonColor();
}

void AbstractParentExpression::paintDeviceChanged()
{
  AbstractExpression::paintDeviceChanged();
  setSonPaintDevice();
}

void AbstractParentExpression::setSonFont()
{
  if ( hasSon() ) {
    const QFont my_font = font();

    if ( son()->font() != my_font )
      son()->setFont(my_font);
  }
}

void AbstractParentExpression::setSonColor()
{
  if ( hasSon() ) {
    son()->setColor(color());
  }
}

void AbstractParentExpression::setSonPaintDevice()
{
  if ( hasSon() && son()->paintDevice() != paintDevice()) {
    assignPaintDevice(son(), paintDevice(), lineWidthX(), lineWidthY(),
                      rLineWidthX(), rLineWidthY());
  }
}

} // namespace ExprDraw




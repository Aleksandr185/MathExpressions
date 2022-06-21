#include "abstract_parent_expression.h"

namespace MathExpressions {

AbstractParentExpression::AbstractParentExpression()
{

}

AbstractParentExpression::~AbstractParentExpression()
{
  delete m_son;
}

void AbstractParentExpression::setSon(ExpressionPtr son)
{
  delete m_son;
  m_son = son;

  if ( hasSon() ) {
    assignParent(m_son, this);
    updateSonPaintDevice();
    updateSonFont();
  }

  setFlag(CalculateFlag::All);
}

ExpressionPtr AbstractParentExpression::cutOffSun()
{
  ExpressionPtr result = m_son;
  m_son = nullptr;
  setFlag(CalculateFlag::All);
  return result;
}

// AbstractExpression interface

void AbstractParentExpression::fontChanged()
{
  AbstractExpression::fontChanged();
  if (hasSon()) {
    updateSonFont();
  }
}

void AbstractParentExpression::colorChanged()
{
  AbstractExpression::colorChanged();
  if (hasSon()) {
    updateSonColor();
  }
}

void AbstractParentExpression::paintDeviceChanged()
{
  AbstractExpression::paintDeviceChanged();
  if (hasSon()) {
    updateSonPaintDevice();
  }
}

void AbstractParentExpression::updateSonFont()
{
  const QFont my_font = font();

  if ( son()->font() != my_font )
    assignFont(son(), my_font, lineWidth(), capMultiplier());
}

void AbstractParentExpression::updateSonColor()
{
  son()->setColor(color());
}

void AbstractParentExpression::updateSonPaintDevice()
{
  if ( son()->paintDevice() != paintDevice()) {
    assignPaintDevice(son(), paintDevice(), lineWidth(), capMultiplier());
  }
}

} // namespace MathExpressions




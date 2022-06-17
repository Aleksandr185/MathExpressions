#include "abstract_big_parent_expression.h"

namespace MathExpressions {

AbstractBigParentExpression::AbstractBigParentExpression()
{

}

AbstractBigParentExpression::~AbstractBigParentExpression()
{
  delete m_daughter;
}

// public

ExpressionPtr AbstractBigParentExpression::cutOffDaughter()
{
  ExpressionPtr result = m_daughter;
  m_daughter = nullptr;
  setFlag(CalculateFlag::All);
  return result;
}

void AbstractBigParentExpression::setDaughter(ExpressionPtr daughter)
{
  delete m_daughter;
  m_daughter = daughter;

  if ( hasDaughter() ) {
    assignParent(m_daughter, this);
    updateDaughterPaintDevice();
    updateDaughterFont();
  }

  setFlag(CalculateFlag::All);
}

// protected
// AbstractExpression interface

void AbstractBigParentExpression::fontChanged()
{
  AbstractParentExpression::fontChanged();
  if (hasDaughter()) {
    updateDaughterFont();
  }
}

void AbstractBigParentExpression::colorChanged()
{
  AbstractParentExpression::colorChanged();
  if (hasDaughter()) {
    updateDaughterColor();
  }
}

void AbstractBigParentExpression::paintDeviceChanged()
{
  AbstractParentExpression::paintDeviceChanged();
  if (hasDaughter()) {
    updateDaughterPaintDevice();
  }
}

// AbstractBigParentExpression interfase

void AbstractBigParentExpression::updateDaughterFont()
{
  const QFont my_font = font();

  if ( daughter()->font() != my_font )
    assignFont(daughter(), my_font, lineWidth(), capMultiplier());
}

void AbstractBigParentExpression::updateDaughterColor()
{
  daughter()->setColor(color());
}

void AbstractBigParentExpression::updateDaughterPaintDevice()
{
  if ( daughter()->paintDevice() != paintDevice()) {
    assignPaintDevice(daughter(), paintDevice(), lineWidth(), capMultiplier());
  }
}

} // namespace MathExpressions




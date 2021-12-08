#include "abstract_big_parent_expression.h"

namespace ExprDraw {

AbstractBigParentExpression::AbstractBigParentExpression()
{

}

AbstractBigParentExpression::~AbstractBigParentExpression()
{
  delete m_daughter;
}

// public

AbstractExpression* AbstractBigParentExpression::cutOffDaughter()
{
  AbstractExpression* result = m_daughter;
  m_daughter = nullptr;
  setFlag(CalculateFlag::All);
  return result;
}

void AbstractBigParentExpression::setDaughter(AbstractExpression* daughter)
{
  delete m_daughter;
  m_daughter = daughter;

  if ( hasDaughter() ) {
    assignParent(m_daughter, this);
    updateDaughterFont();
    updateDaughterPaintDevice();
  }

  setFlag(CalculateFlag::All);
}

// protected
// AbstractExpression interface

void AbstractBigParentExpression::fontChanged()
{
  AbstractParentExpression::fontChanged();
  updateDaughterFont();
}

void AbstractBigParentExpression::colorChanged()
{
  AbstractParentExpression::colorChanged();
  updateDaughterColor();
}

void AbstractBigParentExpression::paintDeviceChanged()
{
  AbstractParentExpression::paintDeviceChanged();
  updateDaughterPaintDevice();
}

// AbstractBigParentExpression interfase

void AbstractBigParentExpression::updateDaughterFont()
{
  if ( hasDaughter() ) {
    const QFont my_font = font();

    if ( daughter()->font() != my_font )
      assignFont(daughter(), my_font, lineWidth(), capMultiplier());
  }
}

void AbstractBigParentExpression::updateDaughterColor()
{
  if ( hasDaughter() ) {
    daughter()->setColor(color());
  }
}

void AbstractBigParentExpression::updateDaughterPaintDevice()
{
  if ( hasDaughter() && daughter()->paintDevice() != paintDevice()) {
    assignPaintDevice(daughter(), paintDevice(), lineWidth(), capMultiplier());
  }
}

} // namespace ExprDraw




#include "abstract_twin_parent_expression.h"

namespace MathExpressions {

AbstractTwinParentExpression::AbstractTwinParentExpression()
  : m_first_twin(nullptr), m_second_twin(nullptr)
{

}

AbstractTwinParentExpression::~AbstractTwinParentExpression()
{
  delete m_first_twin;
  delete m_second_twin;
}

void AbstractTwinParentExpression::setFirstTwin(ExpressionPtr firstTwin)
{
  delete m_first_twin;
  m_first_twin = firstTwin;

  if ( hasFirstTwin() ) {
    assignParent(m_first_twin, this);
    updateFirstTwinPaintDevice();
    updateFirstTwinFont();
  }

  setFlag(CalculateFlag::All);
}

void AbstractTwinParentExpression::setSeconsTwin(ExpressionPtr secondTwin)
{
  delete m_second_twin;
  m_second_twin = secondTwin;

  if ( hasSecondTwin() ) {
    assignParent(m_second_twin, this);
    updateSecondTwinPaintDevice();
    updateSecondTwinFont();
  }

  setFlag(CalculateFlag::All);
}

// AbstractExpression interface

void AbstractTwinParentExpression::fontChanged()
{
  AbstractParentExpression::fontChanged();

  if (hasFirstTwin()) {
    updateFirstTwinFont();
  }

  if (hasSecondTwin()) {
    updateSecondTwinFont();
  }
}

void AbstractTwinParentExpression::colorChanged()
{
  AbstractParentExpression::colorChanged();

  if (hasFirstTwin()) {
    updateFirstTwinColor();
  }

  if (hasSecondTwin()) {
    updateSecondTwinColor();
  }
}

void AbstractTwinParentExpression::paintDeviceChanged()
{
  AbstractParentExpression::paintDeviceChanged();

  if (hasFirstTwin()) {
    updateFirstTwinPaintDevice();
  }

  if (hasSecondTwin()) {
    updateSecondTwinPaintDevice();
  }
}

// AbstractTwinParentExpression interface

void AbstractTwinParentExpression::updateFirstTwinFont()
{
  const QFont my_font = font();

  if ( firstTwin()->font() != my_font )
    assignFont(firstTwin(), my_font, lineWidth(), capMultiplier());
}

void AbstractTwinParentExpression::updateFirstTwinColor()
{
  firstTwin()->setColor(color());
}

void AbstractTwinParentExpression::updateSecondTwinFont()
{
  const QFont my_font = font();

  if ( secondTwin()->font() != my_font )
    assignFont(secondTwin(), my_font, lineWidth(), capMultiplier());
}

void AbstractTwinParentExpression::updateSecondTwinColor()
{
  secondTwin()->setColor(color());
}

// protected

void AbstractTwinParentExpression::updateFirstTwinPaintDevice()
{
  if (firstTwin()->paintDevice() != paintDevice()) {
    assignPaintDevice(firstTwin(), paintDevice(), lineWidth(), capMultiplier());
  }
}

void AbstractTwinParentExpression::updateSecondTwinPaintDevice()
{
  if (secondTwin()->paintDevice() != paintDevice()) {
    assignPaintDevice(secondTwin(), paintDevice(), lineWidth(), capMultiplier());
  }
}

} // namespace MathExpressions

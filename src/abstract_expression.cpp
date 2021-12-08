#include "abstract_expression.h"

#include <QFontMetrics>
#include <QPainter>

//#include <QDebug>


namespace ExprDraw {

AbstractExpression::AbstractExpression()
  : m_flags(CalculateFlag::NoFlag)
{

}

AbstractExpression::~AbstractExpression()
{
  delete m_next;
}

// public

QPen AbstractExpression::pen() const
{
  QPen pen;
  pen.setStyle(Qt::SolidLine);
  pen.setWidth(1);
  pen.setColor(m_color);
  return pen;
}

QBrush AbstractExpression::brush() const
{
  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  brush.setColor(m_color);
  return brush;
}

QColor AbstractExpression::color() const
{
  if ( m_color.isValid() )
    return m_color;
  else if ( hasParent() )
    return parent()->color();
  else
    return QColor(Qt::black);
}

int AbstractExpression::width() const
{
  if ( m_flags.testFlag(CalculateFlag::Width) ) {
    m_width = calcWidth();
    m_flags.setFlag(CalculateFlag::Width, false);
  }

  return m_width;
}

int AbstractExpression::height() const
{
  if ( m_flags.testFlag(CalculateFlag::Height) ) {
    m_height = calcHeight();
    m_flags.setFlag(CalculateFlag::Height, false);
  }

  return m_height;
}

int AbstractExpression::ascent() const
{
  if ( m_flags.testFlag(CalculateFlag::Ascent) ) {
    m_ascent = calcAscent();
    m_flags.setFlag(CalculateFlag::Ascent, false);
  }

  return m_ascent;
}

int AbstractExpression::descent() const
{
  if ( m_flags.testFlag(CalculateFlag::Descent) ) {
    m_descent = calcDescent();
    m_flags.setFlag(CalculateFlag::Descent, false);
  }

  return m_descent;
}

int AbstractExpression::superscriptX() const
{
  if ( m_flags.testFlag(CalculateFlag::SuperscriptX) ) {
    m_superscript_x = calcSuperscriptX();
    m_flags.setFlag(CalculateFlag::SuperscriptX, false);
  }

  return m_superscript_x;
}

int AbstractExpression::superscriptY() const
{
  if ( m_flags.testFlag(CalculateFlag::SuperscriptY) ) {
    m_superscript_y = calcSuperscriptY();
    m_flags.setFlag(CalculateFlag::SuperscriptY, false);
  }

  return m_superscript_y;
}

int AbstractExpression::subscriptX() const
{
  if ( m_flags.testFlag(CalculateFlag::SubscriptX) ) {
    m_subscript_x = calcSubscriptX();
    m_flags.setFlag(CalculateFlag::SubscriptX, false);
  }

  return m_subscript_x;
}

int AbstractExpression::subscriptY() const
{
  if ( m_flags.testFlag(CalculateFlag::SubscriptY) ) {
    m_subscript_y = calcSubscriptY();
    m_flags.setFlag(CalculateFlag::SubscriptY, false);
  }

  return m_subscript_y;
}

int AbstractExpression::capDXLeft() const
{
  if ( m_flags.testFlag(CalculateFlag::CapDX) ) {
    calcCapDX(m_cap_dx_left, m_cap_dx_right);
    m_flags.setFlag(CalculateFlag::CapDX, false);
  }

  return m_cap_dx_left;
}

int AbstractExpression::capDXRight() const
{
  if ( m_flags.testFlag(CalculateFlag::CapDX) ) {
    calcCapDX(m_cap_dx_left, m_cap_dx_right);
    m_flags.setFlag(CalculateFlag::CapDX, false);
  }

  return m_cap_dx_right;
}

int AbstractExpression::capDY() const
{
  if ( m_flags.testFlag(CalculateFlag::CapDY) ) {
    m_cap_dy = calcCapDY();
    m_flags.setFlag(CalculateFlag::CapDY, false);
  }

  return m_cap_dy;
}

void AbstractExpression::draw(int x, int y,
                              HorizontalAlignment hAligment,
                              VerticalAlignment vAligment) const
{
  if ( paintDevice() ) {
    QPainter painter(paintDevice());
    convertCoords(x, y, hAligment, vAligment);
    paint(&painter, x, y);
  }
  else{
    qCritical() << "ExprDraw::AbstractExpression::draw. PaintDevice is null!";
  }
}

void AbstractExpression::draw(QPainter* painter, int x, int y,
                              HorizontalAlignment hAligment, VerticalAlignment vAligment) const
{
  if (paintDevice() == painter->device()) {
    convertCoords(x, y, hAligment, vAligment);
    paint(painter, x, y);
  }
  else {
    qCritical() << "ExprDraw::AbstractExpression::draw. PaintDevice is not equal!";
  }
}

void AbstractExpression::setPaintDevice(QPaintDevice* paintDevice)
{
  if ( m_paint_device != paintDevice) {
    m_paint_device = paintDevice;
    m_flags.setFlag(CalculateFlag::All);
    setLineWidth();
    paintDeviceChanged();
  }
}

void AbstractExpression::setNext(AbstractExpression* next)
{
  delete m_next;
  m_next = next;

  if ( hasNext() )
    setParent(m_next, parent());
}

void AbstractExpression::setFont(const QFont& font)
{
  if ( m_font != font) {
    m_font = font;
    setLineWidth();
    fontChanged();
  }
}

void AbstractExpression::setColor(const QColor& color)
{
  if ( m_color != color ) {
    m_color = color;
    colorChanged();
  }
}

void AbstractExpression::addNext(AbstractExpression* next)
{
  if ( next ) {
    AbstractExpression* last_next = this;
    while (last_next->hasNext()) {
      last_next = last_next->next();
    }

    last_next->m_next = next;
    last_next->setFont(m_font);
    last_next->setPaintDevice( paintDevice() );
  }
  else{
    qCritical() << "ExprDraw::AbstractExpression::addNext. Can't add 'null' next!";
  }
}

MultiplicationFlags AbstractExpression::multiplicationFlags() const
{
  return MultiplicationFlags(MultiplicationFlag::Left | MultiplicationFlag::Right);
}

AbstractExpression* AbstractExpression::cutOff()
{
  AbstractExpression* result = m_next;
  m_next = nullptr;
  return result;
}

// protected

void AbstractExpression::fontChanged()
{
  m_flags.setFlag(CalculateFlag::All);
  if (hasNext())
    next()->setFont(m_font, lineWidthX(), lineWidthY(), capMultiplierX(), capMultiplierY());
}

void AbstractExpression::colorChanged()
{
  if (hasNext())
    next()->setColor(m_color);
}

void AbstractExpression::paintDeviceChanged()
{
  if (hasNext())
    assignPaintDevice(next(), paintDevice(),
                      m_line_width_x, m_line_width_y,
                      m_cap_multiplier_x, m_cap_multiplier_y);
}

bool AbstractExpression::isNeedBrackets() const
{
  return false;
}

bool AbstractExpression::isArgNeedBrackets() const
{
  return true;
}

int AbstractExpression::calcWidth() const
{
  return 0;
}

int AbstractExpression::calcHeight() const
{
  return 0;
}

int AbstractExpression::calcAscent() const
{
  return height() / 2;
}

int AbstractExpression::calcDescent() const
{
  return -((height() - 1) / 2);
}

int AbstractExpression::calcSuperscriptX() const
{
  return width() + m_line_width_x;
}

int AbstractExpression::calcSuperscriptY() const
{
  QFontMetrics fm(font());
  return fm.height() / 2;
}

int AbstractExpression::calcSubscriptX() const
{
  return AbstractExpression::calcSuperscriptX();
}

int AbstractExpression::calcSubscriptY() const
{
  return AbstractExpression::calcSuperscriptY();
}

int AbstractExpression::calcCapDY() const
{
  return 0;
}

void AbstractExpression::calcCapDX(int& dxLeft, int& dxRight) const
{
  dxLeft = 0;
  dxRight = 0;
}

void AbstractExpression::convertCoords(int& X, int& Y,
                                       HorizontalAlignment hAligment,
                                       VerticalAlignment vAligment) const
{
  if (hAligment == HorizontalAlignment::Center) {
    X -= width() / 2;
  }
  else if (hAligment == HorizontalAlignment::Right) {
    X -= width() - 1;
  }
// if (hAligment == HorizontalAlignment::Left) - Do nothing

  if (vAligment == VerticalAlignment::Center) {
    Y -= ascent();
  }
  else if (vAligment == VerticalAlignment::Bottom) {
    Y -= height() - 1;
  }
  // if (vAligment == VerticalAlignment::Top) - Do nothing
}

void AbstractExpression::setFont(const QFont& font, int line_width_x, int line_width_y,
                                 double cap_multiplier_x, double cap_multiplier_y)
{
  if (m_font != font){
    m_font = font;
    m_line_width_x = line_width_x;
    m_line_width_y = line_width_y;
    m_cap_multiplier_x = cap_multiplier_x;
    m_cap_multiplier_y = cap_multiplier_y;
    fontChanged();
  }
}

// protected static

void AbstractExpression::setParent(AbstractExpression* expression, AbstractExpression* parent)
{
  if (!expression){
    qCritical() << "Can't set 'parent' for null 'expression'!";
    return;
  }

  if (expression->parent() != parent) {
    expression->m_parent = parent;

    if (expression->hasNext())
      setParent(expression->next(), parent);
  }
}

void AbstractExpression::assignPaintDevice(AbstractExpression* expression,
                                           QPaintDevice* paintDevice,
                                           int line_width_x, int line_width_y,
                                           double cap_multiplier_x, double cap_multiplier_y)
{
  if (!expression){
    qCritical() << "Can't assign 'paintDevice' for null 'expression'!";
    return;
  }

  if (expression->paintDevice() != paintDevice) {
    expression->m_paint_device = paintDevice;
    expression->setFlag(CalculateFlag::All);
    expression->m_line_width_x = line_width_x;
    expression->m_line_width_y = line_width_y;
    expression->m_cap_multiplier_x = cap_multiplier_x;
    expression->m_cap_multiplier_y = cap_multiplier_y;
    expression->paintDeviceChanged();
  }
}

// private

void AbstractExpression::setLineWidth()
{
  double factorDpiX = 1.0;
  double factorDpiY = 1.0;
  if (m_paint_device) {
    const int logicalDpiX = m_paint_device->logicalDpiX();
    const int logicalDpiY = m_paint_device->logicalDpiY();

    factorDpiY = logicalDpiX / logicalDpiY;
    factorDpiX = logicalDpiY / logicalDpiX;
  }

  QFontMetricsF fm(font());
  m_cap_multiplier_x = fm.height() / 27.6; // NOTE: what is 27.6 ?
  m_cap_multiplier_y = m_cap_multiplier_x * factorDpiX;

  m_line_width_y = fm.boundingRect('_').height();
  m_line_width_x = qRound(m_line_width_y * factorDpiY);
}

} // namespace ExprDraw

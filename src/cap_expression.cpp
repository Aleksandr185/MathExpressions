#include "cap_expression.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPolygon>

/* if ORIGINAL_POINT_SIZE defined will be used original code, wrote by Grigorev Anton,
 * for paining cap 'point'.*/

// TODO: use char 'point' from font.

//#define ORIGINAL_POINT_SIZE

namespace MathExpressions {

#ifndef ORIGINAL_POINT_SIZE
  const int POINT_SIZE = 2;
  const int POINT_SPACE = 2;
#endif

const QChar CHAR_CAP = QChar('^');
const QChar CHAR_TILDE = QChar('~');

CapExpression::CapExpression(ExpressionPtr son, CapStyle capStyle)
  : m_cap_style(capStyle), m_point_count(1)
{
  setSon(son);
}

MultiplicationFlags CapExpression::multiplicationFlags() const
{
  if (hasSon())
    return son()->multiplicationFlags();
  else
    return AbstractParentExpression::multiplicationFlags();
}

void CapExpression::setPointCount(int count)
{
  if (count < 1)
    count = 1;

  if (m_cap_style == CapStyle::Points && m_point_count != count){
    m_point_count = count;
    setFlag(CalculateFlag::Width);
  }
}

void CapExpression::paint(QPainter* painter, int x, int y) const
{
  if (hasSon()){

    const bool isPointTildaCap = (m_cap_style == CapStyle::Points)
        || (m_cap_style == CapStyle::Tilde) || (m_cap_style == CapStyle::Cap);

    const int dxLeft  = son()->capDXLeft();
    const int dxRight = son()->capDXRight();
    const int half_cap_width = isPointTildaCap ? capWidth() / 2 : 0;
    const int center_x = isPointTildaCap ? (dxLeft + son()->width() + dxRight) / 2 : 0;
    const int dx = isPointTildaCap ? qMax(half_cap_width - center_x, 0)
                                   : qMax(0, -dxLeft);

    const int son_y = y + selfHeight();

    son()->draw(painter, x + dx, son_y, Qt::AlignRight | Qt::AlignBottom);

    painter->save();
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setFont(font());

    const int dy = son_y + son()->capDY() - lineWidth().y();

    if (m_cap_style == CapStyle::Points) {
      const int pen_width = painter->pen().width();
      const int line_x = lineWidth().x();
      const int line_y = lineWidth().y();
      const int rec_left_const = x  + dx + center_x - half_cap_width;

      QRect rec;

#ifdef ORIGINAL_POINT_SIZE
      const int rec_right_const = rec_left_const + (2 * line_x);
      rec.setTop(dy - 3 * line_y);
      rec.setBottom(dy - line_y - pen_width);

      for (int i = 0; i < m_point_count; ++i) {
        rec.setLeft(rec_left_const + 4 * line_x * i);
        rec.setRight(rec_right_const + 4 * line_x * i - pen_width);
        painter->drawEllipse(rec);
      }
#else
      const int point_width = POINT_SIZE * line_x;
      const int point_offset = point_width + (line_x * POINT_SPACE);
      rec.setTop(dy - point_width - line_y);
      rec.setLeft(rec_left_const);
      rec.setRight(rec.left() + point_width - pen_width);
      rec.setBottom(dy - line_y - pen_width);
      painter->setRenderHint(QPainter::Antialiasing);

      for (int i = 0; i < m_point_count; ++i) {
        painter->drawEllipse(rec);
        rec.setLeft( rec.left() + point_offset);
        rec.setRight(rec.right() + point_offset);
      }
#endif
    }
    else if (m_cap_style == CapStyle::Vector) {
      QPolygon polygon;

      const int line_x = lineWidth().x();
      const int line_y = lineWidth().y();
      const int left_x = x + qMax(0, dxLeft);
      const int right_x = x + width() + qMin(0, dxRight);

      polygon << QPoint(left_x, dy - 3 * line_y);
      polygon << QPoint(right_x - 2 * line_x, polygon.at(0).y());
      polygon << QPoint(polygon.at(1).x(), polygon.at(1).y() - line_y);

      if (line_y & 1) {
        polygon << QPoint(right_x - 1, polygon.at(1).y() + (line_y / 2));
      }
      else{
        polygon << QPoint(right_x - 1, polygon.at(1).y() + (line_y / 2) - 1);
        polygon << QPoint(polygon.last().x(), polygon.last().y() + 1);
      }

      polygon << QPoint(polygon.at(2).x(), polygon.at(2).y() + (3 * line_y) - 1);
      polygon << QPoint(polygon.last().x(), polygon.last().y() - line_y);
      polygon << QPoint(polygon.at(0).x(), polygon.last().y());

      painter->setRenderHint(QPainter::Antialiasing, false);
      painter->drawPolygon(polygon);
    }
    else if (m_cap_style == CapStyle::Cap) {
      QFontMetrics fm(painter->font());

      QPoint pos;
      pos.setX(x + dx + center_x - half_cap_width);
      pos.setY(dy - qRound(15.0 * capMultiplier().y()) + fm.ascent());

      painter->drawText(pos, CHAR_CAP);
    }
    else if (m_cap_style == CapStyle::Tilde) {
      QFontMetrics fm(painter->font());

      QPoint pos;
      pos.setX(x + dx + center_x - half_cap_width);
      pos.setY(dy - qRound(18.5 * capMultiplier().y()) + fm.ascent());

      painter->drawText(pos, CHAR_TILDE);
    }
    else if (m_cap_style == CapStyle::Line){
      const int pen_width = painter->pen().width();
      const int left_x = x + qMax(0, dxLeft);
      const int right_x = x + width() + qMin(0, dxRight);

      QRect rect;
      rect.setLeft(left_x);
      rect.setRight(right_x - pen_width);
      rect.setTop(dy - 2 * lineWidth().y());
      rect.setBottom(dy - lineWidth().y() - pen_width);

      painter->drawRect(rect);
    }

    painter->restore();
  }

}

int CapExpression::calcWidth() const
{
  if (hasSon()) {

    int result = son()->width();
    const int son_dx_left = son()->capDXLeft();
    const int son_dx_right = son()->capDXRight();

    if ( (m_cap_style == CapStyle::Vector) || (m_cap_style == CapStyle::Line) ) {
      if (son_dx_left < 0)
        result -= son_dx_left;
      if (son_dx_right > 0)
        result += son_dx_right;
    }
    else{
      const int half_cap_width = capWidth() / 2;
      const int center_x = (son_dx_left + son_dx_right + result) / 2;
      result = qMax(center_x, half_cap_width) + qMax(result - center_x, half_cap_width);
    }

    return result;
  }
  else {
    return 0;
  }
}

int CapExpression::calcHeight() const
{
  const int son_height = hasSon() ? son()->height() : 0;
  return son_height + selfHeight();
}

int CapExpression::calcAscent() const
{
  const int son_ascent = hasSon() ? son()->ascent() : 0;
  return son_ascent + selfHeight();
}

int CapExpression::calcDescent() const
{
  return hasSon() ? son()->descent() : 0;
}

int CapExpression::calcSuperscriptX() const
{
  const int son_width = hasSon() ? son()->width() : 0;

  if (son_width == width()) {
    return hasSon() ? son()->superscriptX() : 0;
  }
  else {
    return AbstractParentExpression::calcSuperscriptX();
  }
}

int CapExpression::calcSuperscriptY() const
{
  const int son_superscript_y = hasSon() ? son()->superscriptY() : 0;
  return son_superscript_y + selfHeight();
}

int CapExpression::calcSubscriptX() const
{
  int dx = 0;
  const int son_dx_left = hasSon() ? son()->capDXLeft() : 0;
  if ( (m_cap_style == CapStyle::Points) || (m_cap_style == CapStyle::Cap)
       || (m_cap_style == CapStyle::Tilde) ) {
    const int son_width = hasSon() ? son()->width() : 0;
    const int son_dx_right = hasSon() ? son()->capDXRight() : 0;
    const int center_x = (son_dx_left + son_dx_right + son_width) / 2;
    const int half_cap_width = capWidth() / 2;
    dx = qMax(half_cap_width - center_x, 0);
  }
  else{
    dx = qMax(0, -son_dx_left);
  }

  const int son_subscript_x = hasSon() ? son()->subscriptX() : 0;
  return son_subscript_x + dx;
}

int CapExpression::calcCapDY() const
{
  const int son_dy = hasSon() ? son()->capDY() : 0;
  return qMax(0, son_dy - capHeight());
}

// private

int CapExpression::capWidth() const
{
  if (m_cap_style == CapStyle::Points) {
#ifdef ORIGINAL_POINT_SIZE
    return lineWidthX() * (4 * m_point_count - 2);
#else
    return (capHeight() * m_point_count) + (POINT_SPACE * lineWidth().x() * (m_point_count - 1));
#endif
  }
  else if (m_cap_style == CapStyle::Cap) {
    QFontMetrics fm(font());
    return fm.horizontalAdvance(CHAR_CAP);
  }
  else if (m_cap_style == CapStyle::Tilde) {
    QFontMetrics fm(font());
    return fm.horizontalAdvance(CHAR_TILDE);
  }

  return 0;
}

int CapExpression::capHeight() const
{
  if (m_cap_style == CapStyle::Points) {
#ifdef ORIGINAL_POINT_SIZE
    return 5 * lineWidthY();
#else
    return POINT_SIZE * lineWidth().y();
#endif
  }
  else if (m_cap_style == CapStyle::Vector || m_cap_style == CapStyle::Tilde) {
    return 6 * lineWidth().y();
  }
  else if (m_cap_style == CapStyle::Cap) {
    return 11 * lineWidth().y();              // NOTE: why not QFontMetrics::height('^') ?
  }
  else if (m_cap_style == CapStyle::Line){
    return 4 * lineWidth().y();
  }
  else {
    return 0;
  }
}

int CapExpression::selfHeight() const
{
  const int son_dy = hasSon() ? son()->capDY() : 0;
  return qMax(0, capHeight() - son_dy);
}

} // namespace MathExpressions

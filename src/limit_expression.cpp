#include "limit_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace MathExpressions {

LimitExpression::LimitExpression(ExpressionPtr expression)
{
  setSon(expression);
}

// AbstractExpression interface

bool LimitExpression::isArgNeedBrackets() const
{
  return false;
}

void LimitExpression::paint(QPainter *painter, int x, int y) const
{
  const QFont my_font = font();
  QFontMetrics fm(my_font);
  const int lim_width = fm.horizontalAdvance(text());
  const int son_width = hasSon() ? son()->width() : 0;

  const QPoint lim_pos(x + qMax(0, (son_width - lim_width) / 2),
                       y + fm.ascent());

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->setFont(my_font);
  painter->drawText(lim_pos, text());
  painter->restore();

  if (hasSon()) {
    son()->draw(painter,
              x + qMax(0, (lim_width - son_width) / 2),
              y + fm.height(), Qt::AlignRight | Qt::AlignBottom);
  }
}

int LimitExpression::calcWidth() const
{
  const int son_width = hasSon() ? son()->width() : 0;
  QFontMetrics fm(font());

  return qMax(fm.horizontalAdvance(text()), son_width);
}

int LimitExpression::calcHeight() const
{
  const int son_height = hasSon() ? son()->height() : 0;
  return son_height + QFontMetrics(font()).height();
}

int LimitExpression::calcAscent() const
{
  return QFontMetrics(font()).height() / 2;
}

int LimitExpression::calcDescent() const
{
  const int son_height = hasSon() ? son()->height() : 0;
  const int font_height = QFontMetrics(font()).height() - 1;

  return -(font_height / 2) - son_height;
}

// AbstractParentExpression interface

void LimitExpression::updateSonFont()
{
  const double FONT_FACTOR = 0.7;

  QFont small_font(font());
  small_font.setPointSizeF(FONT_FACTOR * small_font.pointSizeF());
  son()->setFont(small_font);
}

// private

const QString &LimitExpression::text()
{
  static const QString LIMIT_TEXT = QStringLiteral("lim");
  return LIMIT_TEXT;
}

} // namespace MathExpressions

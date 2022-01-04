#include "limit_expression.h"

#include <QFontMetrics>
#include <QPainter>

namespace ExprDraw {

static const QString LIMIT_STRING = QStringLiteral("lim");

LimitExpression::LimitExpression()
{

}

bool LimitExpression::isArgNeedBrackets() const
{
  return false;
}

void LimitExpression::paint(QPainter* painter, int x, int y) const
{
  const QFont my_font = font();
  QFontMetrics fm(my_font);
  const int lim_width = fm.horizontalAdvance(LIMIT_STRING);
  const int son_width = hasSon() ? son()->width() : 0;

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->setFont(my_font);
  painter->drawText(x + qMax(0, (son_width - lim_width) / 2),
                    y + fm.ascent(),
                    LIMIT_STRING);
  painter->restore();

  if (hasSon()) {
    son()->draw(painter,
                x + qMax(0, (lim_width - son_width) / 2),
                y + fm.height(),
                Qt::AlignRight | Qt::AlignBottom);
  }
}

int LimitExpression::calcWidth() const
{
  const int son_width = hasSon() ? son()->width() : 0;
  return qMax( son_width, QFontMetrics(font()).horizontalAdvance(LIMIT_STRING) );
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
  return -( font_height / 2 ) - son_height;
}

void LimitExpression::updateSonFont()
{
  const double FONT_FACTOR = 0.7;

  QFont small_font(font());
  small_font.setPointSizeF(FONT_FACTOR * small_font.pointSizeF());
  son()->setFont(small_font);
}

} // namespace ExprDraw

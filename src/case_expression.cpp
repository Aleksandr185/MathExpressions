#include "case_expression.h"

#include <QPainter>
#include <QFontMetricsF>

namespace MathExpressions {

static const QChar BRACKET = QChar('{');

const int BRACKET_SPACE   = 2;
const int CONDITION_SPACE = 4;
const int VERTICAL_SPACE  = 4;

CaseExpression::CaseExpression(ExpressionPtr expression)
{
  setSon(expression);
}

void CaseExpression::paint(QPainter* painter, int x, int y) const
{
  const QFont bracket_font = bracketFont();
  QFontMetrics fm(bracket_font);
  const QRect bracket_rect = fm.boundingRect(BRACKET);
  const int bracket_x = x - fm.leftBearing(BRACKET);
  const int bracket_y = y + (height() / 2) - bracket_rect.top() - (bracket_rect.height() / 2);

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->setFont(bracket_font);
  painter->translate(bracket_x, bracket_y);
  painter->drawText(0, 0, QString(BRACKET));
  painter->restore();

  const int x_expression = x + width();
  const int x_condition  = x + bracket_rect.width() + BRACKET_SPACE * lineWidth().x();

  int max_ascent = 0;
  int max_descent = 0;

  ExpressionPtr expression = son();
  ExpressionPtr condition = nullptr;
  int i = 1;

  while ( expression ) {
    if (i & 1) {
      max_ascent = expression->ascent();
      max_descent = expression->descent();
      condition = expression;
    }
    else {
      max_ascent = qMax(max_ascent, expression->ascent());
      max_descent = qMax(max_descent, -expression->descent());
      y += max_ascent;

      if (condition) {
        condition->draw(painter, x_condition, y, Qt::AlignRight | Qt::AlignVCenter);
      }

      expression->draw(painter, x_expression, y, Qt::AlignLeft | Qt::AlignVCenter);
      y += max_descent + VERTICAL_SPACE * lineWidth().y();
    }

    ++i;
    expression = expression->next();
  }
}

int CaseExpression::calcWidth() const
{
  int width_expression = 0;
  int width_condition = 0;

  int i = 1;
  ExpressionPtr next_expr = son();
  while (next_expr) {
    if (i & 1) {
      width_expression = qMax(width_expression, next_expr->width());
    }
    else {
      width_condition = qMax(width_condition, next_expr->width());
    }

    ++i;
    next_expr = next_expr->next();
  }

  const int bracket_width = QFontMetrics(bracketFont()).boundingRect(BRACKET).width();

  return bracket_width + ( (BRACKET_SPACE + CONDITION_SPACE) * lineWidth().x() )
      + width_expression + width_condition;
}

int CaseExpression::calcHeight() const
{
  int result = 0;
  int ascent = 0;
  int descent = 0;
  int i = 1;

  const int space = VERTICAL_SPACE * lineWidth().y();

  ExpressionPtr next_expr = son();
  while (next_expr) {
    if (i & 1) {
      ascent = next_expr->ascent();
      descent = qAbs(next_expr->descent());
    }
    else {
      result += qMax(ascent, next_expr->ascent());
      result += qMax(descent, qAbs(next_expr->descent()));

      if (next_expr->hasNext())
        result += space;
    }

    ++i;
    next_expr = next_expr->next();
  }

  if ( !(result & 1) )
    ++result;

  return result;
}

QFont CaseExpression::bracketFont() const
{
  QFont bracket_font = font();
  const int h = height() + lineWidth().x() * 2;
  const auto bracket_height = QFontMetricsF(bracket_font).boundingRect(BRACKET).height();
  bracket_font.setPointSizeF(bracket_font.pointSizeF() * h / bracket_height);

  return bracket_font;
}

} // namespace MathExpressions


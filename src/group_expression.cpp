#include "group_expression.h"

#include <QFontMetricsF>
#include <QPainter>

#include "chain_expression.h"

namespace MathExpressions {

const int SPACE_X   = 2;
const int SPACE_Y   = 2;
const int SPACE_SON = 1;

constexpr const int FULL_SPACE_X = (SPACE_X * 2) + SPACE_SON;

GroupExpression::GroupExpression(const QChar& symbol)
  : m_symbol(symbol)
{

}

// AbstractExpression interface

void GroupExpression::fontChanged()
{
  AbstractTwinParentExpression::fontChanged();
  setFlag(CalculateFlag::SymbolHeight);
  setFlag(CalculateFlag::SymbolWidth);
}

void GroupExpression::paint(QPainter* painter, int x, int y) const
{
  const int first_width = hasFirstTwin() ? firstTwin()->width() : 0;
  const int second_width = hasSecondTwin() ? secondTwin()->width() : 0;
  const int max_width = std::max( {first_width, second_width, symbolWidth()} );
  const int center_x = x + (max_width / 2) + SPACE_X * lineWidth().x();
  const int half_symbol_height = symbolHeight() / 2;

  y += ascent();

  if (hasSecondTwin()) {
    secondTwin()->draw(painter,
                       center_x, y - half_symbol_height,
                       Qt::AlignHCenter | Qt::AlignTop);
  }

  if (hasFirstTwin()) {
    firstTwin()->draw(painter,
                      center_x, y + half_symbol_height,
                      Qt::AlignHCenter | Qt::AlignBottom);
  }

  drawSymbol(painter, center_x, y);

  if (hasSon()) {
    son()->draw(painter,
                x + max_width + FULL_SPACE_X * lineWidth().x(), y,
                Qt::AlignRight | Qt::AlignVCenter);
  }
}

int GroupExpression::calcWidth() const
{
  const int first_width = hasFirstTwin() ? firstTwin()->width() : 0;
  const int second_width = hasSecondTwin() ? secondTwin()->width() : 0;
  const int max_width = std::max( {first_width, second_width , symbolWidth() } );
  const int son_width = hasSon() ? son()->width() : 0;

  return max_width + son_width + (FULL_SPACE_X * lineWidth().x());
}

int GroupExpression::calcHeight() const
{
  GroupExpression* son_as_group = dynamic_cast<GroupExpression*>(son());
  const int son_first_height = (son_as_group && son_as_group->hasFirstTwin())
      ? son_as_group->firstTwin()->height()
      : 0;
  const int son_second_height = (son_as_group && son_as_group->hasSecondTwin())
      ? son_as_group->secondTwin()->height()
      : 0;
  const int first_height = hasFirstTwin() ? firstTwin()->height()
                                          : SPACE_Y * lineWidth().y();
  const int second_height = hasSecondTwin() ? secondTwin()->height()
                                            : SPACE_Y * lineWidth().y();

  return symbolHeight() + qMax(first_height, son_first_height)
      + qMax(second_height, son_second_height);
}

int GroupExpression::calcAscent() const
{
  GroupExpression* son_as_group = dynamic_cast<GroupExpression*>(son());
  const int son_second_height = (son_as_group && son_as_group->hasSecondTwin())
      ? son_as_group->secondTwin()->height()
      : 0;
  const int second_height = hasSecondTwin() ? secondTwin()->height()
                                            : SPACE_Y * lineWidth().y();
  return (symbolHeight() / 2) + qMax(second_height, son_second_height);
}

int GroupExpression::calcDescent() const
{
 GroupExpression* son_as_group = dynamic_cast<GroupExpression*>(son());
 const int son_first_height = (son_as_group && son_as_group->hasFirstTwin())
     ? son_as_group->firstTwin()->height()
     : 0;
 const int first_height = hasFirstTwin() ? firstTwin()->height()
                                         : SPACE_Y * lineWidth().y();
 const int half_symbol_height = (symbolHeight() - 1) / 2;

 return -(half_symbol_height + qMax(first_height, son_first_height));
}

// AbstractTwinParentExpression interface

void GroupExpression::updateFirstTwinFont()
{
  firstTwin()->setFont(smallFont());
}

void GroupExpression::updateSecondTwinFont()
{
  secondTwin()->setFont(smallFont());
}

// GroupExprassion interface

int GroupExpression::calcSymbolWidth() const
{
  return QFontMetrics(symbolFont()).horizontalAdvance(m_symbol) + (SPACE_X * 2 * lineWidth().x());
}

int GroupExpression::calcSymbolHeight() const
{
  const double SYMBOL_HEIGHT_FACTOR = 2.25;

  ChainExpression* son_as_chain = dynamic_cast<ChainExpression*>(son());
  if (son_as_chain
      && !son_as_chain->multiplicationFlags().testFlag(MultiplicationFlag::RoundBrackets) ) {
    ExpressionPtr next = son_as_chain->son();

    while (next) {
      if (GroupExpression* next_as_group = dynamic_cast<GroupExpression*>(next)) {
        return next_as_group->symbolHeight();
      }

      next = next->next();
    }
  }

  if (GroupExpression* son_as_group = dynamic_cast<GroupExpression*>(son())) {
    return son_as_group->symbolHeight();
  }
  else {
    const int son_max = hasSon() ? qMax(son()->ascent(), -son()->descent()) : 0;
    const int font_height = QFontMetrics(font()).height();
    return qRound(SYMBOL_HEIGHT_FACTOR * qMax(son_max, font_height / 2));
  }
}

void GroupExpression::drawSymbol(QPainter* painter, int x, int y) const
{
  const QFont symbol_font = symbolFont();
  QFontMetrics fm(symbol_font);

  // Выставляем символ посередине. В разных шрифтах символы находятся на разной
  // высоте от базовой линии, нам надо чтобы символ всегда был посередине
  const QRect rec = fm.boundingRect(m_symbol);
  const int dy = rec.top() + (rec.height() / 2);
  const int dx = fm.horizontalAdvance(m_symbol) / 2;

  painter->save();
  painter->setPen(pen());
  painter->setBrush(brush());
  painter->setFont(symbol_font);
  painter->translate(x - dx, y - dy);
  painter->drawText(0, 0, QString(m_symbol));
  painter->restore();
}

QFont GroupExpression::smallFont() const
{
  const double FONT_FACTOR = 0.7;

  QFont small_font(font());
  small_font.setPointSizeF(FONT_FACTOR * small_font.pointSizeF());
  return small_font;
}

QFont GroupExpression::symbolFont() const
{
  QFont symbol_font(font());
  const auto font_height = QFontMetricsF(symbol_font).height();
  symbol_font.setPointSizeF(symbol_font.pointSizeF() * symbolHeight() / font_height);
  return symbol_font;
}

// private

int GroupExpression::symbolWidth() const
{
  if ( flags().testFlag(CalculateFlag::SymbolWidth) ) {
    m_symbol_width = calcSymbolWidth();
    setFlag(CalculateFlag::SymbolWidth, false);
  }

  return m_symbol_width;
}

int GroupExpression::symbolHeight() const
{
  if ( flags().testFlag(CalculateFlag::SymbolHeight) ) {
    m_symbol_height = calcSymbolHeight();
    setFlag(CalculateFlag::SymbolHeight, false);
  }

  return m_symbol_height;
}

} // namespace MathExpressions

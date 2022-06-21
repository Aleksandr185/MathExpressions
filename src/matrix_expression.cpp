#include "matrix_expression.h"

namespace MathExpressions {

constexpr int CELL_SPACE   = 6;
constexpr int LEFT_MARGIN  = 2;
constexpr int RIGHT_MARGIN = LEFT_MARGIN;

MatrixExpression::MatrixExpression(const QSize& size, ExpressionPtr expression)
{
  setSize(size);
  setSon(expression);
}

MatrixExpression::MatrixExpression(int w, int h, ExpressionPtr expression)
{
  setSize( QSize(w, h) );
  setSon(expression);
}

void MatrixExpression::setSize(const QSize& size)
{
  if (m_size != size) {
    m_size = size;
    setFlag(CalculateFlag::CellSize);
    setFlag(CalculateFlag::Width);
    setFlag(CalculateFlag::Height);
  }
}

// AbstractExpression interface

void MatrixExpression::paint(QPainter* painter, int x, int y) const
{
  if ( hasSon() ) {
    const int cell_width = cellWidth();
    const int cell_height = cellHeight();
    const int margin = LEFT_MARGIN * lineWidth().x();
    const int space = CELL_SPACE * lineWidth().x();

    ExpressionPtr next_expr = son();
    y += cell_height / 2;

    int h = 0;
    while (h < size().height() && next_expr) {
      int dx = x + margin + cell_width / 2;

      int w = 0;
      while (w < size().width() && next_expr) {
        next_expr->draw(painter, dx, y, Qt::AlignCenter);
        dx += cell_width + space;

        ++w;
        next_expr = next_expr->next();
      }

      y += cell_height;
      ++h;
    }

  } // if
}

int MatrixExpression::calcWidth() const
{
  constexpr int MARGIN = LEFT_MARGIN + RIGHT_MARGIN;
  const int cell_spaces = size().width() > 0 ? CELL_SPACE * (size().width() - 1)
                                             : 0;

  return ( cellWidth() * size().width() ) + lineWidth().x() * ( MARGIN + cell_spaces );
}

int MatrixExpression::calcHeight() const
{
  return cellHeight() * size().height();
}

void MatrixExpression::fontChanged()
{
  AbstractParentExpression::fontChanged();
  setFlag(CalculateFlag::CellSize);
}

// private

int MatrixExpression::cellWidth() const
{
  if ( flags().testFlag(CalculateFlag::CellSize) ) {
    calcCellSize(m_cell_width, m_cell_height);
    setFlag(CalculateFlag::CellSize, false);
  }

  return m_cell_width;
}

int MatrixExpression::cellHeight() const
{
  if ( flags().testFlag(CalculateFlag::CellSize) ) {
    calcCellSize(m_cell_width, m_cell_height);
    setFlag(CalculateFlag::CellSize, false);
  }

  return m_cell_height;
}

void MatrixExpression::calcCellSize(int& cell_width, int& cell_height) const
{
  // TODO recalc cell size if changed 'height' or 'width' or 'ascent'.

  cell_width = 0;

  int over = 0;
  int above = 0;

  const int cell_count = size().width() * size().height();
  int i = 0;
  ExpressionPtr next_expr = son();
  while ( next_expr && (i < cell_count) ) {
    over = qMax(over, next_expr->ascent() + 1);
    above = qMax(above, next_expr->height() - next_expr->ascent() - 1);
    cell_width = qMax(cell_width, next_expr->width());

    next_expr = next_expr->next();
    ++i;
  }

  cell_height = over + above;
}

} // namespace MathExpressions

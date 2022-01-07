#ifndef MATRIXEXPRESSION_H
#define MATRIXEXPRESSION_H

#include "abstract_parent_expression.h"

namespace ExprDraw {

class EXPRDRAW_EXPORT MatrixExpression : public AbstractParentExpression
{
public:
  explicit MatrixExpression(const QSize& size);
  MatrixExpression(int w, int h);

  inline const QSize& size() const { return m_size; }
  void setSize(const QSize& size);

protected:
  // AbstractExpression interface
  void paint(QPainter* painter, int x, int y) const override;
  int calcWidth() const override;
  int calcHeight() const override;
  void fontChanged() override;

private:
  QSize m_size;
  mutable int m_cell_width;
  mutable int m_cell_height;

  int cellWidth() const;
  int cellHeight() const;
  void calcCellSize(int& cell_width, int& cell_height) const;
};

} // namespace ExprDraw

#endif // MATRIXEXPRESSION_H

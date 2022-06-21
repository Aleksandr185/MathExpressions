#include "expression_widget.h"

#include <QPainter>

#include "abstract_expression.h"

ExpressionWidget::ExpressionWidget(QWidget *parent)
  : QWidget(parent)
{
  m_expression_alignment.setFlag(Qt::AlignCenter);
  m_alignment.setFlag(Qt::AlignCenter);
}

// public slots

void ExpressionWidget::setVisibleAxisX(bool value)
{
  if (m_show_axis_x != value) {
    m_show_axis_x = value;
    repaint();
  }
}

void ExpressionWidget::setVisibleAxisY(bool value)
{
  if (m_show_axis_y != value) {
    m_show_axis_y = value;
    repaint();
  }
}

void ExpressionWidget::setExpression(ExpressionPtr expression)
{
  if (m_expression != expression) {
    m_expression = expression;

    if (m_expression) {
      m_expression->setPaintDevice(this);
      this->setMinimumHeight(m_expression->height());
      this->setMinimumWidth(m_expression->width());
    }

    repaint();
  }
}

void ExpressionWidget::setExpressionAlignment(Qt::Alignment alignment)
{
  if (m_expression_alignment != alignment) {
    m_expression_alignment = alignment;
    repaint();
  }
}

void ExpressionWidget::setAlignment(Qt::Alignment alignment)
{
  if (m_alignment != alignment) {
    m_alignment = alignment;
    repaint();
  }
}

// protected

void ExpressionWidget::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);

  paintAxiss();
  paintExpression();
}

// private

void ExpressionWidget::paintAxiss()
{
  if (m_show_axis_x || m_show_axis_y) {

    QPainter painter;
    painter.begin(this);
    painter.setPen( Qt::red );

    const int my_width = width();
    const int half_width = my_width / 2;

    const int my_height = height();
    const int half_height = my_height / 2;

    if (m_show_axis_x) {
      painter.drawLine(half_width, 0,
                       half_width, my_height);
    }

    if (m_show_axis_y) {
      painter.drawLine(0, half_height,
                       my_width, half_height);
    }
  }
}

void ExpressionWidget::paintExpression()
{
  if (m_expression) {
    int x = 0;
    if (m_alignment.testFlag(Qt::AlignHCenter))
      x = width()/2;
    else if (m_alignment.testFlag(Qt::AlignRight))
      x = width();

    int y = 0;
    if (m_alignment.testFlag(Qt::AlignVCenter))
      y = height()/2;
    else if (m_alignment.testFlag(Qt::AlignBottom))
      y = height();

    m_expression->draw(x, y, m_expression_alignment);
  }
}

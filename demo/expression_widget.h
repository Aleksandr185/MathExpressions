#ifndef EXPRESSIONWIDGET_H
#define EXPRESSIONWIDGET_H

#include <QtWidgets/QWidget>

namespace MathExpressions {
  class AbstractExpression;
}

typedef MathExpressions::AbstractExpression* ExpressionPtr;

class ExpressionWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ExpressionWidget(QWidget *parent = nullptr);

public slots:
  void setVisibleAxisX(bool value);
  void setVisibleAxisY(bool value);
  void setExpression(ExpressionPtr );
  void setExpressionAlignment(Qt::Alignment alignment);
  void setAlignment(Qt::Alignment alignment);

protected:
  void paintEvent(QPaintEvent *event);

private:
  bool m_show_axis_x = false;
  bool m_show_axis_y = false;

  ExpressionPtr m_expression = nullptr;
  Qt::Alignment m_expression_alignment;
  Qt::Alignment m_alignment;

  void paintAxiss();
  void paintExpression();
};

#endif // EXPRESSIONWIDGET_H

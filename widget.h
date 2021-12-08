#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>

class QLineEdit;
class QFontComboBox;
class QSpinBox;
class QCheckBox;
class QButtonGroup;

class ExpressionWidget;
namespace ExprDraw {
  class AbstractExpression;
}
typedef ExprDraw::AbstractExpression* ExpressionPtr;

class Widget : public QWidget
{
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget() override;

private:
  QLineEdit*     m_lineEdit;
  QFontComboBox* m_fontName;
  QSpinBox*      m_fontSize;
  QButtonGroup*  m_aling_buttons;
  ExpressionWidget* m_expression_widget;

  ExpressionPtr m_expression = nullptr;

private slots:
  void _q_font_size_changed(int );
  void _q_font_name_changed(const QString& );
  void _q_align_changed(int );
};
#endif // WIDGET_H

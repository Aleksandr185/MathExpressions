#ifndef WIDGETDEMO_H
#define WIDGETDEMO_H

#include <QtWidgets/QWidget>

class QLineEdit;
class QFontComboBox;
class QSpinBox;
class QCheckBox;
class QButtonGroup;
class QCheckBox;


class ExpressionWidget;
namespace MathExpressions {
  class AbstractExpression;
}
typedef MathExpressions::AbstractExpression* ExpressionPtr;

class WidgetDemo : public QWidget
{
  Q_OBJECT

public:
  WidgetDemo(QWidget *parent = nullptr);
  ~WidgetDemo() override;

signals:
  void errorMessage(const QString&);

private:
  QLineEdit*     m_line_edit;
  QFontComboBox* m_font_name;
  QSpinBox*      m_font_size;
  QButtonGroup*  m_aling_buttons;
  QCheckBox*     m_checkbox_variable_auto_index;
  QCheckBox*     m_checkbox_function_auto_index;
  QCheckBox*     m_checkbox_post_symbols;
  QCheckBox*     m_checkbox_wrap_to_frame;

  ExpressionWidget* m_expression_widget;

  std::unique_ptr<MathExpressions::AbstractExpression> m_expression;

private slots:
  void _q_font_size_changed(int );
  void _q_font_name_changed(const QString& );
  void _q_align_changed(int );
  void _q_text_changed(const QString& );
  void _q_builder_flag_changed(bool);
};
#endif // WIDGETDEMO_H

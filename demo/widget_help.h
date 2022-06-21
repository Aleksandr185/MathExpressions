#ifndef WIDGETHELP_H
#define WIDGETHELP_H

#include <QtWidgets/QWidget>

class QListWidget;
class QListWidgetItem;
class QPlainTextEdit;

class ExpressionWidget;
namespace MathExpressions {
  class AbstractExpression;
}
typedef MathExpressions::AbstractExpression* ExpressionPtr;

class WidgetHelp : public QWidget
{
  Q_OBJECT
public:
  explicit WidgetHelp(QWidget *parent = nullptr);
  ~WidgetHelp() override;

signals:

private:
  QListWidget*      m_list_contents;
  QPlainTextEdit*   m_description;
  ExpressionWidget* m_examples;
  std::unique_ptr<MathExpressions::AbstractExpression> m_expression;

  struct HelpSection
  {
    QString title;
    QString description;
    QStringList examples;
  };
  QList<HelpSection> m_help_list;

  void initHelpList();

private slots:
  void _q_current_content_changed(QListWidgetItem *current, QListWidgetItem *previous);
};

#endif // WIDGETHELP_H

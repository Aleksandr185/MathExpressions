#include "widget_demo.h"

#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QFontComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QButtonGroup>
#include <QGroupBox>
#include <QFrame>

#include <QDebug>

#include "expression_widget.h"
#include "expression_builder.h"

WidgetDemo::WidgetDemo(QWidget *parent)
  : QWidget(parent)
{
  // Font
  m_font_name = new QFontComboBox(this);
  m_font_name->setCurrentText("MV Boli");
  m_font_size = new QSpinBox(this);
  m_font_size->setMaximumWidth(50);
  m_font_size->setMinimum(8);
  m_font_size->setMaximum(200);
  m_font_size->setValue(18);
  QHBoxLayout* lay_font = new QHBoxLayout();
  lay_font->addWidget(m_font_name);
  lay_font->addWidget(m_font_size);

  // * CheckX, CheckY
  QCheckBox* axisX = new QCheckBox(this);
  axisX->setText(tr("axis X"));
  QCheckBox* axisY = new QCheckBox(this);
  axisY->setText(tr("axis Y"));
  QHBoxLayout* lay_axis = new QHBoxLayout();
  lay_axis->addWidget(axisX);
  lay_axis->addWidget(axisY);
  lay_axis->addStretch(1);

  //
  m_checkbox_variable_auto_index = new QCheckBox(tr("variable auto index"), this);
  m_checkbox_variable_auto_index->setChecked(true);
  m_checkbox_function_auto_index = new QCheckBox(tr("function auto index"), this);
  m_checkbox_function_auto_index->setChecked(true);
  m_checkbox_post_symbols = new QCheckBox(tr("post symbols"), this);
  m_checkbox_post_symbols->setChecked(true);
  m_checkbox_wrap_to_frame = new QCheckBox(tr("wrap empty expression to frame"), this);
  m_checkbox_wrap_to_frame->setChecked(true);
  QVBoxLayout* lay_checkboxs = new QVBoxLayout();
  lay_checkboxs->addWidget(m_checkbox_variable_auto_index);
  lay_checkboxs->addWidget(m_checkbox_function_auto_index);
  lay_checkboxs->addWidget(m_checkbox_post_symbols);
  lay_checkboxs->addWidget(m_checkbox_wrap_to_frame);

  //   QButtonGroup
  m_aling_buttons = new QButtonGroup(this);
  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignLeft | Qt::AlignTop);
  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignHCenter | Qt::AlignTop);
  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignRight | Qt::AlignTop);

  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignLeft | Qt::AlignVCenter);
  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignCenter);
  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignRight | Qt::AlignVCenter);

  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignLeft | Qt::AlignBottom);
  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignHCenter | Qt::AlignBottom);
  m_aling_buttons->addButton(new QToolButton(this), Qt::AlignRight | Qt::AlignBottom);

  QList<QAbstractButton*> buttons_list = m_aling_buttons->buttons();
  for (int i = 0; i < buttons_list.size(); ++i) {
    buttons_list.at(i)->setCheckable(true);
  }

  QGridLayout* lay_buttons = new QGridLayout();
  lay_buttons->setMargin(5);
  lay_buttons->setSpacing(3);

  QAbstractButton* button = m_aling_buttons->button(Qt::AlignLeft | Qt::AlignTop);
  button->setText(QStringLiteral("↖"));
  lay_buttons->addWidget(button,0,0);
  button = m_aling_buttons->button(Qt::AlignHCenter | Qt::AlignTop);
  button->setText(QStringLiteral("↥"));
  lay_buttons->addWidget(button,0,1);
  button = m_aling_buttons->button(Qt::AlignRight | Qt::AlignTop);
  button->setText(QStringLiteral("↗"));
  lay_buttons->addWidget(button,0,2);

  button = m_aling_buttons->button(Qt::AlignLeft | Qt::AlignVCenter);
  button->setText(QStringLiteral("↤"));
  lay_buttons->addWidget(button,1,0);
  button = m_aling_buttons->button(Qt::AlignCenter);
  button->setText(QStringLiteral("┼"));
  lay_buttons->addWidget(button,1,1);
  button = m_aling_buttons->button(Qt::AlignRight | Qt::AlignVCenter);
  button->setText(QStringLiteral("↦"));
  lay_buttons->addWidget(button,1,2);

  button = m_aling_buttons->button(Qt::AlignLeft | Qt::AlignBottom);
  button->setText(QStringLiteral("↙"));
  lay_buttons->addWidget(button,2,0);
  button = m_aling_buttons->button(Qt::AlignHCenter | Qt::AlignBottom);
  button->setText(QStringLiteral("↧"));
  lay_buttons->addWidget(button,2,1);
  button = m_aling_buttons->button(Qt::AlignRight | Qt::AlignBottom);
  button->setText(QStringLiteral("↘"));
  lay_buttons->addWidget(button,2,2);

  QVBoxLayout* lay_rigth = new QVBoxLayout();
  lay_rigth->setMargin(5);
  lay_rigth->setSpacing(3);
  lay_rigth->addLayout(lay_font);
  lay_rigth->addLayout(lay_axis);
  lay_rigth->addLayout(lay_checkboxs);
  lay_rigth->addStretch(1);
  lay_rigth->addLayout(lay_buttons);
  lay_rigth->addStretch(100);
  lay_rigth->setSizeConstraint(QLayout::SetMinimumSize);


  m_line_edit = new QLineEdit(this);
  m_expression_widget = new ExpressionWidget();
  m_expression_widget->setAlignment(Qt::AlignCenter);

  QVBoxLayout* lay_left = new QVBoxLayout();
  lay_left->addWidget(m_line_edit);
  lay_left->addWidget(m_expression_widget);
  lay_left->setMargin(5);
  lay_left->setSpacing(3);

  QFrame* v_line = new QFrame(this);
  v_line->setFrameShape(QFrame::VLine);
  v_line->setFrameShadow(QFrame::Sunken);
  v_line->setLineWidth(1);

  QHBoxLayout* lay_this = new QHBoxLayout();
  lay_this->addLayout(lay_left, 10);
  lay_this->addWidget(v_line);
  lay_this->addLayout(lay_rigth);

  this->setLayout(lay_this);

  // Signals & Slots
  connect(m_line_edit, &QLineEdit::textChanged, this, &WidgetDemo::_q_text_changed);
  connect(axisX, &QCheckBox::clicked, m_expression_widget, &ExpressionWidget::setVisibleAxisX);
  connect(axisY, &QCheckBox::clicked, m_expression_widget, &ExpressionWidget::setVisibleAxisY);
  connect(m_font_name, &QFontComboBox::currentTextChanged, this, &WidgetDemo::_q_font_name_changed);
  connect(m_font_size, SIGNAL(valueChanged(int)), this, SLOT(_q_font_size_changed(int)));
  connect(m_aling_buttons, SIGNAL(buttonClicked(int)), this, SLOT(_q_align_changed(int)));

  connect(m_checkbox_variable_auto_index, &QCheckBox::clicked,
          this, &WidgetDemo::_q_builder_flag_changed);
  connect(m_checkbox_function_auto_index, &QCheckBox::clicked,
          this, &WidgetDemo::_q_builder_flag_changed);
  connect(m_checkbox_post_symbols, &QCheckBox::clicked,
          this, &WidgetDemo::_q_builder_flag_changed);
  connect(m_checkbox_wrap_to_frame, &QCheckBox::clicked,
          this, &WidgetDemo::_q_builder_flag_changed);

}

WidgetDemo::~WidgetDemo()
{
  m_expression_widget->setExpression(nullptr);
}

void WidgetDemo::_q_font_size_changed(int size)
{
  if (m_expression) {
    QFont font = m_expression->font();
    font.setPointSize(size);
    m_expression->setFont(font);

    m_expression_widget->repaint();
  }
}

void WidgetDemo::_q_font_name_changed(const QString& name)
{
  if (m_expression) {
    QFont font = m_expression->font();
    font.setFamily(name);
    m_expression->setFont(font);

    m_expression_widget->repaint();
  }
}

void WidgetDemo::_q_align_changed(int align)
{
  QAbstractButton* button = m_aling_buttons->button(align);
  if (button && button->isChecked()) {
    m_expression_widget->setExpressionAlignment( Qt::Alignment(align));
  }
}

void WidgetDemo::_q_text_changed(const QString & text)
{
  MathExpressions::ExpressionBuilder builder;
  builder.variableAutoIndex(m_checkbox_variable_auto_index->isChecked());

  builder.functionAutoIndex(m_checkbox_function_auto_index->isChecked());
  builder.postSymbols(m_checkbox_post_symbols->isChecked());
  builder.wrapEmptyExpressionToFrame(m_checkbox_wrap_to_frame->isChecked());

  m_expression = builder.exec(text);
  QFont font;
  font.setFamily(m_font_name->currentText());
  font.setPointSize(m_font_size->value());
  m_expression->setFont(font);

  m_expression_widget->setExpression(m_expression.get());

  if (builder.lastError().isValid()) {
    emit errorMessage(builder.lastError().text());

    qDebug() << QString("Error(%1): %2").arg(builder.lastError().number()).arg(builder.lastError().text());
    qDebug() << text;
    QString str;
    str.reserve(builder.lastError().position());
    for (int i = 1; i < builder.lastError().position(); ++i) {
      str.append(' ');
    }
    str.append('^');
    qDebug() << str;
  }
  else {
    qDebug() << "No error";
    qDebug() << text;
  }
  qDebug() << "************************************************************";
  qDebug() << "";
}

void WidgetDemo::_q_builder_flag_changed(bool)
{
  _q_text_changed(m_line_edit->text());
}

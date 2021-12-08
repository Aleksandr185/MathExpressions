#include "widget.h"

#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QFontComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QButtonGroup>

#include "expression_widget.h"

#include "src/chain_expression.h"
#include "src/variable_expression.h"
#include "src/cap_expression.h"

enum AligmentRole{
  LeftTop = 0,
  CenterTop,
  RightTop,
  LeftCenter,
  CenterCenter,
  RightCenter,
  LeftBottom,
  CenterBottom,
  RightBottom
};

Widget::Widget(QWidget *parent)
  : QWidget(parent)
{
  // Font
  QHBoxLayout* lay_font = new QHBoxLayout();
  m_fontName = new QFontComboBox(this);
  m_fontSize = new QSpinBox(this);
  m_fontSize->setMaximumWidth(50);
  m_fontSize->setMinimum(8);
  m_fontSize->setMaximum(200);
  m_fontSize->setValue(18);
  lay_font->addWidget(m_fontName);
  lay_font->addWidget(m_fontSize);

  // * CheckX, CheckY
  QHBoxLayout* lay_axis = new QHBoxLayout();
  QCheckBox* axisX = new QCheckBox(this);
  axisX->setText(QStringLiteral("ось X"));
  QCheckBox* axisY = new QCheckBox(this);
  axisY->setText(QStringLiteral("ось Y"));
  lay_axis->addWidget(axisX);
  lay_axis->addWidget(axisY);
  lay_axis->addStretch(1);

  QVBoxLayout* lay_rigth = new QVBoxLayout();
  lay_rigth->setMargin(5);
  lay_rigth->setSpacing(3);
  lay_rigth->addLayout(lay_font);
  lay_rigth->addLayout(lay_axis);

  //   QButtonGroup
  m_aling_buttons = new QButtonGroup(this);
  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::LeftTop);
  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::CenterTop);
  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::RightTop);

  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::LeftCenter);
  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::CenterCenter);
  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::RightCenter);

  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::LeftBottom);
  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::CenterBottom);
  m_aling_buttons->addButton(new QToolButton(this), AligmentRole::RightBottom);

  for (int i = 0; i < 9; ++i) {
    QAbstractButton* T = m_aling_buttons->button(i);
    T->setCheckable(true);
  }

  QGridLayout* lay_buttons = new QGridLayout();
  lay_buttons->setMargin(5);
  lay_buttons->setSpacing(3);

  QAbstractButton* button = m_aling_buttons->button(AligmentRole::LeftTop);
  button->setText(QStringLiteral("↖"));
  lay_buttons->addWidget(button,0,0);
  button = m_aling_buttons->button(AligmentRole::CenterTop);
  button->setText(QStringLiteral("↥"));
  lay_buttons->addWidget(button,0,1);
  button = m_aling_buttons->button(AligmentRole::RightTop);
  button->setText(QStringLiteral("↗"));
  lay_buttons->addWidget(button,0,2);

  button = m_aling_buttons->button(AligmentRole::LeftCenter);
  button->setText(QStringLiteral("↤"));
  lay_buttons->addWidget(button,1,0);
  button = m_aling_buttons->button(AligmentRole::CenterCenter);
  button->setText(QStringLiteral("┼"));
  lay_buttons->addWidget(button,1,1);
  button = m_aling_buttons->button(AligmentRole::RightCenter);
  button->setText(QStringLiteral("↦"));
  lay_buttons->addWidget(button,1,2);

  button = m_aling_buttons->button(AligmentRole::LeftBottom);
  button->setText(QStringLiteral("↙"));
  lay_buttons->addWidget(button,2,0);
  button = m_aling_buttons->button(AligmentRole::CenterBottom);
  button->setText(QStringLiteral("↧"));
  lay_buttons->addWidget(button,2,1);
  button = m_aling_buttons->button(AligmentRole::RightBottom);
  button->setText(QStringLiteral("↘"));
  lay_buttons->addWidget(button,2,2);

//  phLay = new QHBoxLayout();
//  phLay->addStretch(1);
//  phLay->addLayout(lay_buttons);
//  phLay->addStretch(1);

  lay_rigth->addStretch(1);
  lay_rigth->addLayout(lay_buttons);
  lay_rigth->addStretch(1);
  lay_rigth->setSizeConstraint(QLayout::SetMinimumSize);


  m_lineEdit = new QLineEdit(this);
  m_expression_widget = new ExpressionWidget();

  QVBoxLayout* lay_left = new QVBoxLayout();
  lay_left->addWidget(m_lineEdit);
  lay_left->addWidget(m_expression_widget);
  lay_left->setMargin(5);
  lay_left->setSpacing(3);

  QHBoxLayout* my_lay = new QHBoxLayout();
  my_lay->addLayout(lay_left, 10);
  my_lay->addLayout(lay_rigth);

  this->setLayout(my_lay);
  this->setMinimumSize(550, 400);

  connect(axisX, &QCheckBox::clicked, m_expression_widget, &ExpressionWidget::setVisibleAxisX);
  connect(axisY, &QCheckBox::clicked, m_expression_widget, &ExpressionWidget::setVisibleAxisY);
  connect(m_fontName, &QFontComboBox::currentTextChanged, this, &Widget::_q_font_name_changed);
  connect(m_fontSize, SIGNAL(valueChanged(int)), this, SLOT(_q_font_size_changed(int)));
  connect(m_aling_buttons, SIGNAL(buttonClicked(int)), this, SLOT(_q_align_changed(int)));


  // TEMP
  using namespace ExprDraw;

  AbstractExpression* expr = new CapExpression(new VariableExpression('a'), CapExpression::CapStyle::Vector);
  expr->addNext(new CapExpression(new VariableExpression('A'), CapExpression::CapStyle::Line));
  expr->addNext(new CapExpression(new VariableExpression('.'), CapExpression::CapStyle::Tilde));
  CapExpression* points = new CapExpression(new VariableExpression('d'), CapExpression::CapStyle::Points);
  points->setPointCount(3);
  expr->addNext(points);
  expr->addNext(new CapExpression(new VariableExpression('y'), CapExpression::CapStyle::Cap));

  ChainExpression* chain = new ChainExpression(expr);

  chain->setFont(QFont(m_fontName->currentText(), m_fontSize->value()));

  m_expression = chain;
  m_expression_widget->setExpression(m_expression);

  button = m_aling_buttons->button(AligmentRole::CenterCenter);
  button->setChecked(true);
}

Widget::~Widget()
{
  m_expression_widget->setExpression(nullptr);
  delete m_expression;
}

void Widget::_q_font_size_changed(int size)
{
  if (m_expression) {
    QFont font = m_expression->font();
    font.setPointSize(size);
    m_expression->setFont(font);

    m_expression_widget->repaint();
  }
}

void Widget::_q_font_name_changed(const QString& name)
{
  if (m_expression) {
    QFont font = m_expression->font();
    font.setFamily(name);
    m_expression->setFont(font);

    m_expression_widget->repaint();
  }
}

void Widget::_q_align_changed(int align)
{
  QAbstractButton* button = m_aling_buttons->button(align);
  if (button && button->isChecked()) {
    Qt::Alignment align_flags;

    if (align == LeftTop || align == LeftCenter || align == LeftBottom) {
      align_flags.setFlag(Qt::AlignLeft);
    }
    else if (align == RightTop || align == RightCenter || align == RightBottom) {
      align_flags.setFlag(Qt::AlignRight);
    }
    else {
      align_flags.setFlag(Qt::AlignHCenter);
    }

    if (align == LeftTop || align == CenterTop || align == RightTop) {
      align_flags.setFlag(Qt::AlignTop);
    }
    else if (align == LeftBottom || align == CenterBottom || align == RightBottom) {
      align_flags.setFlag(Qt::AlignBottom);
    }
    else {
      align_flags.setFlag(Qt::AlignVCenter);
    }

    m_expression_widget->setExpressionAlignment(align_flags);
  }
}



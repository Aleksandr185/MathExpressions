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
#include <QDoubleSpinBox>

#include "expression_widget.h"

#include "src/chain_expression.h"
#include "src/variable_expression.h"
#include "src/cap_expression.h"
#include "src/fraction_expression.h"
#include "src/extended_number_expresssion.h"
#include "src/character_expression.h"
#include "src/planck_expression.h"
#include "src/comma_expression.h"
#include "src/lambda_expression.h"
#include "src/nabla_expression.h"
#include "src/space_expression.h"
#include "src/strokes_expression.h"
#include "src/simple_expression.h"
#include "src/empty_expression.h"
#include "src/root_expression.h"
#include "src/at_value_expression.h"
#include "src/common_function_expression.h"
#include "src/function_expression.h"
#include "src/bracketed_expression.h"
#include "src/index_expression.h"
#include "src/group_expression.h"
#include "src/integral_expression.h"
#include "src/limit_expression.h"
#include "src/stand_expression.h"
#include "src/sign_expression.h"
#include "src/frame_expression.h"

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
  m_fontName->setCurrentText("MV Boli");
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

//  AbstractExpression* expr = new CapExpression(new VariableExpression('a'), CapExpression::CapStyle::Vector);
//  expr->addNext(new CapExpression(new VariableExpression('A'), CapExpression::CapStyle::Line));
//  expr->addNext(new CapExpression(new VariableExpression('.'), CapExpression::CapStyle::Tilde));
//  CapExpression* points = new CapExpression(new VariableExpression('d'), CapExpression::CapStyle::Points);
//  points->setPointCount(3);
//  expr->addNext(points);
//  expr->addNext(new CapExpression(new VariableExpression('y'), CapExpression::CapStyle::Cap));

  // Fraction
  FractionExpression* fraction = new FractionExpression();
  fraction->setSon(new VariableExpression('x'));
  fraction->setDaughter(new VariableExpression('y'));

//  expr->addNext(fraction);

  // Number
  ExtendedNumberExpresssion* expr_num = new ExtendedNumberExpresssion();
  //expr_num->setStyle(NumberExpresssion::Style::Exponential);
  expr_num->setNumber(0.00000000001/3);
  expr_num->addNext(new PlanckExpression());
  expr_num->addNext(new SignExpression(SignExpression::PlusMinus));
  expr_num->addNext(new SignExpression(SignExpression::MinusPlus));
  expr_num->addNext(new CharacterExpression(0x2213));
  FrameExpression* frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::Parallel));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::Perpendicular));
  expr_num->addNext(frame);

  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::Angle));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::IdenticalTo));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::Less));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::LessOrEqual));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::MuchLess));
  expr_num->addNext( frame );
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::MuchGreater));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::ApproxLess));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::ApproxGreater));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new SignExpression(SignExpression::AlmostEqual));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new CharacterExpression(0x226a));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new CharacterExpression(0x2272));
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(new CharacterExpression(0x2243));
  expr_num->addNext(frame);


  StrokesExpression* strokes = new StrokesExpression(1);
  //fraction->setDaughter(strokes);

  RootExpression* root = new RootExpression();
  root->setSon(new VariableExpression('x'));

  frame = new FrameExpression();
  frame->setSon(root);
  expr_num->addNext(frame);
  frame = new FrameExpression();
  frame->setSon(strokes);
  expr_num->addNext(frame);

  frame = new FrameExpression();
  frame->setSon(new NablaExpression());
  expr_num->addNext(frame);

  frame = new FrameExpression();
  frame->setSon(new FunctionExpression("(f)", new VariableExpression('x')));
  expr_num->addNext(frame);

  AtValueExpression* at_value = new AtValueExpression();
  at_value->setSon(fraction);
  at_value->setDaughter(new SimpleExpression("x = 0"));
  frame = new FrameExpression();
  frame->setSon(at_value);
  expr_num->addNext(frame);

  IndexExpression* index = new IndexExpression();
  index->setSon(new VariableExpression('H'));
  index->setFirstTwin(new SimpleExpression('2'));
  index->setSeconsTwin(new SimpleExpression('3'));
  frame = new FrameExpression();
  frame->setSon(index);
  expr_num->addNext(frame);

  IntegralExpression* summa = new IntegralExpression();
  summa->setFirstTwin(new VariableExpression('a'));
  summa->setSeconsTwin(new VariableExpression('t'));
  frame = new FrameExpression();
  frame->setSon(summa);
  expr_num->addNext(frame);

  LimitExpression* lim = new LimitExpression();
  lim->setSon(new SimpleExpression("x = 0"));
  frame = new FrameExpression();
  frame->setSon(lim);
  expr_num->addNext(frame);

  ChainExpression* chain = new ChainExpression();
  chain->setSon(expr_num);

  QDoubleSpinBox* double_spin = new QDoubleSpinBox();
  double_spin->setValue(expr_num->number());
  double_spin->setMinimum(-99999999.0);
  double_spin->setMaximum(9999999.0);
  double_spin->setDecimals(14);
  QCheckBox* check_exponent = new QCheckBox("Exponential style");

  QSpinBox* spin_precision = new QSpinBox();
  spin_precision->setMinimum(-10);
  spin_precision->setMaximum(100);
  spin_precision->setValue(expr_num->precision());

  QSpinBox* spin_decimals = new QSpinBox();
  spin_decimals->setMinimum(-10);
  spin_decimals->setMaximum(100);
  spin_decimals->setValue(expr_num->decimals());

  QSpinBox* spin_degree = new QSpinBox();
  spin_degree->setMinimum(-100);
  spin_degree->setMaximum(100);
  //spin_degree->setValue(expr_num->maxDegree());
  spin_degree->setValue(strokes->count());

  QVBoxLayout* lay_number = new QVBoxLayout();
  lay_number->addWidget(double_spin);
  lay_number->addWidget(check_exponent);
  lay_number->addWidget(spin_precision);
  lay_number->addWidget(spin_decimals);
  lay_number->addWidget(spin_degree);
  lay_rigth->addLayout(lay_number);

  connect(double_spin, static_cast<void (QDoubleSpinBox::*)(double)>( &QDoubleSpinBox::valueChanged),
          [=](double v)
  {
    expr_num->setNumber(v);
    m_expression_widget->repaint();
  });

  connect(check_exponent, &QCheckBox::stateChanged,
          [=](int v)
  {
    expr_num->setStyle(v == Qt::Checked ? ExprDraw::NumberExpresssion::Style::Exponential
                                        : ExprDraw::NumberExpresssion::Style::Regular);
    m_expression_widget->repaint();
  });

  connect(spin_precision, static_cast<void (QSpinBox::*)(int)>( &QSpinBox::valueChanged),
          [=](int v)
  {
    expr_num->setPrecision(v);
    m_expression_widget->repaint();
  });

  connect(spin_decimals, static_cast<void (QSpinBox::*)(int)>( &QSpinBox::valueChanged),
          [=](int v)
  {
    expr_num->setDecimals(v);
    m_expression_widget->repaint();
  });

  connect(spin_degree, static_cast<void (QSpinBox::*)(int)>( &QSpinBox::valueChanged),
          [=](int v)
  {
    //expr_num->setMaxDegree(v);
    strokes->setCount(v);
    m_expression_widget->repaint();
  });


  m_expression = chain;
  m_expression->setFont(QFont(m_fontName->currentText(), m_fontSize->value()));
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


// TODO: нарисуй знак корня как в 'Times News Roman'

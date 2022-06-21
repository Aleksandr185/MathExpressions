#include "widget_help.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include <QScrollArea>
#include <QSplitter>
#include <QHBoxLayout>

#include "expression_widget.h"
#include "expression_builder.h"
#include "chain_expression.h"
#include "stand_expression.h"
#include "variable_expression.h"
#include "simple_expression.h"


WidgetHelp::WidgetHelp(QWidget *parent)
  : QWidget{parent}, m_expression(nullptr)
{
  m_list_contents = new QListWidget(this);
  m_description = new QPlainTextEdit(this);
  m_description->setReadOnly(true);
  m_examples = new ExpressionWidget(this);
  m_examples->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  m_examples->setExpressionAlignment(Qt::AlignRight | Qt::AlignBottom);

  QScrollArea* examples_scroll_area = new QScrollArea(this);
  examples_scroll_area->setWidget(m_examples);

  QSplitter* splitter_description_examples = new QSplitter(Qt::Vertical, this);
  splitter_description_examples->addWidget(m_description);
  splitter_description_examples->addWidget(examples_scroll_area);
  splitter_description_examples->setCollapsible(0, false);
  splitter_description_examples->setCollapsible(1, false);

  QSplitter* splitter_contents = new QSplitter(Qt::Horizontal, this);
  splitter_contents->addWidget(m_list_contents);
  splitter_contents->addWidget(splitter_description_examples);
  splitter_contents->setCollapsible(0, false);
  splitter_contents->setCollapsible(1, false);
  splitter_contents->setStretchFactor(0, 0);
  splitter_contents->setStretchFactor(1, 100);
  splitter_contents->setSizes( QList<int>{250, 300} );

  QHBoxLayout* lay_this = new QHBoxLayout();
  lay_this->addWidget(splitter_contents);
  this->setLayout(lay_this);

  initHelpList();

  for (int i = 0; i < m_help_list.size(); ++i) {
    QListWidgetItem* item = new QListWidgetItem(m_list_contents);
    item->setText(m_help_list.at(i).title);
    item->setToolTip(m_help_list.at(i).description);
  }

  // Signals & Slots
  connect(m_list_contents, &QListWidget::currentItemChanged,
          this, &WidgetHelp::_q_current_content_changed);

}

WidgetHelp::~WidgetHelp()
{

}

void WidgetHelp::initHelpList()
{
  {
    HelpSection section;
    section.title = tr("Простые выражения");
    section.description = tr("К простым выражениям относятся идентификаторы, токены и цифровые константы. "
                             "Токены - это зарезервированные слова, использующиеся для обозначения специальных символов (пример 1). "
                             "Регистр символов при написании токенов не важен за исключением токенов, обозначающих греческие буквы. "
                             "Если первый символ такого токена имеет верхний регистр, то получается заглавная буква, если нижний - строчная. "
                             "Идентификаторы воспроизводятся так, как они написаны в тексте. Идентификаторы могут состоять из английских и "
                             "русских букв и цифр, причём начинаться идентификатор должен с буквы. Если идентификатор начинается с английской "
                             "буквы, он выводится наклонным шрифтом (пример 2), если с русской - прямым (пример 3). Вывести английский текст "
                             "прямым шрифтом можно с помощью функции String. Если в конце идентификатора стоят цифры, а "
                             "ExpressionBuilder::variableAutoIndex() == true (по умолчанию), то они рассматриваются как нижний индекс (пример 4). "
                             "Если цифры стоят после токена, то они также рассматриваются как нижний индекс (пример 5). "
                             "Цифровые константы записываются в обычном для Паскаля синтаксисе (пример 6). Допускается использование английской "
                             "'e' или 'E' для обозначения экспоненциальной части числа. Оптимальный формат вывода числа подбирается автоматически. "
                             "Если он по каким-то причинам не устраивает, можно воспользоваться символом '#' или функцией Num.");
    section.examples << QStringLiteral("Alpha") << QStringLiteral("alpha")
                     << QStringLiteral("EnglEnc") << QStringLiteral("РУС")
                     << QStringLiteral("x12") << QStringLiteral("beta0");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Выражения");
    section.description = tr("Выражения состоят из операндов, разделённых знаками сравнения, операнды - из слагаемых, "
                             "разделённых знаками сложения и вычитания, слагаемые - из множителей, разделённых знаками "
                             "умножения и деления. \n"
                             "В примере 1 'a' и 'b+c/d' - операнды, 'b' и 'c/d' - слагаемые, 'c' и 'd' - множители. "
                             "Множители могут быть одного из следующих видов: \n"
                             "1. Числовая константа (в том числе начинающаяся с символа '#'), идентификатор или токен. \n"
                             "2. Выражение, заключенное в круглые, квадратные, прямые или фигурные скобки. \n"
                             "3. Функция. \n"
                             "Перед множителем может стоять символ '_', обозначающий вектор, и/или символы '+', '-', "
                             "'+-', '-+', обозначающие соответсвующие унарные операции. "
                             "Если ExpressionBuilder::postSymbols() == true (по умолчанию), после множителя могут стоять "
                             "символы '_', '^', '!' или '`'. Несколько выражений могут склеиваться в одно при помощи "
                             "символа '&' или запятой.");
    section.examples  << QStringLiteral("a=b+c/d")
                      << QStringLiteral("B_(2*k)=((-1)^(n-1)**2*(2*k)!/(2*pi)^(2*k))*summa(1/n^(2*k),n=1,inf)")
                      << QStringLiteral("line(u_i^2)=(2*plank^2/M/k/Theta)*((T^2/Theta^2)*int(x*diff(x)/(e^x-1),0,T//Theta)+1/4)")
                      << QStringLiteral("a0=(r2^2*(r2-3*r1)*V1(r1)-r1^2*(r1-3*r2)*V3(r2)-(r2*strokes(V1(r1))+r1*strokes(V3(r2)))*r1*r2*(r2-r1))/(r2-r1)^3")
                      << QStringLiteral("int(phi*cap(p)_x*psi*diff(x))=-i*plank*int(phi*pdiffrf(psi,x)*diff(x))=i*plank*int(psi*pdiffrf(phi,x)*diff(x))=int(psi*cap(p)_x^asterix*phi*diff(x))")
                      << QStringLiteral("grad(U)=lim(v->0,circ(U*diff(_S),Sigma)/v) & space(15) & div(_V)=lim(v->0,int(_V*diff(_S),Sigma)/v) & space(15) & rot(_V)=-lim(v->0,int(_V*+d*_S,Sigma)/v)")
                      << QStringLiteral("P(x)=summa(prod((x-x_j)/(x_i-x_j),standc(j=0,j<>i),n),i=0,n)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функции");
    section.description = tr("Функцией считается текст, после которого в скобках стоит один или несколько аргументов. \n"
                             "Существуют зарезервированные имена функций, которые используются для специальных целей. \n"
                             "Все зарезервированные функции описаны в данном справочнике. \n"
                             "При описании используется следующие условные обозначения: \n"
                             "\t E, E1, E2 и т. д. - произвольные выражения, \n"
                             "\t m, n, n1, n2 и т. д. - целочисленные константы, \n"
                             "\t R - вещественная константа. \n"
                             "Имена зарезервированных функций нечувствительны к регистру символов. Если имя функции не "
                             "является зарезервированным словом, результат зависит от длины имени. Имена функций, состоящие "
                             "из одного символа, выводятся курсивом, а их аргументы всегда заключаются в круглые скобки (пример 1). \n"
                             "Более длинные имена функций выводятся прямым шрифтом, а их аргументы заключаются в скобки "
                             "только при необходимости (пример 2). \n"
                             "Можно принудительно заключить аргумент в скобки, используя вместо открывающей скобки комбинацию '!(' (пример 3). \n"
                             "При возведении функции в степень или присоденении к ней индекса знаки соответсвующих операций должны стоять "
                             "после аргумента (пример 4). \n"
                             "Цифры, стоящие в конце имени функции, интерпретируются как нижний индекс, если "
                             "ExpressionBuilder::functionAutoIndex() == true (по умолчанию; пример 5). \n"
                             "В качестве имён функций можно использовать токены, обозначающие греческие буквы, а также "
                             "токены Nabla и PLambda. В этом случае цифры в конце также интерпретируются как нижний индекс (пример 6).");
    section.examples  << QStringLiteral("f(x,y,z)") << QStringLiteral("cos(x), sin(pi/2+x), tg(1/x)")
                      << QStringLiteral("cos!(x)") << QStringLiteral("f(x)_n,cos(x)^2")
                      << QStringLiteral("f0(x)=g1(x)") << QStringLiteral("gamma0(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Единицы ширины");
    section.description = tr("Единицы ширины, использующиеся в некоторых зарезервированных функциях "
                             "для указания размера пробела, подобраны так, что одна единица ширины "
                             "примерно равна толщине вертикальной линии в символе '+'(на самом деле она равна высоте символа '_').");
    section.examples  << QStringLiteral("a&space(20)&b");

    m_help_list.append(section);
  }

// -------------------------------------------- Tokens ---------------------------------------------
  {
    HelpSection section;
    section.title = tr("Токен ...");
    section.description = tr("Многоточие.");
    section.examples  << QStringLiteral("f(x1,x2,...,x_n)=0");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Alpha");
    section.description = tr("Греческая буква альфа");
    section.examples  << QStringLiteral("Alpha") << QStringLiteral("alpha");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Beta");
    section.description = tr("Греческая буква бета");
    section.examples  << QStringLiteral("Beta") << QStringLiteral("beta");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Gamma");
    section.description = tr("Греческая буква гамма");
    section.examples  << QStringLiteral("Gamma") << QStringLiteral("gamma");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Delta");
    section.description = tr("Греческая буква дельта");
    section.examples  << QStringLiteral("Delta") << QStringLiteral("delta");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Epsilon");
    section.description = tr("Греческая буква эпсилон");
    section.examples  << QStringLiteral("Epsilon") << QStringLiteral("epsilon");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Zeta");
    section.description = tr("Греческая буква зета");
    section.examples  << QStringLiteral("Zeta") << QStringLiteral("zeta");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Eta");
    section.description = tr("Греческая буква ета");
    section.examples  << QStringLiteral("Eta") << QStringLiteral("eta");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Theta");
    section.description = tr("Греческая буква тета");
    section.examples  << QStringLiteral("Theta") << QStringLiteral("theta");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Iota");
    section.description = tr("Греческая буква йота");
    section.examples  << QStringLiteral("Iota") << QStringLiteral("iota");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Kappa");
    section.description = tr("Греческая буква каппа");
    section.examples  << QStringLiteral("Kappa") << QStringLiteral("kappa");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Lambda");
    section.description = tr("Греческая буква лямбда");
    section.examples  << QStringLiteral("Lambda") << QStringLiteral("lambda");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Mu");
    section.description = tr("Греческая буква мю");
    section.examples  << QStringLiteral("Mu") << QStringLiteral("mu");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Nu");
    section.description = tr("Греческая буква ню");
    section.examples  << QStringLiteral("Nu") << QStringLiteral("nu");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Xi");
    section.description = tr("Греческая буква кси");
    section.examples  << QStringLiteral("Xi") << QStringLiteral("xi");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Omicron");
    section.description = tr("Греческая буква омикрон");
    section.examples  << QStringLiteral("Omicron") << QStringLiteral("omicron");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Pi");
    section.description = tr("Греческая буква пи");
    section.examples  << QStringLiteral("Pi") << QStringLiteral("pi");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Rho");
    section.description = tr("Греческая буква ро");
    section.examples  << QStringLiteral("Rho") << QStringLiteral("rho");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Sigma");
    section.description = tr("Греческая буква сигма");
    section.examples  << QStringLiteral("Sigma") << QStringLiteral("sigma");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Tau");
    section.description = tr("Греческая буква тау");
    section.examples  << QStringLiteral("Tau") << QStringLiteral("tau");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Upsilon");
    section.description = tr("Греческая буква упсилон");
    section.examples  << QStringLiteral("Upsilon") << QStringLiteral("upsilon");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Phi");
    section.description = tr("Греческая буква фи");
    section.examples  << QStringLiteral("Phi") << QStringLiteral("phi");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Chi");
    section.description = tr("Греческая буква хи");
    section.examples  << QStringLiteral("Chi") << QStringLiteral("chi");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Psi");
    section.description = tr("Греческая буква пси");
    section.examples  << QStringLiteral("Psi") << QStringLiteral("psi");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Omega");
    section.description = tr("Греческая буква омега");
    section.examples  << QStringLiteral("Omega") << QStringLiteral("omega");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Inf");
    section.description = tr("Символ бесконечности");
    section.examples  << QStringLiteral("lim(x->0,1/x)=+-Inf");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Plank");
    section.description = tr("Постоянная Планка с чертой");
    section.examples  << QStringLiteral("Plank=h/2/pi");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Nil");
    section.description = tr("Пустое выражение, имеющее нулевые размеры. \n"
                             "Используется там, где по синтаксису должно быть выражение, но в конкретном случае требуется, чтобы его не было.");
    section.examples  << QStringLiteral("lim(nil,f(x))=0");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Null");
    section.description = tr("Пустое выражение, имеющее нулевые размеры. \n"
                             "Используется там, где по синтаксису должно быть выражение, но в конкретном случае требуется, чтобы его не было.");
    section.examples  << QStringLiteral("lim(null,f(x))=0");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Empty");
    section.description = tr("Пустое выражение. \n"
                             "В отличие от Nil и Null, имеет только нулевую ширину, а высота равна высоте символов.");
    section.examples  << QStringLiteral("Empty^2");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Comma");
    section.description = tr("Вставляет в текст запятую. \n"
                             "В большинстве случаев может быть заменена символом ',' или функцией Comma.");
    section.examples  << QStringLiteral("x&comma&y");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Const");
    section.description = tr("Слово const, обозначающее произвольную константу. \n"
                             "В отличие от обычных идентификаторов, пишется прямым шрифтом, а не курсивом.");
    section.examples  << QStringLiteral("Int(x*Diff(x))=x^2/2+const");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Asterix");
    section.description = tr("Символ '*', несколько опущенный вниз по сравнению с обычным положением. \n"
                             "Предназначен для использования в верхних индексах.");
    section.examples  << QStringLiteral("Summa(a_n*a_n^Asterix,n)=Int(Psi*Psi^Asterix*Diff(q))");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Minus");
    section.description = tr("Знак 'минус'. Предназначен для использования преимущественно в индексах. \n"
                             "Прочие знаки ('+', '='' и т. п.) можно отобразить с помощью функции String. \n"
                             "Но выражение String(-) даст не минус, а дефис, который существенно короче \n"
                             "(в кодировке Unicode минус и дефис - это разные символы).");
    section.examples  << QStringLiteral("a_Minus<>a_String(-)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Parallel");
    section.description = tr("Знак параллельности.");
    section.examples  << QStringLiteral("_a & Parallel & _b") << QStringLiteral("H_Parallel");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Perpendicular");
    section.description = tr("Знак перпендикулярности.");
    section.examples  << QStringLiteral("_a & Perpendicular & _b") << QStringLiteral("v_Perpendicular");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Angle");
    section.description = tr("Знак угла");
    section.examples  << QStringLiteral("Angle**A");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен PLambda");
    section.description = tr("Лямбда с чертой (используется в квантвой механике).");
    section.examples  << QStringLiteral("PLambda=lambda/2/pi");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Токен Nabla");
    section.description = tr("Символ 'набла' (перевернутая греческая буква дельта)");
    section.examples  << QStringLiteral("Nabla*f=PDiffRF(f,x)*_e_x+PDiffRF(f,y)*_e_y+PDiffRF(f,z)*_e_z")
                      << QStringLiteral("Nabla*_a=PDiffRF(a_x,x)+PDiffRF(a_y,y)+PDiffRF(a_z,z)");
    m_help_list.append(section);
  }

// ----------------------------------------- Brackets ----------------------------------------------

  {
    HelpSection section;
    section.title = tr("Скобки ()");
    section.description = tr("Круглые скобки, служат для изменения порядка выполнения действий. \n"
                             "Могут быть убраны, если это не приведёт к искажению смысла выражения. \n"
                             "Для принудительной установки скобок используйте скобки '!()'.");
    section.examples  << QStringLiteral("(x+1)*(y-2)") << QStringLiteral("(x+1)/(x-1)")
                      << QStringLiteral("a+(b+c)=d*.(e*.f)") << QStringLiteral("a*(b+c)<>d+(e*f)")
                      << QStringLiteral("y=(1+1/(1+1/x))");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Скобки !()");
    section.description = tr("Используются там же, где и обычные круглые скобки, \n"
                             "но никогда не убираются построителем формулы.");
    section.examples  << QStringLiteral("!(x+1)*!(y-2)") << QStringLiteral("!(x+1)/!(x-1)")
                      << QStringLiteral("a+!(b+c)=d*.!(e*.f)")  << QStringLiteral("a*!(b+c)<>d+!(e*f)")
                      << QStringLiteral("y=!(1+1/!(1+1/x))");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Скобки []");
    section.description = tr("Квадратные скобки. \n"
                             "Никогда не убираются построителем.");
    section.examples  << QStringLiteral("[x+1]*[y-2]") << QStringLiteral("[x+1]/[x-1]")
                      << QStringLiteral("a+[b+c]=d*.[e*.f]") << QStringLiteral("a*[b+c]<>d+[e*f]")
                      << QStringLiteral("y=[1+1/[1+1/x]]");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Скобки {}");
    section.description = tr("Фигурные скобки. \n"
                             "Никогда не убираются построителем.");
    section.examples  << QStringLiteral("{x+1}*{y-2}") << QStringLiteral("{x+1}/{x-1}")
                      << QStringLiteral("a+{b+c}=d*.{e*.f}") << QStringLiteral("a*{b+c}<>d+{e*f}")
                      << QStringLiteral("y={1+1/{1+1/x}}");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Скобки ||");
    section.description = tr("Прямые скобки. \n"
                             "Никогда не убираются построителем.");
    section.examples  << QStringLiteral("|x+1|*|y-2|") << QStringLiteral("|x+1|/|x-1|")
                      << QStringLiteral("a+|b+c|=d*.|e*.f|") << QStringLiteral("a*|b+c|<>d+|e*f|")
                      << QStringLiteral("y=|1+1/|1+1/x||");
    m_help_list.append(section);
  }

// ------------------------------------------- Symbols ---------------------------------------------
  {
    HelpSection section;
    section.title = tr("Символ '+'");
    section.description = tr("Символ 'плюс', использующийся обычно для обозначения сложения.");
    section.examples  << QStringLiteral("a+b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '-'");
    section.description = tr("Символ 'минус', использующийся обычно для обозначения вычитания.");
    section.examples  << QStringLiteral("a-b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '*'");
    section.description = tr("Операция умножения. \n"
                             "Проверяет, могут ли сомножители быть перемноженными без знака. \n"
                             "Если могут, то знак умножения не используется (пример 1). \n"
                             "Сомножители могут быть переставлены местами, чтобы обеспечить умножение без символа (пример 2). \n"
                             "Если среди сомножителей несколько чисел, то числа объединяются в одно (пример 3). \n"
                             "Если перемножение без символа невозможно ни при каком порядке множителей, \n"
                             "используется символ точка (пример 4).");
    section.examples  << QStringLiteral("5*x") << QStringLiteral("y*2")
                      << QStringLiteral("2*a*3") << QStringLiteral("sin(x)*cos(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '**'");
    section.description = tr("Операция умножения. \n"
                             "Сомножители перемножаются без знака независимо от того, допустимо ли такое перемножение. \n"
                             "Перестановка множителей не производится.");
    section.examples  << QStringLiteral("5**x") << QStringLiteral("y**2")
                      << QStringLiteral("2**a**3") << QStringLiteral("sin(x)**cos(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '*.'");
    section.description = tr("Операция умножения. \n"
                             "Сомножители перемножаются с помощью знака точки. \n"
                             "Перестановка множителей не производится.");
    section.examples  << QStringLiteral("5*.x") << QStringLiteral("y*.2")
                      << QStringLiteral("2*.a*.3") << QStringLiteral("sin(x)*.cos(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '*+'");
    section.description = tr("Операция умножения. \n"
                             "Сомножители перемножаются с помощью знака косого креста. \n"
                             "Перестановка множителей не производится.");
    section.examples  << QStringLiteral("5*+x") << QStringLiteral("y*+2")
                      << QStringLiteral("2*+a*+3") << QStringLiteral("sin(x)*+cos(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '<'");
    section.description = tr("Знак операции 'меньше'.");
    section.examples  << QStringLiteral("a<b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '<<'");
    section.description = tr("Знак операции 'много меньше'.");
    section.examples  << QStringLiteral("a<<b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '<>'");
    section.description = tr("Знак операции 'не равно'.");
    section.examples  << QStringLiteral("a<>b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '<~'");
    section.description = tr("Знак операции 'меньше или порядка'.");
    section.examples  << QStringLiteral("a<~b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '<='");
    section.description = tr("Знак операции 'меньше или равно'.");
    section.examples  << QStringLiteral("a<=b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '>'");
    section.description = tr("Знак операции 'больше'.");
    section.examples  << QStringLiteral("a>b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '>>'");
    section.description = tr("Знак операции 'много больше'.");
    section.examples  << QStringLiteral("a>>b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '>~'");
    section.description = tr("Знак операции 'больше или порядка'.");
    section.examples  << QStringLiteral("a>~b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '>='");
    section.description = tr("Знак операции 'больше или равно'.");
    section.examples  << QStringLiteral("a>=b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '='");
    section.description = tr("Знак операции 'равно'.");
    section.examples  << QStringLiteral("a=b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '=='");
    section.description = tr("Знак операции 'тождественно'.");
    section.examples  << QStringLiteral("a==b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '=~'");
    section.description = tr("Знак операции 'равно или порядка'.");
    section.examples  << QStringLiteral("a=~b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '~'");
    section.description = tr("Знак операции 'порядка'.");
    section.examples  << QStringLiteral("a~b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '~~'");
    section.description = tr("Знак операции 'примерно равно'.");
    section.examples  << QStringLiteral("a~~b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '-+'");
    section.description = tr("Знак операции 'минус-плюс'.");
    section.examples  << QStringLiteral("a-+b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '+-'");
    section.description = tr("Знак операции 'плюс-минус'.");
    section.examples  << QStringLiteral("a+-b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '->'");
    section.description = tr("Знак операции 'стремится к'.");
    section.examples  << QStringLiteral("a->b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '/'");
    section.description = tr("Знак операции деления. \n"
                             "Всегда используется деление в виде простой дроби. В сложных выражениях, \n"
                             "использующих различные символы умножения и деления в произвольном порядке \n"
                             "'интеллектуально' распределяет множители между числителем и знаменателем (пример 2). \n"
                             "Чтобы вынести множитель за пределы дроби, нужно дробь заключить в скобки (примеры 3, 4 и 5).");
    section.examples  << QStringLiteral("a/b") << QStringLiteral("(x+1)/(x-1)*(x+2)/(x-2)/(x//y)*4")
                      << QStringLiteral("(1/2)*x") << QStringLiteral("(3/4)*((x+1)/(x-1))")
                      << QStringLiteral("(3/4)*(x+1)/(x-1)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '//'");
    section.description = tr("Знак операции деления. \n"
                             "Используется косая черта. В некоторых случаях неправильно убирает скобки (пример 2). \n"
                             "В этом случае рекомендуется пользоваться скобками '!()' (пример 3).");
    section.examples  << QStringLiteral("a//b") << QStringLiteral("x//(2*y)") << QStringLiteral("x//!(2*y)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '/+'");
    section.description = tr("Знак операции деления.");
    section.examples  << QStringLiteral("a/+b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '&' или ' & '");
    section.description = tr("Конкатенация двух выражений. \n"
                             "Символ должен либо не отделяться от обоих выражений пробелами (пример 1), \n"
                             "либо отделяться от каждого из них одним пробелом (пример 2).");
    section.examples  << QStringLiteral("x&y") << QStringLiteral("x & y");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '_' (вектор)");
    section.description = tr("Символ используется для обозначения вектора, ставится перед выражением. \n"
                             "В некоторых случаях возможна неправильная расстановка скобок. \n"
                             "В таких случаях вместо символа рекомендуется использовать функцию Vect.");
    section.examples  << QStringLiteral("_a");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '_' (нижний индекс)");
    section.description = tr("Символ используется для обозначения нижнего индекса. \n"
                             "В некоторых случаях возможна неправильная расстановка скобок или многоступенчатых индексов (пример 2). \n"
                             "В таких случаях вместо символа рекомендуется использовать функцию Ind (пример 3). \n"
                             "Использование символа '_' для обозначения нижнего индекса возможно только если "
                             "ExpressionBuilder::postSymbols() == true (по умолчанию)");
    section.examples  << QStringLiteral("a_b") << QStringLiteral("a_x_0") << QStringLiteral("Ind(a,Ind(x,0))");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '^'");
    section.description = tr("Символ используется для обозначения верхнего индекса или возведения в степень. \n"
                             "В некоторых случаях возможна неправильная расстановка скобок или многоступенчатых индексов. \n"
                             "В таких случаях вместо символа рекомендуется использовать функцию Pow. \n"
                             "Если используется вместе с нижним индексом (символ '_' или функция Ind), \n"
                             "то сначала указывается нижний индекс, а затем - верхний (пример 2), \n"
                             "в противном случае индексы будут отображаться некорректно (пример 3).\n"
                             "Использование символа '^' возможно только, если ExpressionBuilder::postSymbols() == true (по умолчанию).");
    section.examples  << QStringLiteral("a^b") << QStringLiteral("Ind(x,a)^2, x_a^2") << QStringLiteral("Ind(x^2,a), x^2_a");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '`' (обратный апостроф)");
    section.description = tr("Используется для обозначения производной. \n"
                             "Допустимо использование нескольких знаков подряд для обозначения производных высших степеней. \n"
                             "В некоторых случаях возможна неправильная расстоновка скобок. В таких случаях вместо символа \n"
                             "рекомендуется использовать функцию Strokes. Использование символа '`' возможно только если \n"
                             "ExpressionBuilder::postSymbols() == true (по умолчанию).");
    section.examples  << QStringLiteral("f(x)`") << QStringLiteral("f(x)```");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '!'");
    section.description = tr("Символ факториала. \n"
                             "Использование символа '!' для обозначения факториала возможно только, если \n"
                             "ExpressionBuilder::postSymbols() == true (по умолчанию).");
    section.examples  << QStringLiteral("C_n^k=n!/k!/(n-k)!");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '!='");
    section.description = tr("Знак операции 'не равно' (в стиле C).");
    section.examples  << QStringLiteral("a!=b");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ ','");
    section.description = tr("Запятая, отделяющая несколько подряд идущих выражений. \n"
                             "После запятой может быть произвольное число пробелов, \n"
                             "однако это не влияет на расстояние между выражениями, которое составляет семь единиц толщины. \n"
                             "Перед запятой пробелов быть не должно.");
    section.examples  << QStringLiteral("a0,a1, a2,            a3");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Символ '#'");
    section.description = tr("Ставится перед цифровыми константами для указания, что они должны быть выведены в научном формате."
                             "Сравните пример 1 и пример 2.");
    section.examples  << QStringLiteral("0.03*x") << QStringLiteral("#0.03*x");
    m_help_list.append(section);
  }

// -------------------------------------------- Функции --------------------------------------------

  {
    HelpSection section;
    section.title = tr("Функция Sqrt");
    section.description = tr("Синтаксис: Sqrt(E). \n"
                             "Извлечение квадратного корня из E.");
    section.examples  << QStringLiteral("Sqrt(x^2+y^2)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Sqr");
    section.description = tr("Синтаксис: Sqr(E). \n"
                             "Возведение выражения E в квадрат. \n"
                             "Не имеет никаких преимуществ по сравнению с использованием символа '^'' или функции Pow. \n"
                             "Добавлена для совместимости с синтаксисом Паскаля.");
    section.examples  << QStringLiteral("Sqr(a+b)=Sqr(a)+2*a*b+Sqr(b)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Log");
    section.description = tr("Синтаксис: Log(E1,E2). \n"
                             "Логарифм E2 по основанию E1.");
    section.examples  << QStringLiteral("log(a,x+1)=ln(x+1)/ln(a)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Abs");
    section.description = tr("Синтаксис: Abs(E). \n"
                             "Модуль E. \n"
                             "Не имеет никаких преимуществ перед использованием скобок ||. \n"
                             "Добавлена для совместимости с синтаксисом Паскаля.");
    section.examples  << QStringLiteral("Abs(x^2)=Abs(x)^2");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Pow");
    section.description = tr("Синтаксис: Pow(E1,E2). \n"
                             "Возведение E1 в степень E2. \n"
                             "При использовании с функций Ind должна применяться после Ind (пример 2). \n"
                             "В большинстве случаев может быть заменена символом '^'.");
    section.examples  << QStringLiteral("Pow(x+2,2//3)") << QStringLiteral("Pow(Ind(x,a),3)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Root");
    section.description = tr("Синтаксис: Ind(E1,E2). \n"
                             "Добавление к E1 нижнего индекса в виде E2. \n"
                             "В большинстве случаев может быть заменена символом '_'. \n"
                             "При использовании с функций Pow должна применяться раньше Pow (пример 2).");
    section.examples  << QStringLiteral("Ind(a,n)")
                      << QStringLiteral("Pow(Ind(x,n),2)=(Pow(Ind(x,n+1),2)+Pow(Ind(x,n-1),2))/2");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Lim");
    section.description = tr("Синтаскис: Lim(E1,E2). \n"
                             "Предел выражения E2 при условии E1.");
    section.examples  << QStringLiteral("Lim(StandC(x->0,x>0),f(x))=1");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Func");
    section.description = tr("Синтаксис: Func(E1,E2). \n"
                             "Функция, 'именем' которой является E1, а аргументом - E2.");
    section.examples  << QStringLiteral("Func(PDiffRF(f,x,3),x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Space");
    section.description = tr("Синтаксис: Space(n). \n"
                             "Пробел размером в n единиц толщины. \n"
                             "Используется для разделения выражений.");
    section.examples  << QStringLiteral("y=x & space(7) & z=q");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Diff");
    section.description = tr("Синтксис: Diff(E1[,E2]). \n"
                             "Дифференциал E1 степени E2. \n"
                             "Аргумент E2 опциональный.");
    section.examples  << QStringLiteral("Diff(x)") << QStringLiteral("Diff(x,n)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция PDiff");
    section.description = tr("Синтаксис: PDiff(E1[,E2]). \n"
                             "'Частный дифференциал' E1 спепени E2. \n"
                             "Аргумент E2 опциональный. \n"
                             "С математической точки зрения подобный 'дифференциал' не имеет смысла, \n"
                             "но функция очень удобна для создания выражений типа примера 3.");
    section.examples  << QStringLiteral("PDiff(x)") << QStringLiteral("PDiff(x,n)")
                      << QStringLiteral("PDiffN(f(x,y),3)/PDiff(x)/PDiff(y,2)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция DiffN");
    section.description = tr("Синтаксис: DiffN(E1[,E2]). \n"
                             "Аргумент E2 опциональный. \n"
                             "Дифференциал степени E2 выражения E1.");
    section.examples  << QStringLiteral("DiffN(x)") << QStringLiteral("DiffN(x,n)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция PDiffN");
    section.description = tr("Синтаксис: PDiffN(E1[,E2]). \n"
                             "Аргумент E2 опциональный. \n"
                             "'Частный дифференциал'' спепени E2 выражения E1. \n"
                             "С математической точки зрения подобный 'дифференциал' не имеет смысла, \n"
                             "но функция очень удобна для создания выражений типа примера 3.");
    section.examples  << QStringLiteral("PDiffN(x)") << QStringLiteral("PDiffN(x,n)")
                      << QStringLiteral("PDiffN(f(x,y),3)/PDiff(x)/PDiff(y,2)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция DiffR");
    section.description = tr("Синтаксис: DiffR(E1[,E2]). \n"
                             "Аргумент E2 опциональный. \n"
                             "Полная производная по E1 степени E2.");
    section.examples  << QStringLiteral("DiffR(x)") << QStringLiteral("DiffR(x,n)")
                      << QStringLiteral("DiffR(x,2)*f(x)=DiffR(x)*DiffR(x)*f(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция PDiffR");
    section.description = tr("Синтаксис: PDiffR(E1[,E2]). \n"
                             "Аргумент E2 опциональный. \n"
                             "Частная производная по E1 степени E2.");
    section.examples  << QStringLiteral("PDiffR(x)") << QStringLiteral("PDiffR(x,n)")
                      << QStringLiteral("Nabla=PDiffR(x)*_e_x+PDiffR(y)*_e_y+PDiffR(z)*_e_z");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция DiffRF");
    section.description = tr("Синтаксис: DiffRF(E1,E2[,E3]). \n"
                             "Аргумент E3 опциональный. \n"
                             "Полная производная E1 по E2 степени E3.");
    section.examples  << QStringLiteral("DiffRF(f,x)") << QStringLiteral("DiffRF(f(x),x,n)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция PDiffRF");
    section.description = tr("Синтаксис: PDiffRF(E1,E2[,E3]). \n"
                             "Аргумент E3 опциональный. \n"
                             "Частная производная E1 по E2 степени E3.");
    section.examples  << QStringLiteral("PDiffRF(f,x)") << QStringLiteral("PDiffRF(f(x,y),x,n)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция String");
    section.description = tr("Синтаксис: String(Текст) или String(\"Текст\"). \n"
                             "Текст, выводящийся прямым шрифтом без изменений. \n"
                             "Если в тексте встречаются круглые скобки, он должен быть заключён в двойные кавычки.");
    section.examples  << QStringLiteral("String(Произвольный текст)")
                      << QStringLiteral("String(\"Текст (со скобками)\")");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Strokes");
    section.description = tr("Синтаксис: Strokes(E[,n]). \n"
                             "n - количество штрихов (опционально, по умолчанию n = 1) \n"
                             "Добавляет к E штрихи, обычно обозначающие производную.");
    section.examples  << QStringLiteral("Strokes(f(x))") << QStringLiteral("Strokes(y,3)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Fact");
    section.description = tr("Синтаксис: Fact(E). \n"
                             "Факториал E. \n"
                             "Функция может быть заменена символом '!' после E.");
    section.examples  << QStringLiteral("Fact(n)") << QStringLiteral("Fact(k+1)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция At");
    section.description = tr("Синтаксис: At(E1,E2). \n"
                             "Значение E1 при условии E2.");
    section.examples << QStringLiteral("At(DiffRF(f,x),x=0)=1");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Line");
    section.description = tr("Синтаксис: Line(E). \n"
                             "Горизонтальная черта над E.");
    section.examples << QStringLiteral("Line(x)") << QStringLiteral("Line(x^2)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Vect");
    section.description = tr("Синтаксис: Vect(E). \n"
                             "Стрелка (вектор) над E. \n"
                             "В большинстве случаев может быть заменена на символ '_' перед E.");
    section.examples << QStringLiteral("Vect(a)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Cap");
    section.description = tr("Синтаксис: Cap(E). \n"
                             "Знак 'крышка' над E.");
    section.examples << QStringLiteral("Cap(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Tilde");
    section.description = tr("Синтаксис: Tilde(E). \n"
                             "Знак тильды над E.");
    section.examples << QStringLiteral("Tilde(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Points");
    section.description = tr("Синтаксис: Points(E[,n]). \n"
                             "n - количество точек (опционально, по умолчанию n = 1) \n"
                             "Точки над E, обычно означающие производную по времени.");
    section.examples << QStringLiteral("Points(y,2)=y*Points(x)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция StandL");
    section.description = tr("Синтаксис: StandL(E[,...]). \n"
                             "Размещает несколько выражений одно под другим, выравнивая по левому краю.");
    section.examples << QStringLiteral("StandL(0<=i<n,i<>j)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция StandR");
    section.description = tr("Синтаксис: StandR(E[,...]). \n"
                             "Размещает несколько выражений одно под другим, выравнивая по правому краю.");
    section.examples << QStringLiteral("StandR(0<=i<n,i<>j)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция StandC");
    section.description = tr("Синтаксис: StandC(E[,...]). \n"
                             "Размещает несколько выражений одно под другим, выравнивая по центру.");
    section.examples << QStringLiteral("StandC(0<=i<n,i<>j)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Matrix");
    section.description = tr("Синтаксис: Matrix(n,m,E[,...]). \n"
                             "Матрица размером n на m."
                             "Выражения E и следующие за ним расставляются по ячейкам матрицы. \n"
                             "Выражений может быть меньше, чем n*m - в этом случае последние ячейки остаются пустыми. \n"
                             "Матрица не обрамляется скобками, скобки надо добавлять явно. ");
    section.examples << QStringLiteral("Matrix(2,3,x,y,x-y,x+y,z,z+y)")
                     << QStringLiteral("!(Matrix(2,2,1,2,-3,4))")
                     << QStringLiteral("[_a,_b]=|Matrix(3,3,_e_x,_e_y,_e_z,x_a,y_a,z_a,x_b,y_b,z_b)|");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Summa");
    section.description = tr("Синтаксис: Summa(E1[,E2[,E3]]). \n"
                             "Аргументы E2 и E3 опциональны. \n"
                             "Сумма выражений E1. Под знаком суммы ставится E2, над ним - E3.");
    section.examples << QStringLiteral("Summa(a_i)")
                     << QStringLiteral("Summa(a_i,i<>j)")
                     << QStringLiteral("Summa(a_i,i=0,n)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Prod");
    section.description = tr("Синтаксис: Prod(E1[,E2[,E3]]). \n"
                             "Аргументы E2 и E3 опциональны. \n"
                             "Произведение выражений E1. Под знаком произведения ставится E2, над ним - E3.");
    section.examples << QStringLiteral("Prod(a_i)")
                     << QStringLiteral("Prod(a_i,i<>j)")
                     << QStringLiteral("Prod(a_i,i=0,n)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Circ");
    section.description = tr("Синтаксис: Circ(E1[,E2[,E3]]). \n"
                             "Аргументы E2 и E3 опциональны. \n"
                             "Циркуляция выражения E1. Под знаком циркуляции ставится E2, над ним - E3.");
    section.examples << QStringLiteral("Circ(_E*Diff(_l),L)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Int");
    section.description = tr("Синтаксис: Int(E1[,E2[,E3]]). \n"
                             "Аргументы E2 и E3 опциональны. \n"
                             "Интеграл выражения E1. Под знаком интеграла ставится E2, над ним - E3.");
    section.examples << QStringLiteral("F(x)=Int(f(x)*Diff(x))")
                     << QStringLiteral("Phi=Int(_H*Diff(_S),S)")
                     << QStringLiteral("Int(x,0,1)=1");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция IntM");
    section.description = tr("Синтаксис: IntM(n,E1[,E2[,E3]]). \n"
                             "Аргументы E2 и E3 опциональны. \n"
                             "n-кратный интеграл выражения E1. \nПод знаком интеграла ставится E2, над ним - E3. \n"
                             "Если n = 0, рисуется интеграл с неизвестной кратностью (используется многоточие).");
    section.examples << QStringLiteral("IntM(3,f(x,y,z)*Diff(x)*Diff(y)*Diff(z),V)")
                     << QStringLiteral("IntM(0,f(x1,...,x_n)*DiffN(x,n))");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Case");
    section.description = tr("Синтаксис: Case(E[,...]). \n"
                             "Выбор одного из возможных вариантов. \n"
                             "Выражения в скобках идут в виде пар вариант-условие.");
    section.examples << QStringLiteral("|x|=Case(-x,x<0,0,x=0,x,x>0)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Comma");
    section.description = tr("Синтаксис: Comma(n). \n"
                             "Вставляет в выражение запятую, а после неё - пробел шириной в n единиц ширины.");
    section.examples << QStringLiteral("x & comma(15) & y");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Brackets");
    section.description = tr("Синтаксис: Brackets(S1S2,E). \n"
                             "Заключает E в различные скобки."
                             "В качестве S1 может стоять символ '(', '[', '{', '|' или '0'. \n"
                             "В качестве S2 может стоять символ ')', ']', '}', '|' или '0'. \n"
                             "Символ '0' означает отсутствие скобки с данной стороны.");
    section.examples << QStringLiteral("Brackets((],0&comma&1)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция System");
    section.description = tr("Синтаксис: System(E[,...]). \n"
                             "Объединяет выражения в систему.");
    section.examples << QStringLiteral("System(x+y=5,x*y=6)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Num");
    section.description = tr("Синтаксис: Num(R[,n1[,n2[,n3]]]). \n"
                             "Позволяет управлять форматом записи числа R. \n"
                             "Если порядок числа меньше или равен -n3, используется научная запись с точностью n1, \n"
                             "если больше - обычная запись с числом разрядов перед точкой n1 и общим n2. \n"
                             "По умолчанию n1=4, n2=4, n3=2.");
    section.examples << QStringLiteral("Num(0.00123456)")
                     << QStringLiteral("Num(0.00123456, 6)")
                     << QStringLiteral("Num(0.00123456, 4, 4, 3)")
                     << QStringLiteral("Num(0.00123456, 4, 6, 3)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Symbol");
    section.description = tr("Синтаксис: Symbol(n). \n"
                             "Вставляет в выражение символ с кодом n в кодировке Unicode.");
    section.examples << QStringLiteral("Symbol(198)=1")
                     << QStringLiteral("Symbol(216)&6")
                     << QStringLiteral("6586&Symbol(8381)");
    m_help_list.append(section);
  }

  {
    HelpSection section;
    section.title = tr("Функция Angle");
    section.description = tr("Синтаксис: Angle(Текст) или Angle(\"Текст\"). \n"
                             "Ставит перед текстом знак угла. \n"
                             "Если текст содержит круглые скобки, его следует заключать в кавычки.");
    section.examples << QStringLiteral("Angle(ABC)");
    m_help_list.append(section);
  }
}

// private slots

void WidgetHelp::_q_current_content_changed(QListWidgetItem *current, QListWidgetItem *previous)
{
  Q_UNUSED(previous)

  if (current) {
    using namespace MathExpressions;

    m_description->setPlainText(current->toolTip());
    const int content_index = m_list_contents->row(current);

    if (content_index < 0)
      return;

    ExpressionPtr expr = nullptr;

    ExpressionBuilder builder;
    const QStringList& examples = m_help_list.at(content_index).examples;
    const QPalette& my_palette = this->palette();
    const QColor color_highlight = my_palette.color(QPalette::Highlight);
    const QColor color_text = my_palette.color(QPalette::Text);

    for (int i = 0; i < examples.size(); ++i) {
      VariableExpression* expr_title = new VariableExpression(tr("Пример %1: ").arg(i + 1));
      expr_title->setColor(color_highlight);

      SimpleExpression* expr_example_text = new SimpleExpression(examples.at(i));
      expr_example_text->setColor(color_text);
      expr_title->addNext(expr_example_text);

      VariableExpression* expr_result_text = new VariableExpression(tr(" Результат: "));
      expr_result_text->setColor(color_highlight);
      expr_title->addNext(expr_result_text);

      std::unique_ptr<AbstractExpression> e = builder.exec(examples.at(i));
      e->setColor(color_text);
      expr_title->addNext(e.release());

      if (expr)
        expr->addNext(new ChainExpression(expr_title));
      else
        expr = new ChainExpression(expr_title);
    }

    m_expression.reset(new StandExpression(expr));
    QFont expr_font = m_examples->font();
    expr_font.setFamily(QStringLiteral("Times New Roman"));
    expr_font.setPointSize(expr_font.pointSize() + 6);
    m_expression->setFont(expr_font);
  }
  else {
    m_description->clear();
    m_expression.reset();
  }

  m_examples->setExpression(m_expression.get());
}

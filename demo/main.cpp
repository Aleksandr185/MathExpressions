
#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>
#include <QSplitter>
#include <QGroupBox>
#include <QVBoxLayout>

#include "widget_demo.h"
#include "widget_help.h"


using namespace MathExpressions;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QMainWindow w;

  QGroupBox* group_demo = new QGroupBox(QObject::tr("demo"));
  QVBoxLayout* lay_demo = new QVBoxLayout();
  WidgetDemo* widget_demo = new WidgetDemo();
  lay_demo->addWidget(widget_demo);
  group_demo->setLayout(lay_demo);

  QGroupBox* group_help = new QGroupBox(QObject::tr("help && doc"));
  QVBoxLayout* lay_help = new QVBoxLayout();
  lay_help->addWidget(new WidgetHelp());
  group_help->setLayout(lay_help);

  QSplitter* splitter = new QSplitter(Qt::Vertical);
  splitter->addWidget(group_demo);
  splitter->addWidget(group_help);
  splitter->setStretchFactor(0, 50);
  splitter->setStretchFactor(1, 50);

  w.setCentralWidget(splitter);

  QStatusBar* status_bar = w.statusBar();
  QObject::connect(widget_demo, &WidgetDemo::errorMessage,
                   [=](const QString& msg) { status_bar->showMessage(msg, 5000);} );

  w.resize(820, 780);
  w.show();

  return a.exec();
}

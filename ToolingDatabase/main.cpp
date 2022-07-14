#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(resource);

  QApplication::setStyle(QStyleFactory::create("fusion"));
  QApplication a(argc, argv);
  MainWindow w;
  w.setIconSize(QSize(32,32));
  w.show();
  return a.exec();
}

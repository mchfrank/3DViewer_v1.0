#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QApplication::setOrganizationName("elsamadn");
  QApplication::setApplicationName("3DV");

  MainWindow w;
  w.show();
  return a.exec();
}

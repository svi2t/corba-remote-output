#include "pch1.h"
#include "../remote-print.hh"
#include "servermanager.h"


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  setlocale(LC_ALL, "rus");

  ServerManager serverManager(argc, argv);

  QObject::connect(&serverManager, SIGNAL(finished()), &a, SLOT(quit()));
  QTimer::singleShot(0, &serverManager, SLOT(startManager()));
  return a.exec();
}

#include "pch.h"
#include "../remote-print.hh"
#include "remoteoutputclient.h"


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  setlocale(LC_ALL, "rus");

  RemoteOutputClient outputClient(argc, argv);
  QObject::connect(&outputClient, SIGNAL(finished()), &a, SLOT(quit()));
  QTimer::singleShot(0, &outputClient, SLOT(startRemoteOutputClient()));
  return a.exec();
}

#include "outputserver.h"
#include <iostream>
#include <QString>
#include <QByteArray>

OutputServer::OutputServer() :qcout(stdout)
{
  codecLocal = QTextCodec::codecForLocale();
  return;
}

short OutputServer::setRemoteCodec(int MiB)
{
  codecRemote = QTextCodec::codecForMib(MiB);
  return 1;
}

short OutputServer::echoChar(unsigned int charOutUInt)
{
  QChar charOut = charOutUInt;
  ba.clear();
  ba += charOut;
  if (codecLocal->mibEnum() == codecRemote->mibEnum())
    qcout << ba;
  else
    qcout << codecLocal->fromUnicode(codecRemote->toUnicode(ba));
  qcout.flush();
  return 1;
}

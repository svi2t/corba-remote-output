#include "outputserver.h"
#include <iostream>
#include <QString>
#include <QByteArray>

OutputServer::OutputServer() :qcout(stdout)
{
  codecLocal = QTextCodec::codecForLocale();
  return;
}

void OutputServer::printGreetingMessage()
{
  qcout << codecLocal->fromUnicode("Сервер запущен. Ожидаем прием данных. "
            "Для завершения нажмите <ctrl>+c\n");
  qcout.flush();
  return;
}

short OutputServer::echoChar(int codecRemote, unsigned int charOutUInt)
{
  // поскольку возможно подключение нескольких клиентов, codecRemote заранее устанавливать не получится
  // (можно также хранить кодировки всех подключенных клиентов, потом соотносить)
  QChar charOut = charOutUInt;
  ba.clear();
  ba += charOut;
  if (codecLocal->mibEnum() == codecRemote)
    qcout << ba;
  else
    qcout << codecLocal->fromUnicode(QTextCodec::codecForMib(codecRemote)->toUnicode(ba));
  qcout.flush();
  return 1;
}

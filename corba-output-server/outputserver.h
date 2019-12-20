#ifndef OUTPUTSERVER_H
#define OUTPUTSERVER_H

#include "../remote-print.hh"
#include <QTextStream>
#include <QTextCodec>

class OutputServer : public POA_Example::Echo
{
public:
  OutputServer();
  virtual ~OutputServer() {}
  virtual short setRemoteCodec(int MiB);
  virtual short echoChar(unsigned int mesg);

  QTextStream qcout;
  QByteArray ba;
  QTextCodec *codecRemote;
  QTextCodec *codecLocal;
};


#endif // OUTPUTSERVER_H

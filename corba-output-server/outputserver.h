#ifndef OUTPUTSERVER_H
#define OUTPUTSERVER_H

#include "pch1.h"
#include "../remote-print.hh"


class OutputServer : public POA_Example::Echo
{
public:
  OutputServer();
  virtual ~OutputServer() {}
  void printGreetingMessage();
  virtual short echoChar(int codec, unsigned int mesg);

private:
  int codect;
  QTextStream qcout;
  QByteArray ba;
  QTextCodec *codecLocal;
};


#endif // OUTPUTSERVER_H

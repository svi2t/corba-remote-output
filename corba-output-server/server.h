#ifndef SERVER_H
#define SERVER_H

#include "../remote-print.hh"
#include <QTextStream>

class Server : public POA_Example::Echo
{
public:
  inline Server() :qcout(stdout) {}
  virtual ~Server() {}
  virtual unsigned int echoChar( unsigned int mesg);

  QTextStream qcout;
};


#endif // SERVER_H

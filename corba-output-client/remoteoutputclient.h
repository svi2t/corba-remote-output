#ifndef REMITECLIENT_H
#define REMITECLIENT_H

#include "pch.h"
#include "../remote-print.hh"

class RemoteOutputClient : public QObject
{
  Q_OBJECT
public:
  explicit RemoteOutputClient(int argc, char *argv[], QObject *parent = nullptr);
  ~RemoteOutputClient();

private:
  bool setupClient();
  bool getCharsFromConsole();
  void finishInputChar();


  CORBA::ORB_var orb;
  CORBA::Object_var rootContextObj;
  CosNaming::NamingContext_var nc;
  ::Example::Echo_var remoteServer;

public slots:
  void startRemoteOutputClient();

signals:
  void finished();


};

#endif // REMITECLIENT_H

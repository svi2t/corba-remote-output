#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include "pch1.h"
#include "outputserver.h"

class ServerManager : public QObject
{
  Q_OBJECT
public:
  explicit ServerManager(int argc, char *argv[], QObject *parent = nullptr);
  ~ServerManager();

private:
  bool setupServer();
  bool runServer();

  CORBA::ORB_var orb;

  CORBA::Object_var poa_obj;
  PortableServer::POA_var poa;
  PortableServer::POAManager_var manager;
  OutputServer *server;

public slots:
  void startManager();
signals:
  void finished();
};

#endif // SERVERMANAGER_H

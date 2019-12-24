#ifndef REMITECLIENT_H
#define REMITECLIENT_H

#include <QObject>
#include "../remote-print.hh"

class RemoteOutputClient : public QObject
{
  Q_OBJECT
public:
  explicit RemoteOutputClient(::Example::Echo_var _remoteServer, QObject *parent = nullptr);
  void getCharsFromConsole();

private:
  void finishInputChar();

  ::Example::Echo_var remoteServer;
};

#endif // REMITECLIENT_H

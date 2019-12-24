#include "pch.h"
#include <omniORB4/CORBA.h>
#include <omniORB4/Naming.hh>
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

  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
    CORBA::Object_var rootContextObj =
        orb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var nc =
        CosNaming::NamingContext::_narrow(rootContextObj.in());

    CosNaming::Name name;
    name.length(1);
    name[0].id = CORBA::string_dup("echo_s");
    name[0].kind = CORBA::string_dup("");
    CORBA::Object_var managerObj = nc->resolve(name);

    ::Example::Echo_var remoteServer = ::Example::Echo::_narrow(managerObj.in());

    if (CORBA::is_nil(remoteServer))
    {
      qWarning() << "Can't narrow reference." << endl;
      return -1;
    }

   // RemoteOutputClient outputClient(remoteServer);
   // outputClient.getCharsFromConsole();

    orb->destroy();

  }
  catch (CORBA::TRANSIENT&)
  {
    qWarning() << "Невозможно подключится к серверу\n";
  }
  catch (CORBA::SystemException& ex)
  {
    qWarning() << "Возникла ошибка при работе с ORB. Системное исключение CORBA::" << ex._name() << "\n";
  }
  catch (CORBA::Exception& ex)
  {
    qWarning() << "Возникла ошибка при работе с ORB. Исключение CORBA::" << ex._name() << "\n";
  }
  catch (...)
  {
    qWarning() << "Возникла неопределенная ошибка.\n";
  }

  return 0;
}

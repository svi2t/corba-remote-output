#include <QCoreApplication>
#include <QTextStream>
#include <omniORB4/CORBA.h>
#include <omniORB4/Naming.hh>
#include "../remote-print.hh"
#include <QDebug>
#include <QTextCodec>
#include "../qaux.h"


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  globalSetLocale();


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
      return 1;
    }
    short res = 88;

    remoteServer->setRemoteCodec(QTextCodec::codecForLocale()->mibEnum());
    res = remoteServer->echoChar(static_cast<unsigned int>('e'));

    orb->destroy();

    //QTimer::
    return a.exec();
  }
  catch (CORBA::TRANSIENT&)
  {
    qWarning() << toLocalCodec("Caught system exception TRANSIENT - unable to contact the server \n");
  }
  catch (CORBA::SystemException& ex)
  {
    qWarning() << toLocalCodec("Caught a CORBA::") << ex._name() << "\n";
  }
  catch (CORBA::Exception& ex)
  {
    qWarning() << toLocalCodec("Caught CORBA::Exception: ") << ex._name() << "\n";
  }
  catch (...)
  {
    qWarning() << toLocalCodec("Что-то пошло не так...\n");
  }

  return 1;
}

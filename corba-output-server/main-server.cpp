#include <QCoreApplication>
#include <QTextStream>
#include <omniORB4/CORBA.h>
#include <omniORB4/Naming.hh>
#include "../remote-print.hh"
#include "outputserver.h"
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  setlocale(LC_ALL, "rus");

  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(poa_obj);
    PortableServer::POAManager_var manager = poa->the_POAManager();

    OutputServer *server = new OutputServer();
    CORBA::Object_var ns_obj = orb->resolve_initial_references("NameService");
    if (!CORBA::is_nil(ns_obj))
    {
       CosNaming::NamingContext_ptr nc = CosNaming::NamingContext::_narrow(ns_obj);
       CosNaming::Name name;
       name.length(1);
       name[0].id = CORBA::string_dup("echo_s");
       name[0].kind = CORBA::string_dup("");
       nc->rebind(name, server->_this());
    }

    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    server->printGreetingMessage();
    orb->run();
    orb->destroy();
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

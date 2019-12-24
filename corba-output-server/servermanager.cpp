#include "servermanager.h"

ServerManager::ServerManager(int argc, char *argv[], QObject *parent) : QObject(parent)
{
   orb = CORBA::ORB_init(argc, argv);
}

ServerManager::~ServerManager()
{
  orb->destroy();
}

void ServerManager::startManager()
{
  if (setupServer())
    runServer();
  emit finished();
}
bool ServerManager::setupServer()
{
  try
  {
    poa_obj = orb->resolve_initial_references("RootPOA");
    poa = PortableServer::POA::_narrow(poa_obj);
    manager = poa->the_POAManager();

    server = new OutputServer();
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
    return true;
  }
  catch (CORBA::SystemException& ex)
  {
    qWarning() << "Возникла ошибка при инициализации работы с ORB. Системное исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (CORBA::Exception& ex)
  {
    qWarning() << "Возникла ошибка при инициализации работы с ORB. Исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (...)
  {
    qWarning() << "Возникла неопределенная ошибка при инициализации работы с ORB.\n";
    return false;
  }
}

bool ServerManager::runServer()
{
  try
  {
    server->printGreetingMessage();
    orb->run();
  }
  catch (CORBA::SystemException& ex)
  {
    qWarning() << "Возникла ошибка во время работы сервера. Системное исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (CORBA::Exception& ex)
  {
    qWarning() << "Возникла ошибка во время работы сервера. Исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (...)
  {
    qWarning() << "Возникла неопределенная ошибка во время работы с ORB.\n";
    return false;
  }
  return true;
}

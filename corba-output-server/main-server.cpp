#include <QCoreApplication>
#include <QTextStream>
#include <omniORB4/CORBA.h>
#include <omniORB4/Naming.hh>
#include "../remote-print.hh"
#include "server.h"
#include <QDebug>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  setlocale(LC_CTYPE, "rus");
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(poa_obj);
    PortableServer::POAManager_var manager = poa->the_POAManager();
//    CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
//    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

    // Вариант 1
    Server* server = new Server();
    CORBA::Object_var ns_obj = orb->resolve_initial_references("NameService");
    if (!CORBA::is_nil(ns_obj))
    {
       CosNaming::NamingContext_ptr nc = CosNaming::NamingContext::_narrow(ns_obj);
       CosNaming::Name name;
       name.length(1);
       // name[0].id = CORBA::string_dup(SERVER_NAME);
       name[0].id = CORBA::string_dup("echo_s");
       name[0].kind = CORBA::string_dup("");
       nc->rebind(name, server->_this());

    }

//    HelloServant * hello = new HelloServant;

//    try {
//        //------------------------------------------------------------------------
//        // Bind object to name service as defined by directive InitRef
//        // and identifier "NameService" in config file omniORB.cfg.
//        //------------------------------------------------------------------------
//        CORBA::Object_var ns_obj = orb->resolve_initial_references("NameService");
//        if (!CORBA::is_nil(ns_obj)) {
//            //------------------------------------------------------------------------
//            // Narrow this to the naming context
//            //------------------------------------------------------------------------
//            CosNaming::NamingContext_ptr nc = CosNaming::NamingContext::_narrow(ns_obj);

//            //------------------------------------------------------------------------
//            // Bind to CORBA name service. Same name to be requested by client.
//            //------------------------------------------------------------------------
//            CosNaming::Name name;
//            name.length(1);
//            // name[0].id = CORBA::string_dup(SERVER_NAME);
//            name[0].id = CORBA::string_dup("");
//            name[0].kind = CORBA::string_dup("hello");
//            nc->rebind(name, hello->_this());

//            //------------------------------------------------------------------------
//            // Intizialization ready, server runs
//            //------------------------------------------------------------------------
//            cout << argv[0] << " C++ server is running .." << endl;
//        }


 // Вариант 2
//    PortableServer::Servant_var<Server> server = new Server();
//    poa->activate_object(server);

    CORBA::String_var sior(orb->object_to_string(server->_this()));
    qWarning() << sior << endl;

    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    orb->run();
    orb->destroy();
  }
  catch (CORBA::SystemException& ex)
  {
    qWarning() << "Caught CORBA::" << ex._name() << "\n";
  }
  catch (CORBA::Exception& ex)
  {
    qWarning() << "Caught CORBA::Exception: " << ex._name() << "\n";
  }
  return 0;

  //return a.exec();
}

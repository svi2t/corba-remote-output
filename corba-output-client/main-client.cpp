#include <QCoreApplication>
#include <QTextStream>
#include <omniORB4/CORBA.h>
#include <omniORB4/Naming.hh>
#include "../remote-print.hh"
#include <QDebug>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  setlocale(LC_CTYPE, "rus");

  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    qDebug() << argv[2];


    // Get a reference to the Naming Service
    CORBA::Object_var rootContextObj =
        orb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var nc =
        CosNaming::NamingContext::_narrow(rootContextObj.in());

    CosNaming::Name name;
    name.length(1);
    name[0].id = CORBA::string_dup("echo_s");
    name[0].kind = CORBA::string_dup("");
    CORBA::Object_var managerObj = nc->resolve(name);
    // Narrow the previous object to obtain the correct type
    //::Example::Echo manager =
//        ::Example::_narrow(managerObj.in());
    ::Example::Echo_var manager = ::Example::Echo::_narrow(managerObj.in());

 //    // Narrow the previous object to obtain the correct type
 //    ::CaesarAlgorithm_var manager =
 //        ::CaesarAlgorithm::_narrow(managerObj.in()
    // -------


      if (CORBA::is_nil(manager))
      {
         qWarning() << "Can't narrow reference." << endl;
         return 1;
      }


    unsigned int res = 88;

    res = manager->echoChar(static_cast<unsigned int>('e'));

    qWarning() << res;




    //hello(echoref);
    orb->destroy();
  }
  catch (CORBA::TRANSIENT&)
  {
    qWarning() << "Caught system exception TRANSIENT -- unable to contact the "
         << "server." << "\n";
  }
  catch (CORBA::SystemException& ex)
  {
    qWarning() << "Caught a CORBA::" << ex._name() << "\n";
  }
  catch (CORBA::Exception& ex)
  {
    qWarning() << "Caught CORBA::Exception: " << ex._name() << "\n";
  }
  return 0;

}

#include "pch.h"
#include "remoteoutputclient.h"
#ifdef Q_OS_LINUX
#include <ncursesw/ncurses.h>
#endif
#ifdef Q_OS_WIN
#include <conio.h>
#endif

#define CSTR(str) QString::fromUtf8(str).toLocal8Bit()

RemoteOutputClient::RemoteOutputClient(int argc, char *argv[], QObject *parent) : QObject(parent)
{
  orb = CORBA::ORB_init(argc, argv);
}

RemoteOutputClient::~RemoteOutputClient()
{
  orb->destroy();
}

void RemoteOutputClient::startRemoteOutputClient()
{
  if (setupClient())
    getCharsFromConsole();
  emit finished();
}

bool RemoteOutputClient::setupClient()
{
  try
  {
    rootContextObj = orb->resolve_initial_references("NameService");
    nc = CosNaming::NamingContext::_narrow(rootContextObj.in());

    CosNaming::Name name;
    name.length(1);
    name[0].id = CORBA::string_dup("echo_s");
    name[0].kind = CORBA::string_dup("");
    CORBA::Object_var managerObj = nc->resolve(name);

    remoteServer = ::Example::Echo::_narrow(managerObj.in());

    if (CORBA::is_nil(remoteServer))
    {
      qWarning() << "Can't narrow reference." << endl;
      return false;
    }
    return true;
  }
  catch (CORBA::TRANSIENT&)
  {
    qWarning() << "Невозможно подключится к серверу\n";
    return false;
  }
  catch (CORBA::SystemException& ex)
  {
    qWarning() << "Возникла ошибка при работе с ORB. Системное исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (CORBA::Exception& ex)
  {
    qWarning() << "Возникла ошибка при работе с ORB. Исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (...)
  {
    qWarning() << "Возникла неопределенная ошибка.\n";
  }

}

bool RemoteOutputClient::getCharsFromConsole()
{
  int currentCodec = QTextCodec::codecForLocale()->mibEnum();
  // QCoreApplication::processEvents() не получится здесь использовать,
  // поскольку не совместим с режимом работы ncursesksys
#ifdef Q_OS_WIN
  try
  {
    system("cls");
    printf(CSTR("Отправка данных...\r\n"));
    uint c;
    while (true)
    {
      while (true)
      {
        c = getche();
        remoteServer->echoChar(currentCodec, static_cast<uint>(c));
        if (c == 26 || c == 3) // <ctrl> + c и <ctrl> + z
        {
          finishInputChar();
          return 1;
        }
        if (c == '\r')
        {
          printf("\r\n");
          break;
        }
      }

      printf(CSTR("Если нужно продолжить отправку, нажмите Y(y) или Д(д)\r\n"));
      c = getch();
      if (c != static_cast<int>('Y') && c != static_cast<int>('y') && c != 228 && c != 196) // Д и д в win1251 (сам файл *.cpp в utf8)
      {
        printf(CSTR("Завершение работы программы. Нажмите любую клавишу\r\r"));
        c = getche();
        break;
      }
      printf(CSTR("Новая отправка данных...\r\n"));

    }
  }
#endif
#ifdef Q_OS_LINUX

  if (initscr() == nullptr)
  {
    qCritical() << "Невозможно войти в режим посимвольного считывания (ncurses)";
    return false;
  }
  timeout(-1);
  echo();
  try
  {
    uint c = 0;
    printw("Отправка данных...\n");
    while (true)
    {
      while(true)
      {
        get_wch(&c);
        remoteServer->echoChar(currentCodec, c);
        if (c == 26 || c == 3)
        {
          finishInputChar();
          return true;
        }
        if (c == '\n')
        {
          putwchar(static_cast<wchar_t>(c));
          break;
        }
      }
      printw("Если нужно продолжить отправку, нажмите Y(y) или Д(д)\n");
      noecho();
      get_wch(&c);
      echo();
      if (c != static_cast<int>('y') && c != static_cast<int>(U'д') &&
          c != static_cast<int>('Y') && c != static_cast<int>(U'Д') )
      {
        printw("Завершение работы программы. Нажмите любую клавишу");
        get_wch(&c);
        break;
      }
      printw("Новая отправка данных...\n");
    }
  }
#endif
  catch (CORBA::TRANSIENT&)
  {
    finishInputChar();
    qWarning() << "Невозможно подключится к серверу\n";
    return  false;
  }
  catch (CORBA::COMM_FAILURE& ex)
  {
    finishInputChar();
    qWarning() << "Связь с сервером потеряна" ;
    return  false;
  }
  catch (CORBA::SystemException& ex)
  {
    finishInputChar();
    qWarning() << "Возникла ошибка при работе с ORB. Системное исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (CORBA::Exception& ex)
  {
    finishInputChar();
    qWarning() << "Возникла ошибка при работе с ORB. Исключение CORBA::" << ex._name() << "\n";
    return false;
  }
  catch (...)
  {
    finishInputChar();
    qWarning() << "Неопределенная ошибка\n";
    return false;
  }
  finishInputChar();
  return true;
}

void RemoteOutputClient::finishInputChar()
{
#ifdef Q_OS_WIN
  system("cls");
#endif
#ifdef Q_OS_LINUX
  refresh();
  endwin();
#endif
}

#include "remoteoutputclient.h"
#ifdef Q_OS_LINUX
#include <ncursesw/ncurses.h>
#endif
#ifdef Q_OS_WIN
#include <conio.h>
#endif
#include <QTextCodec>
#include <QString>
#include <QDebug>

#define CSTR(str) QString::fromUtf8(str).toLocal8Bit()

RemoteOutputClient::RemoteOutputClient(::Example::Echo_var _remoteServer, QObject *parent) : QObject(parent)
{
  remoteServer = _remoteServer;
}

void RemoteOutputClient::getCharsFromConsole()
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
    return;
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
          return ;
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
  }
  catch (CORBA::COMM_FAILURE& ex)
  {
    finishInputChar();
    qWarning() << "Связь с сервером потеряна" ;
  }
  catch (CORBA::SystemException& ex)
  {
    finishInputChar();
    qWarning() << "Возникла ошибка при работе с ORB. Системное исключение CORBA::" << ex._name() << "\n";
  }
  catch (CORBA::Exception& ex)
  {
    finishInputChar();
    qWarning() << "Возникла ошибка при работе с ORB. Исключение CORBA::" << ex._name() << "\n";
  }
  catch (...)
  {
    finishInputChar();
    qWarning() << "Неопределенная ошибка\n";
  }
  finishInputChar();
  return;
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

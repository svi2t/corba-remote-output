#include "remoteoutputclient.h"
#include "ncurses.h"

RemoteOutputClient::RemoteOutputClient(::Example::Echo_var _remoteServer, QObject *parent) : QObject(parent)
{
  remoteServer = _remoteServer;
  connect(this, SIGNAL(needNewCharInput()), this, SLOT(getNextCharInput()),Qt::QueuedConnection);
}

void RemoteOutputClient::prepareCharInput()
{
  initscr();
  timeout(-1);
  echo();
  uint c = 0;
  printw("Отправка данных...\n");
  emit needNewCharInput();
  refresh();
  endwin();

}

void RemoteOutputClient::getNextCharInput()
{
  uint c;
  while (true)
  {
    get_wch(&c);
    if (c == 26 || c == 3)
    {
      refresh();
      endwin();
      return ;
    }
    if (c == '\n')
    {
      putwchar(c);
      break;
    }
  }
  printw("Если нужно продожить отправку, нажмите Y(y) или Д(д)\n");
  noecho();
  get_wch(&c);
  echo();
  if (c != static_cast<int>('y') && c != static_cast<int>(U'д') &&
      c != static_cast<int>('Y') && c != static_cast<int>(U'Д') )
  {
    printw("Завершение работы программы. Нажмите любую клавишу");
    get_wch(&c);
    return;
  }
  printw("Новая отправка данных...\n");
  emit needNewCharInput();
  return;
}

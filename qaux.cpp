#include "qaux.h"

void globalSetLocale()
{
#ifdef Q_OS_LINUX
  setlocale(LC_ALL, "");
#endif
#ifdef Q_OS_WIN

#endif
}


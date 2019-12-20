#include <QString>

void globalSetLocale();

#ifdef Q_OS_LINUX
#define toLocalCodec(str) str
#endif
#ifdef Q_OS_WIN
#define toLocalCodec(str) QString::fromUtf8(str).toLocal8Bit();
#endif

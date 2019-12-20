#include "server.h"
#include <iostream>

short Server::setCodecForServer(int codec)
{
  return 1;
}

short Server::echoChar(unsigned int charOut)
{
  qcout << "Got " << charOut ;
  qcout.flush();
  return 1;
}

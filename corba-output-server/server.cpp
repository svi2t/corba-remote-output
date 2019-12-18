#include "server.h"
#include <iostream>

unsigned int Server::echoChar(unsigned int charOut)
{
  qcout << "Got " << charOut ;
  qcout.flush();
  return 779;
}

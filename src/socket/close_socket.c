/***************************************************************************/
/**                                                                       **/
/**                  c  l  o  s  e  _  s  o  c  k  e  t  .  c             **/
/**                                                                       **/
/**     Function closes socket                                            **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 20.11.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "types.h"
#include "channel.h"

void close_socket(Socket *socket)
{
#ifdef _WIN32
  closesocket(socket->channel);
  WSACleanup();
#else
  close(socket->channel);
#endif
  free(socket);
} /* of 'close_socket' */

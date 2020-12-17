/***************************************************************************/
/**                                                                       **/
/**               f  c  l  o  s  e  _  s  o  c  k  e  t  .  c             **/
/**                                                                       **/
/**     FORTRAN interface to close_socket                                 **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.01.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "types.h"
#include "channel.h"

void close_socket_(Socket *socket,int *err)
{
#ifdef _WIN32
  *err=closesocket(socket->channel);
  WSACleanup();
#else
  *err=close(socket->channel);
#endif
} /* of 'close_socket' */
